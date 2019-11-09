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
	for(int j=1;j<33;j++)
		cout<<a[j]<<" ";
	
	cout<<endl;
	
	copy(a+1,a+18,temp);
	
	while(len < 32)
	{
		
		if(temp[0] == 1)
		{
			ArrayXOR(temp,divisor,0,17);
			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = a[len+1];			
		}	
		else
		{			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = a[len+1];
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
			
		
		temp = read(clientSocketId, a, 33*4);
		
		cout<<"Received frame token No. :- "<<a[0]<<"   For Packet :- "<<p+1<<endl;
				
		if(temp == -1)
		{
			cout<<"Data could not be received :"<<endl;
			perror("The following error occurred");	
		}
		else if(temp == 0)
			cout<<"Connection closed by server."<<endl;
			
		int remainder[16]={0};

		CRC16(a, remainder);

		int s1 = sum(remainder, 16);
		
		if(s1 == 0)
			break;	
	
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

	const char *ack[] = {"ACK0","ACK1","ACK2"};
	const char *sack[] = {"ACK0S","ACK1S","ACK2S"};
	const char *sdack[] = {"ACK0SD","ACK1SD","ACK2SD"};
	const char *dack[] = {"ACK0D","ACK1D","ACK2D"};
	const char *nack[] = {"NACK0","NACK1","NACK2"};
	const char *snack[] = {"NACK0S","NACK1S","NACK2S"};
	const char *sdnack[] = {"NACK0SD","NACK1SD","NACK2SD"};
	const char *dnack[] = {"NACK0D","NACK1D","NACK2D"};

	int received_int_1[33]={0};//First Frame
	int received_int_2[33]={0};//Second Frame

	while(packetCount < 4)
	{
		//Receive the First Frame
		temp = read(clientSocketId, received_int_1, sizeof(received_int_1));
		
		cout<<"(First Frame)Received frame token No. :- "<<received_int_1[0]<<"   For Packet :- "<<packetCount+1<<endl;
				
		if(temp == -1)
		{
			cout<<"Data could not be received :"<<endl;
			perror("The following error occurred");	
		}
		else if(temp == 0)
			cout<<"Connection closed by server."<<endl;
			
		int remainder[16]={0};

		CRC16(received_int_1, remainder);

		int s1 = sum(remainder, 16);	
			
			
			
			
		//Receive the Second Frame	
		temp = read(clientSocketId, received_int_2, sizeof(received_int_2));
		
		cout<<"(Second Frame)Received frame token No. :- "<<received_int_2[0]<<"   For Packet :- "<<packetCount+2<<endl;
				
		if(temp == -1)
		{
			cout<<"Data could not be received :"<<endl;
			perror("The following error occurred");	
		}
		else if(temp == 0)
			cout<<"Connection closed by server."<<endl;	

		memset(remainder,0,16*4);
		
		CRC16(received_int_2, remainder);

		int s2 = sum(remainder, 16);
		
		
		
		int delay = RandomNumberGenerator(2);
		int drop = RandomNumberGenerator(2);
		
		
		
		//If the received frame number is not the same present in the window
		if(received_int_1[0] != token )
		{
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
						
				/*	window[0] = received_int_1[0];
					window[1] = received_int_2[0];	*/
					
					window[(win+1)%2] = received_int_1[0];
					window[(win+2)%2] = received_int_2[0];
					
					win = (win+2)%2;
					
					token = (token+2)%4;
					
					packetCount += 2;
					
					for(int i=0;i<16;i++)
								f<<received_int_1[i+1]<<" ";
									
					for(int i=0;i<16;i++)
								f<<received_int_2[i+1]<<" ";		
				}
				else if(s1 && !s2)//Only First Frame is in error
				{
					//window[1] = received_int_2[0];//assigning window TWO with second frame
					
					bool chk = false;
					
					int nackToken = received_int_1[0];
					
					memset(received_int_1,0,33*4);
					
					while(!chk)
					{
						chk = Receive(nackToken,packetCount,received_int_1,clientSocketId);
					}
					
					//window[0] = received_int_1[0];//assigning window ONE with first frame	
	
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
						f<<received_int_1[i+1]<<" ";
									
					for(int i=0;i<16;i++)
						f<<received_int_2[i+1]<<" ";		

					
				}
				else if(!s1 && s2)//Only Second Frame is in error
				{
					//window[0] = received_int_1[0];//assigning window ONE with first frame
					
					bool chk = false;
					
					int nackToken = received_int_2[0];
					
					memset(received_int_2,0,33*4);
					
					while(!chk)
					{
						chk = Receive(nackToken,packetCount+1,received_int_2,clientSocketId);
					}
					
					//window[1] = received_int_2[0];//assigning window TWO with second frame	
					
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
						f<<received_int_1[i+1]<<" ";
									
					for(int i=0;i<16;i++)
						f<<received_int_2[i+1]<<" ";
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
				}
			}
			else//If the received frame(first) number is not the expected one
			{
				//If the Second frame is the expected one
/*				if(received_int_2[0] == (token+1)%4)
				{
					if(!s2)//second frame has error
					{
						bool chk = false;
					
						int nackToken = received_int_2[0];
					
						memset(received_int_2,0,33*4);
					
						while(!chk)
						{
							chk = Receive(nackToken,packetCount+1,received_int_2,clientSocketId);
						}
						
						if(!drop)
						{
							if(delay)
							{
								cout<<"ACK Delayed for packet : "<<packetCount+2<<endl;
		
								usleep(3150000);
							
								temp = send( clientSocketId,dack[(token+2)%4],sizeof(dack[(token+2)%4]), 0);
														
							}
							else
							{
								temp = send(clientSocketId, ack[(token+2)%4], sizeof( ack[(token+2)%4] ), 0);
							}
		
							if(temp == -1)
							{
								perror("The following error occurred");	
								return -1;
							}
							else
								cout<<"Sent ACK"<<(token+2)%4<<" for packet : "<<packetCount+2<<"\nsent : "<<temp<<" bytes."<<endl;
						}
						else
							cout<<"ACK"<<(token+2)%4<<" Dropped for packet : "<<packetCount+2<<endl;	
					}
					else//second frame has no error
					{
					
					}
				}
				else//If even the second frame is not the expected one
				{
				
				}			
*/			
				cout<<"You Just got Fucked."<<endl;
			}
		}
		else//If the received frame number is same as the one present in the window
		{
/*			if(received_int_2[0] == (token+1)%4)//Checking if the Second frame is the expected one
			{
				window[0] = received_int_2[0];
			}
*/
			cout<<"You just Got Fucked Harder."<<endl;
		}
		
				
		
		if(!s1 && !s2)//No Error present in both the frames
		{
		
		}
		else if(s1 && !s2)//Only First Frame is in error
		{
		
		}
		else if(!s1 && s2)//Only Second Frame is in error
		{
		
		}
		else if(s1 && s2)//Both the Frames are in Error
		{
		
		}	
			


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		int delay = RandomNumberGenerator(2);
		int drop = RandomNumberGenerator(2);

		//If the received frame number is not the same present in the window 
		if(received_int[0] != token )
		{
			//If the received frame number is the expected one(helps in checking if the first Frame was dropped or not)
			if(received_int[0] == (token+1)%4)
			{
				int remainder[16]={0};

				CRC16(received_int, remainder);
		
				int s1 = sum(remainder, 16);
				
				//First Frame
				//No error present in the received message
				//Checks the second frame only if the first frame is free of error and of the correct order
				if(s == 0)
				{
					packetCount++;
					
					//Updates the window with the last accepted frame number
					token = received_int[0];
				
					//OUTPUT THE RECEIVED INTEGER ARRAY INTO A FILE
					for(int i=0;i<16;i++)
							f<<received_int[i+1]<<" ";
		
					memset(received_int,0,33*4);
					
					if(packetCount >= 4)
						break;
					
					//Receive the Second Frame
					temp = read(clientSocketId, received_int, sizeof(received_int));
		
					cout<<"(Second Frame)Received frame token No. :- "<<received_int[0]<<"   For Packet :- "<<packetCount+1<<endl;
		
					if(temp == -1)
					{
						cout<<"Data could not be received :"<<endl;
						perror("The following error occurred");	
					}
					else if(temp == 0)
						cout<<"Connection closed by server."<<endl;
						
					//(Second Frame)If the received frame number is not the same present in the window	
					if(received_int[0] != token )
					{
						//(Second Frame)If the received frame number is the expected one
						if(received_int[0] == (token+1)%3)
						{
							memset(remainder,0,16*4);

							CRC16(received_int, remainder);
		
							s = sum(remainder, 16);
					
							//Second Frame
							//No error present in the message
							if(s == 0)
							{
								packetCount++;
					
								//OUTPUT THE RECEIVED INTEGER ARRAY INTO A FILE
								for(int i=0;i<16;i++)
									f<<received_int[i+1]<<" ";
					
								//Updates the window with the last accepted frame number	
								token = received_int[0];	

								if(!drop)
								{
									if(delay)
									{
										cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
					
										usleep(3250000);
										
										temp = send( clientSocketId,dack[(token+1)%3],sizeof(dack[(token+1)%3]), 0);
																	
									}
									else
									{
										temp = send(clientSocketId, ack[(token+1)%3], sizeof( ack[(token+1)%3] ), 0);
									}
					
									if(temp == -1)
									{
										perror("The following error occurred");	
										return -1;
									}
									else
										cout<<"Sent ACK"<<(token+1)%3<<" for packet : "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
								}		

							}
							else//If their is an error in the second frame
							{
								if(!drop)
								{
									if(delay)
									{
										cout<<"NACK Delayed for packet : "<<packetCount+1<<endl;
					
										usleep(3150000);
						
										temp = send(clientSocketId, dnack[received_int[0]], sizeof( dnack[received_int[0]] ), 0);
									}
									else
										temp = send(clientSocketId, nack[received_int[0]], sizeof( nack[received_int[0]] ), 0);

									if(temp == -1)
									{
										perror("The following error occurred");	
										return -1;
									}
									else
										cout<<"Sent NACK"<<received_int[0]<<" for packet "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
								}
								else
									cout<<"NACK"<<received_int[0]<<" Dropped for packet : "<<packetCount+1<<endl;
							}
						}
						else//(Second Frame)if the received frame number is not the expected one
						{
							if(!drop)
							{
								if(delay)
								{
									cout<<"NACK"<<(token+1)%3<<" Delayed for packet : "<<packetCount+1<<endl;
					
									usleep(3150000);
						
									temp = send(clientSocketId, dnack[(token+1)%3], sizeof( dnack[(token+1)%3] ), 0);
								}
								else
									temp = send(clientSocketId, nack[(token+1)%3], sizeof( nack[(token+1)%3] ), 0);

								if(temp == -1)
								{
									perror("The following error occurred");	
									return -1;
								}
								else
									cout<<"Sent NACK"<<(token+1)%3<<" for packet "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
							}
							else
								cout<<"NACK"<<(token+1)%3<<" Dropped for packet : "<<packetCount+1<<endl;
						}
					}
					else//(Second Frame)if the received frame number is same as the one present in the window
					{
						if(!drop)
						{
							if(delay)
							{
								cout<<"NACK Delayed for packet : "<<packetCount+1<<endl;
				
								usleep(3150000);
				
								temp = send(clientSocketId, sdnack[(token+1)%3], sizeof( sdnack[(token+1)%3] ), 0);
					
							}
							else
							{
								temp = send(clientSocketId, snack[(token+1)%3], sizeof( snack[(token+1)%3] ), 0);
							}
				
				
							if(temp == -1)
							{
								perror("The following error occurred");	
								return -1;
							}
							else if(temp > 0)
							{
								cout<<"(Received same frame)Sent NACK"<<(token+1)%3<<" for packet : "<<packetCount+1<<"\nsent : "
									<<temp<<" bytes."<<endl;
							}
						}
						else
							cout<<"NACK"<<(token+1)%3<<" Dropped for packet : "<<packetCount+1<<endl;
					}	

				}
				else//If their is an error in the first frame
				{
					if(!drop)
					{
						if(delay)
						{
							cout<<"NACK Delayed for packet : "<<packetCount+1<<endl;
					
							usleep(3150000);
						
							temp = send(clientSocketId, dnack[received_int[0]], sizeof( dnack[received_int[0]] ), 0);
						}
						else
							temp = send(clientSocketId, nack[received_int[0]], sizeof( nack[received_int[0]] ), 0);

						if(temp == -1)
						{
							perror("The following error occurred");	
							return -1;
						}
						else
							cout<<"Sent NACK"<<received_int[0]<<" for packet "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
					}
					else
						cout<<"NACK"<<received_int[0]<<" Dropped for packet : "<<packetCount+1<<endl;	
				}
			
			}
			else//If the received frame number is not the expected one
			{	
				if(!drop)
				{
					if(delay)
					{
						cout<<"NACK"<<(token+1)%3<<" Delayed for packet : "<<packetCount+1<<endl;
					
						usleep(3150000);
						
						temp = send(clientSocketId, dnack[(token+1)%3], sizeof( dnack[(token+1)%3] ), 0);
					}
					else
						temp = send(clientSocketId, nack[(token+1)%3], sizeof( nack[(token+1)%3] ), 0);

					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent NACK"<<(token+1)%3<<" for packet "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
				}
				else
					cout<<"NACK"<<(token+1)%3<<" Dropped for packet : "<<packetCount+1<<endl;	
			}

		}
		else//If the received frame number is same as the one present in the window
		{
			if(!drop)
			{
				if(delay)
				{
					/*cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
				
					usleep(3150000);
				
					temp = send(clientSocketId, sdack[(token+1)%3], sizeof( sdack[(token+1)%3] ), 0);*/
					
					cout<<"NACK Delayed for packet : "<<packetCount+1<<endl;
				
					usleep(3150000);
				
					temp = send(clientSocketId, sdnack[(token+1)%3], sizeof( sdnack[(token+1)%3] ), 0);
					
				}
				else
				{
					//temp = send(clientSocketId, sack[(token+1)%3], sizeof( sack[(token+1)%3] ), 0);
					temp = send(clientSocketId, snack[(token+1)%3], sizeof( snack[(token+1)%3] ), 0);
				}
				
				
				if(temp == -1)
				{
					perror("The following error occurred");	
					return -1;
				}
				else if(temp > 0)
				{
					cout<<"(Received same frame)Sent NACK"<<(token+1)%3<<" for packet : "<<packetCount+1<<"\nsent : "
						<<temp<<" bytes."<<endl;
				}
			}
			else
				cout<<"NACK"<<(token+1)%3<<" Dropped for packet : "<<packetCount+1<<endl;
		}
		


		cout<<endl<<endl<<endl;
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
