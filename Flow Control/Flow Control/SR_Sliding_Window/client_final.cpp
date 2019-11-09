#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <cstdio>
#include <cstring>



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



int * CRC16(int *a, int *dividend)
{
	int divisor[17]={1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1};
	int remainder[16]={0};
	int temp[17]={0};
	int len = 17;
	
	memset(dividend,0,33*4);

	copy(a,a+16,dividend+1);
		
	copy(dividend+1,dividend+18,temp);
		
	while(len < 32)
	{
		
		if(temp[0] == 1)
		{
			ArrayXOR(temp,divisor,0,17);
			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = dividend[len+1];	
			
		}	
		else
		{			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = dividend[len+1];
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
	
	ArrayXOR(dividend+17,remainder,0,16);

	cout<<"Remainder :-"<<endl;
	for(int j=0;j<16;j++)
		cout<<remainder[j]<<" ";
		
	cout<<endl;	

	return dividend;

}



void IntroduceError(int *a)
{
	int i=0;
	
	while(i <5)
	{
		int ind = RandomNumberGenerator(32);

		if(!a[ind+1])
			a[ind+1] = 1;
		else
			a[ind+1] = 0;

		i++;
	}
}



bool Resend(int *a, int clientSocketId, int expToken, int p)//CORRECT PACKET VALUE
{
	int temp;

	while(true)
	{
		int drop = RandomNumberGenerator(2);
		
		if(drop)
				cout<<"Packet No."<<p+1<<" Dropped."<<endl;
	
		if(!drop)
		{
			int error = RandomNumberGenerator(2);

			if(error)
			{
				cout<<"Packet No."<<p+1<<" has error."<<endl;
			
				int errArray[33]={0};
				
				copy(a,a+33,errArray);
			
				IntroduceError(errArray);
				
				//Send the Message with error introduced
				temp = send(clientSocketId, errArray, 33*4, 0);

				if(temp == -1)
				{
					perror("The following error occurred");	
					return -1;
				}
				else
					cout<<"Sent Packet : "<<p+1<<" Message with error."<<endl;
			}
			else
			{
				//Send the Message without error
				temp = send(clientSocketId, a, 33*4, 0);

				if(temp == -1)
				{
					perror("The following error occurred");	
					return -1;
				}
				else
					cout<<"Sent Packet : "<<p+1<<" Message without error."<<endl;
			}
		}
		
		
		
		sleep(3);
			
		read_fds = master;
		
		int numFds = select(fdmax+1, &read_fds, NULL, NULL, &tv);	
	
		if(numFds == -1)
		{
			perror("Select() ");	
			return -1;
		}
		else if(numFds > 0)
		{
			memset(receivedBuff, '\0', 100);//clear the buffer
	
			temp = recv(clientSocketId, receivedBuff, sizeof(receivedBuff), 0);//receiving Ack or Nack

			if(temp == -1)
			{
				cout<<"Data could not be received :"<<endl;
				perror("recv ");	
			}
			else if(temp == 0)
			{
				cout<<"Connection closed by remote machine."<<endl;
				break;
			}
			
			cout<<receivedBuff<<endl;
			
			if(receivedBuff[0] == "A")
			{
				if( expToken == atoi(receivedBuff[3]) )
					break;
			}

		}
		else
			cout<<"Time-out Occurred for packet :- "<<p+1<<endl;		
		
		
	}//END WHILE LOOP

	
	return true;
}



int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		cout<<"Usage syntax: ./client <ip_address> <port_no>"<<endl;
		return 0;
	}

	struct sockaddr_in serverSocket;
	int clientSocketId;


	//Create a new Socket
	clientSocketId = socket(PF_INET, SOCK_STREAM, 0);

	if(clientSocketId == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout<<"Socket successfully created."<<endl;


	//Save Server Socket Address Information
	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(atoi(argv[2]));
	serverSocket.sin_addr.s_addr = inet_addr(argv[1]);
	memset(serverSocket.sin_zero,'\0',sizeof(serverSocket.sin_zero));
	
	//Connect To The Destination Server
	int temp = connect(clientSocketId, (struct sockaddr *)&serverSocket, sizeof(struct sockaddr));

	if(temp == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout<<"Connection established with server"<<endl;
	
//############################################################################################################################
	
	//int msg[80]={0};
	int msg[64]={0};
	int numberOfPackets = 4;//128/16;
	int p = 0, seek = 0, token = 0;
	
	//Take 128 bits input from file
	fstream f;
	f.open("input.txt",ios::in);
	
	int index=0,val;
	
	while(f>>val)
		msg[index++] = val;
		
	for(int i=0;i<64;i++)
		cout<<msg[i]<<((i!=63)?" ":"\n");	
	
	int a[16];
	int *crcMsg1,*crcMsg2;
	//int finalMsg[66]={0};
	int drop;//1 : drop & 0 : Don't drop
	int error;//1 : error & 0 : No error
	
	
	int fdmax = 0;
	fd_set master, read_fds;
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	FD_SET(clientSocketId, &master);
	fdmax = clientSocketId;
	
	struct timeval tv;
	
	tv.tv_sec = 0;//3;
	tv.tv_usec = 0;//3000000;
	
	char receivedBuff[100];

	
	srand(time(NULL));
	
	while(p < numberOfPackets)
	{
		cout<<"Packet no. :- "<<p+1<<endl;
		cout<<"Sender window No.(first frame) :- "<<token<<endl;
	
		copy(msg+seek,msg+seek+16,a);
		
		int dividend1[33]={0};
		int dividend2[33]={0};
		
		crcMsg1 = CRC16(a,dividend1);
	
		crcMsg1[0] = token % 4;
	
		seek += 16;
		
		copy(msg+seek,msg+seek+16,a);
		
		crcMsg2 = CRC16(a,dividend2);
		
		crcMsg2[0] = (token+1)%4;
		
		bool next = false;
		
		while(!next)
		{
			/*
			
			Below code :-
							1.If to drop the first frame.
							2.If to introduce error in the first frame.
			
			*/
			drop = RandomNumberGenerator(2);

			if(drop)
				cout<<"Packet No."<<p+1<<" Dropped."<<endl;
	
			if(!drop)
			{
				error = RandomNumberGenerator(2);

				if(error)
				{
					cout<<"Packet No."<<p+1<<" has error."<<endl;
				
					int errArray[33]={0};
					
					copy(crcMsg1,crcMsg1+33,errArray);
				
					IntroduceError(errArray);
					
					//Send the Message with error introduced
					temp = send(clientSocketId, errArray, 33*4, 0);
	
					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent Packet : "<<p+1<<" Message with error."<<endl;
				}
				else
				{
					//Send the Message without error
					temp = send(clientSocketId, crcMsg1, 33*4, 0);
	
					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent Packet : "<<p+1<<" Message without error."<<endl;
				}
			}
			
			
			
			/*
			
			Below code :-
							1.If to drop the second frame.
							2.If to introduce error in the second frame.
			
			*/
			drop = RandomNumberGenerator(2);

			if(drop)
				cout<<"Packet No."<<p+2<<" Dropped."<<endl;
	
			if(!drop)
			{
				error = RandomNumberGenerator(2);

				if(error)
				{
					cout<<"Packet No."<<p+2<<" has error."<<endl;
				
					int errArray[33]={0};
					
					copy(crcMsg2,crcMsg2+33,errArray);
				
					IntroduceError(errArray);
					
					//Send the Message with error introduced
					temp = send(clientSocketId, errArray, 33*4, 0);
	
					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent Packet : "<<p+2<<" Message with error."<<endl;
				}
				else
				{
					//Send the Message without error
					temp = send(clientSocketId, crcMsg2, 33*4, 0);
	
					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent Packet : "<<p+2<<" Message without error."<<endl;
				}
			}
			
			
			//Receive message using select and time it for 3 sec
			sleep(3);
			
			read_fds = master;
			
			int numFds = select(fdmax+1, &read_fds, NULL, NULL, &tv);	
		
			if(numFds == -1)
			{
				perror("Select() ");	
				return -1;
			}
			else if(numFds > 0)
			{
				memset(receivedBuff, '\0', 100);//clear the buffer
		
				temp = recv(clientSocketId, receivedBuff, sizeof(receivedBuff), 0);//receiving Ack or Nack

				if(temp == -1)
				{
					cout<<"Data could not be received :"<<endl;
					perror("recv ");	
				}
				else if(temp == 0)
				{
					cout<<"Connection closed by remote machine."<<endl;
					break;
				}
				
				cout<<receivedBuff<<endl;
				
				/*
				
				ACK :- 1.Both the frames sent are error free 
					   2.Both the frames sent are in error
				*/
				if(receivedBuff[0]=='A') 
				{
					//If Both the Sent Frames are in error
					if( token == atoi(&receivedBuff[3]) )
					{
						seek -= 16;
						
						next = true;
					}
					//If Both the Sent Frames are free of error
					else if( (token+2)%4 == atoi(&receivedBuff[3]) )
					{
						seek += 16;
						
						next = true;
						
						p += 2;
						
						token = (token+2)%4;
					}
					else
						cout<<"Received wrong ACK."<<endl;
				}
				/*
				
				NACK :- 1.If the first frame has error.
						2.If the second frame has error.
						3.If the first or second frames are dropped.
						4.If the expected frame has not been received.
				
				*/
				else
				{
					if( token == atoi(&receivedBuff[4]) )//If the first frame sent is in error
					{						
						while(!next)
						{
							next = Resend(crcMsg1, clientSocketId, (token+2)%4, p);
						}
						
						seek += 16;

						p += 2;
						
						token = (token+2)%4;
					}
					else if( (token+1)%4 == atoi(&receivedBuff[4]) )//If the second Frame sent is in error
					{	
						while(!next)
						{
							next = Resend(crcMsg2, clientSocketId, (token+2)%4, p+1);
						}
						
						seek += 16;

						p += 2;
						
						token = (token+2)%4;
					}
				}

			}
			else
				cout<<"Time-out Occurred for packet :- "<<p+1<<" & "<<p+2<<endl;
			
		}
		
//		if(token)
//			token = 0;
//		else
//			token = 1;

/*		if(token == 0)
			token = 1;
		else
			token = 0;
*/		
//		p++;
//		seek += 16;
		
		cout<<endl<<endl<<endl;
	}
	
	cout<<endl<<endl<<"Message sent successfully."<<endl<<endl;
	
	bool chk = true;
	while(chk)
	{
		memset(receivedBuff, '\0', 100);//clear the buffer
		
		temp = recv(clientSocketId, receivedBuff, sizeof(receivedBuff), 0);//receiving Ack or Nack

		if(temp == -1)
		{
			cout<<"Data could not be received :"<<endl;
			perror("recv ");	
		}
		else if(temp == 0)
		{
			cout<<"Connection closed by remote machine."<<endl;
			return -1;
		}
	
		if(receivedBuff == "Received Message")
		{
			cout<<"Received acknoledgement for the Whole Message."<<endl;
			chk = false;
		}
		else
		{
			cout<<"Did not Receive acknoledgement for the Whole Message."<<endl;
			cout<<"Received :- "<<receivedBuff<<endl;
		}
			
	}
	
		
	
	f.close();

	close(clientSocketId);
	
	return 0;
	
}
