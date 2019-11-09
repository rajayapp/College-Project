#include <iostream>
#include <sstream>//istringstream
#include <string>
#include <vector>
#include <iterator>
#include <sys/sendfile.h>//sendfile()
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <cstdio>
#include <fstream>



using namespace std;



int RandomNumberGenerator(int i)
{
	return rand() % i;
}



void ArrayXOR(int *a, int *b, int start, int len)
{
	for(int i=start;i<len;i++)
		*(a+i) = *(a+i)^*(b+i);//a[i] = a[i] ^ b[i];
}



void CRC16(int *a, int *remainder)
{
	int divisor[17]={1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1};
	int temp[17]={0};
	int len = 17;

	memset(remainder,0,16*4);
	
	cout<<"Received message."<<endl;
	for(int j=2;j<34;j++)
		cout<<a[j]<<" ";
	
	cout<<endl;
	
	copy(a+2,a+19,temp);
	
	while(len < 32)
	{
		
		if(temp[0] == 1)
		{
			ArrayXOR(temp,divisor,0,17);
			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = a[len+2];			
		}	
		else
		{			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = a[len+2];
		}
		
		len += 1;
		
	}
	
	if(temp[0] == 1)
	{
		ArrayXOR(temp,divisor,0,17);
		
		copy(temp+1,temp+17,remainder);
	}	
	else
	{
		copy(temp+1,temp+17,remainder);
	}
	
	cout<<"remainder is :- "<<endl;
	for(int j=0;j<16;j++)
		cout<<remainder[j]<<" ";
		
	cout<<endl;	

}



int sum(int *a,int size)
{
	int s=0;

	for(int i=0;i<size;i++)
		s += a[i];

	return s;
}



//Request for the frame which is in error untill you get the correct one.
bool Receive(int n, int p, int *a, int clientSocketId)
{
	int delay;
	int drop;
	
	int temp;

	const char *dnack[] = {"NACK0D","NACK1D","NACK2D","NACK3D"};
	const char *nack[] = {"NACK0","NACK1","NACK2","NACK3"};
	
	cout<<"NackToken : "<<n<<endl;
	
	while(true)
	{
		delay = RandomNumberGenerator(2);
		drop = RandomNumberGenerator(2);

		if(!drop)
		{
			if(delay)
			{
				cout<<"NACK"<<n<<" Delayed for packet : "<<p+1<<endl;

				usleep(3150000);
				
				temp = send( clientSocketId,dnack[n],sizeof(dnack[n]), 0);
											
			}
			else
			{
				temp = send(clientSocketId, nack[n], sizeof( nack[n] ), 0);
			}

			if(temp == -1)
			{
				perror("The following error occurred");	
				return -1;
			}
			else
				cout<<"Sent NACK"<<n<<" for packet : "<<p+1<<"\nsent : "<<temp<<" bytes."<<endl;
		}
		else
			cout<<"NACK"<<n<<" Dropped for packet : "<<p+1<<endl;
			
		
		temp = read(clientSocketId, a, 34*4);
		
		cout<<"Received frame token No. :- "<<a[0]<<"   For Packet :- "<<p+1<<endl;
				
		if(temp == -1)
		{
			cout<<"Data could not be received :"<<endl;
			perror("The following error occurred");	
		}
		else if(temp == 0)
			cout<<"Connection closed by server."<<endl;
			
		if(n == a[0])
		{
			int remainder[16]={0};

			CRC16(a, remainder);

			int s1 = sum(remainder, 16);
			
			if(s1 == 0)
				break;	
		}
			
	}

	return true;
}	



int main(int argc, char **argv)
{
	if(argc != 2)
	{
		cout<<"Usage syntax: ./server <port_no>"<<endl;
		return 0;
	}
	
	int serverSocketId;
	
	serverSocketId = socket(PF_INET, SOCK_STREAM, 0);
	
	if(serverSocketId == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout<<"Socket successfully created."<<endl;


	
	struct sockaddr_in serverSocket;
	
	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(atoi(argv[1]));
	serverSocket.sin_addr.s_addr = INADDR_ANY;
	memset(serverSocket.sin_zero,0,sizeof(serverSocket.sin_zero));	
	
	
	
	//Bindig the Socket to a port
	int temp = bind(serverSocketId, (struct sockaddr *)&serverSocket, sizeof(struct sockaddr));

	if(temp == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout << "Server started running at "<< inet_ntoa(serverSocket.sin_addr)  << ":" << argv[1] << endl;



	//Listen to the port
	temp = listen(serverSocketId, 10);

	if(temp == -1)
	{
		perror("The following error occurred");	
		return -1;
	}

	struct sockaddr clientSocket;
	socklen_t addrlen = sizeof(clientSocket);

	int clientSocketId = accept(serverSocketId, (struct sockaddr *)&clientSocket, &addrlen);//?Doubt if it will work or no

	if(clientSocketId == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout<<"Connection accepted."<<endl;
	
//#########################################################################################################################################

	int token = 3;//last accepted frame number in receiver window
	ofstream f("output.txt");
	int packetCount = 0;//Number of packets currently accepted
	int window[] = {9,9};
	int win = 1;//current window number which has a accepted frame

	const char *ack[] = {"ACK0","ACK1","ACK2","ACK3"};
	const char *sack[] = {"ACK0S","ACK1S","ACK2S","ACK3S"};
	const char *sdack[] = {"ACK0SD","ACK1SD","ACK2SD","ACK3SD"};
	const char *dack[] = {"ACK0D","ACK1D","ACK2D","ACK3D"};
	const char *nack[] = {"NACK0","NACK1","NACK2","NACK3"};
	const char *snack[] = {"NACK0S","NACK1S","NACK2S","NACK3S"};
	const char *sdnack[] = {"NACK0SD","NACK1SD","NACK2SD","NACK3SD"};
	const char *dnack[] = {"NACK0D","NACK1D","NACK2D","NACK3D"};

	int received_int_1[34]={0};//First Frame
	int received_int_2[34]={0};//Second Frame
	
	int fdmax = 0;
	fd_set master, read_fds;
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	FD_SET(clientSocketId, &master);
	fdmax = clientSocketId;
	
	struct timeval tv;
	
	tv.tv_sec = 1;//3;
	tv.tv_usec = 0;//500000;//3000000;
	
	bool flag1,flag2;
	
	int remainder[16]={0};
	int s1=0,s2=0;

	while(packetCount < 4)
	{
		flag1 = false;
		flag2 = false;
		
		//Receive the First Frame
		read_fds = master;
			
		int numFds = select(fdmax+1, &read_fds, NULL, NULL, &tv);
		
		if(numFds == -1)
		{
			perror("Select() ");	
			return -1;
		}
		else if(numFds > 0)
		{
			temp = read(clientSocketId, received_int_1, sizeof(received_int_1));
					
			if(temp == -1)
			{
				cout<<"Data could not be received :"<<endl;
				perror("The following error occurred");	
			}
			else if(temp == 0)
				cout<<"Connection closed by server."<<endl;
			else
				cout<<"(First Frame)Received frame token No. :- "<<received_int_1[0]<<"   For Packet :- "<<packetCount+1<<endl;

			memset(remainder,0,16*4);
			
			CRC16(received_int_1, remainder);

			s1 = sum(remainder, 16);
			
			flag1 = true;
		}
		
	
		//Receive the Second Frame	
		read_fds = master;
			
		numFds = select(fdmax+1, &read_fds, NULL, NULL, &tv);
		
		if(numFds == -1)
		{
			perror("Select() ");	
			return -1;
		}
		else if(numFds > 0)
		{
			temp = read(clientSocketId, received_int_2, sizeof(received_int_2));
					
			if(temp == -1)
			{
				cout<<"Data could not be received :"<<endl;
				perror("The following error occurred");	
			}
			else if(temp == 0)
				cout<<"Connection closed by server."<<endl;	
			else
				cout<<"(Second Frame)Received frame token No. :- "<<received_int_2[0]<<"   For Packet :- "<<packetCount+2<<endl;
			
			memset(remainder,0,16*4);
			
			CRC16(received_int_2, remainder);

			s2 = sum(remainder, 16);
			
			flag2 = true;
		}
		
		int delay = RandomNumberGenerator(2);
		int drop = RandomNumberGenerator(2);

		//cout<<"Flag 1 : "<<flag1<<" & Flag 2 : "<<flag2<<endl;
		
		if(flag1 && flag2)
		{
			cout<<"Last accepted Token : "<<token<<endl;
			
			//If the received frame number is the expected one(helps in checking if the first Frame was dropped or not)
			if(received_int_1[0] == (token+1)%4)
			{
				if(!s1 && !s2)//No Error present in both the frames
				{
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+3)%4],sizeof(dack[(token+3)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+3)%4], sizeof( ack[(token+3)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+3)%4<<" for packet : "<<packetCount+3<<"\nsent : "<<temp<<" bytes."<<endl;
											
					}
					else
						cout<<"ACK"<<(token+3)%4<<" Dropped for packet : "<<packetCount+3<<endl;
					
					window[(win+1)%2] = received_int_1[0];
					window[(win+2)%2] = received_int_2[0];
					
					win = (win+2)%2;
					
					token = (token+2)%4;
					
					packetCount += 2;
					
					for(int i=0;i<16;i++)
								f<<received_int_1[i+2]<<" ";
									
					for(int i=0;i<16;i++)
								f<<received_int_2[i+2]<<" ";		
				}
				else if(s1 && !s2)//Only First Frame is in error
				{
					
					bool chk = false;
					
					int nackToken = received_int_1[0];
					
					memset(received_int_1,0,34*4);
					
					while(!chk)
					{
						chk = Receive(nackToken,packetCount,received_int_1,clientSocketId);
					}
	
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+3<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+3)%4],sizeof(dack[(token+3)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+3)%4], sizeof( ack[(token+3)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+3)%4<<" for packet : "<<packetCount+3<<"\nsent : "<<temp<<" bytes."<<endl;
					}
					else
						cout<<"ACK"<<(token+3)%4<<" Dropped for packet : "<<packetCount+3<<endl;

					
					token = (token+2)%4;
					
					packetCount += 2;
					
					window[(win+1)%2] = received_int_1[0];
					window[(win+2)%2] = received_int_2[0];
					
					for(int i=0;i<16;i++)
						f<<received_int_1[i+2]<<" ";
									
					for(int i=0;i<16;i++)
						f<<received_int_2[i+2]<<" ";		

					
				}
				else if(!s1 && s2)//Only Second Frame is in error
				{
					
					bool chk = false;
					
					int nackToken = received_int_2[0];
					
					memset(received_int_2,0,34*4);
					
					while(!chk)
					{
						chk = Receive(nackToken,packetCount+1,received_int_2,clientSocketId);
					}	
					
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+3<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+3)%4],sizeof(dack[(token+3)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+3)%4], sizeof( ack[(token+3)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+3)%4<<" for packet : "<<packetCount+3<<"\nsent : "<<temp<<" bytes."<<endl;
					}
					else
						cout<<"ACK"<<(token+3)%4<<" Dropped for packet : "<<packetCount+3<<endl;
					
					token = (token+2)%4;
					
					packetCount += 2;
					
					window[(win+1)%2] = received_int_1[0];
					window[(win+2)%2] = received_int_2[0];
					
					win = (win+2)%2;
					
					for(int i=0;i<16;i++)
						f<<received_int_1[i+2]<<" ";
									
					for(int i=0;i<16;i++)
						f<<received_int_2[i+2]<<" ";
				}
				else if(s1 && s2)//Both the Frames are in Error
				{
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+1)%4],sizeof(dack[(token+1)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+1)%4], sizeof( ack[(token+1)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+1)%4<<" for packet : "<<packetCount+1<<" & "<<packetCount+2<<"\nsent : "<<temp<<" bytes."<<endl;
					}
					else
						cout<<"ACK"<<(token+1)%4<<" Dropped for packet "<<packetCount+1<<endl;
				}
			}
			else if(received_int_1[0] == (token-1)%4 || received_int_1[0] == token)//If the received frames are the same currently present in the window.
			{	
				if(!drop)
				{
					if(delay)
					{
						cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
	
						usleep(3150000);
						
						temp = send( clientSocketId,sdack[(token+1)%4],sizeof(sdack[(token+1)%4]), 0);
													
					}
					else
					{
						temp = send(clientSocketId, sack[(token+1)%4], sizeof( sack[(token+1)%4] ), 0);
					}
	
					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent ACK"<<(token+1)%4<<" for packet : "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
										
				}
				else
					cout<<"ACK"<<(token+1)%4<<" Dropped for packet : "<<packetCount+1<<endl;
			}
			else
				cout<<"Something new happened."<<endl;
		}
		else if(flag1)
		{
			cout<<"Last accepted Token : "<<token<<endl;
			
			//If the received frame is one of the expected one. 
			if(received_int_1[0]==(token+1)%4 || received_int_1[0]==(token+2)%4)
			{
				if(!s1)
				{
					bool chk = false;
					int nackToken;
						
					//If the first frame received is the second frame sent by the sender
					//First frame may have been dropped
					if(received_int_1[1])
					{
						nackToken = received_int_1[0];//Second frame Number
						nackToken = (nackToken-1)%4;//First Frame Number
						
						copy(received_int_1,received_int_1+34,received_int_2);//Copying data from window 1 to window 2 as it's the second frame which is received.
						
						memset(received_int_1,0,34*4);
						
						while(!chk)
						{
							chk = Receive(nackToken,packetCount,received_int_1,clientSocketId);
						}	
					}
					//If the first frame received is the First frame sent by the sender
					//Second frame may have been dropped
					else
					{
						nackToken = received_int_1[0];//First frame Number
						nackToken = (nackToken+1)%4;//Second Frame Number
						
						memset(received_int_2,0,34*4);
						
						while(!chk)
						{
							chk = Receive(nackToken,packetCount+1,received_int_2,clientSocketId);
						}
					}
					
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+3<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+3)%4],sizeof(dack[(token+3)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+3)%4], sizeof( ack[(token+3)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+3)%4<<" for packet : "<<packetCount+3<<"\nsent : "<<temp<<" bytes."<<endl;
					}
					else
						cout<<"ACK"<<(token+3)%4<<" Dropped for packet : "<<packetCount+3<<endl;
					
					token = (token+2)%4;
					
					packetCount += 2;
					
					window[(win+1)%2] = received_int_1[0];
					window[(win+2)%2] = received_int_2[0];
					
					win = (win+2)%2;
					
					for(int i=0;i<16;i++)
						f<<received_int_1[i+2]<<" ";
									
					for(int i=0;i<16;i++)
						f<<received_int_2[i+2]<<" ";
				}
				//Received frame( Whichever it may be ) is in error
				//This case is similar to receiving both the frames in error
				else
				{
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+1)%4],sizeof(dack[(token+1)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+1)%4], sizeof( ack[(token+1)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+1)%4<<" for packet : "<<packetCount+1<<" & "<<packetCount+2<<"\nsent : "<<temp<<" bytes."<<endl;
					}
				}
			}
			//If the received frame is one of the same frame currently present in the window.
			else
			{
				if(received_int_1[0]==(token-1)%4 || received_int_1[0]==token)
				{
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,sdack[(token+1)%4],sizeof(sdack[(token+1)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, sack[(token+1)%4], sizeof( sack[(token+1)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+1)%4<<" for packet : "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
											
					}
					else
						cout<<"ACK"<<(token+1)%4<<" Dropped for packet : "<<packetCount+1<<endl;
				}
				else
					cout<<"Something new has happened again."<<endl;
			}
			
		}
		else if(flag2)
		{
			cout<<"Last accepted Token : "<<token<<endl;
			
			//If the received frame is one of the expected one. 
			if(received_int_2[0]==(token+1)%4 || received_int_2[0]==(token+2)%4)
			{
				if(!s2)
				{
					bool chk = false;
					int nackToken;
						
					//If the Second frame received is the second frame sent by the sender
					//First frame may have been dropped
					if(received_int_2[1])
					{
						nackToken = received_int_2[0];//Second frame Number
						nackToken = (nackToken-1)%4;//First Frame Number

						memset(received_int_1,0,34*4);
						
						while(!chk)
						{
							chk = Receive(nackToken,packetCount,received_int_1,clientSocketId);
						}	
					}
					//If the Second frame received is the First frame sent by the sender
					//Second frame may have been dropped
					else
					{
						nackToken = received_int_2[0];//First frame Number
						nackToken = (nackToken+1)%4;//Second Frame Number
						
						//received_int_1 = received_int_2;
						copy(received_int_2,received_int_2+34,received_int_1);
						
						memset(received_int_2,0,34*4);
						
						while(!chk)
						{
							chk = Receive(nackToken,packetCount+1,received_int_2,clientSocketId);
						}
					}
					
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+3<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+3)%4],sizeof(dack[(token+3)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+3)%4], sizeof( ack[(token+3)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+3)%4<<" for packet : "<<packetCount+3<<"\nsent : "<<temp<<" bytes."<<endl;
					}
					else
						cout<<"ACK"<<(token+3)%4<<" Dropped for packet : "<<packetCount+3<<endl;
					
					token = (token+2)%4;
					
					packetCount += 2;
					
					window[(win+1)%2] = received_int_1[0];
					window[(win+2)%2] = received_int_2[0];
					
					win = (win+2)%2;
					
					for(int i=0;i<16;i++)
						f<<received_int_1[i+2]<<" ";
									
					for(int i=0;i<16;i++)
						f<<received_int_2[i+2]<<" ";
				}
				//Received frame( Whichever it may be ) is in error
				//This case is similar to receiving both the frames in error
				else
				{
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,dack[(token+1)%4],sizeof(dack[(token+1)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, ack[(token+1)%4], sizeof( ack[(token+1)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+1)%4<<" for packet : "<<packetCount+1<<" & "<<packetCount+2<<"\nsent : "<<temp<<" bytes."<<endl;
					}
				}
			}
			//If the received frame is one of the same frame currently present in the window.
			else
			{
				if(received_int_2[0]==(token-1)%4 || received_int_2[0]==token)
				{
					if(!drop)
					{
						if(delay)
						{
							cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
		
							usleep(3150000);
							
							temp = send( clientSocketId,sdack[(token+1)%4],sizeof(sdack[(token+1)%4]), 0);
														
						}
						else
						{
							temp = send(clientSocketId, sack[(token+1)%4], sizeof( sack[(token+1)%4] ), 0);
						}
		
						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent ACK"<<(token+1)%4<<" for packet : "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
											
					}
					else
						cout<<"ACK"<<(token+1)%4<<" Dropped for packet : "<<packetCount+1<<endl;
				}
				else
					cout<<"I don't know what's going on."<<endl;
			}
			
		}
		
		//cout<<endl<<endl<<endl;
	}



	
	char recAck[] = "Received Message";
	temp = send(clientSocketId, recAck, sizeof(recAck), 0);
	
	if(temp == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout<<"Sent Full Message Acknoledgement."<<endl;

	close(clientSocketId);
	
	return 0;
	
}
