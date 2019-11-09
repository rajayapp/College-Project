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


/*
void ArrayXOR(int *a, int *b)
{
	for(int i=0;i<17;i++)
		a[i] = a[i] ^ b[i];
}*/
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
	
/*	cout<<"Temp Original."<<endl;
	for(int j=0;j<17;j++)
		cout<<temp[j]<<" ";
*/	
//	cout<<endl;
	
	while(len < 32)
	{
		
		if(temp[0] == 1)
		{
			ArrayXOR(temp,divisor,0,17);
			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = a[len+1];
			
/*			cout<<"next bit :- "<<a[len+1]<<endl;	
			
			cout<<"temp loop "<<len-16<<" :- "<<endl;
			
			for(int i=0;i<17;i++)
				cout<<temp[i]<<(i!=16?" ":"\n");
*/			
		}	
		else
		{
//			int t[27] = {0};
			
//			ArrayXOR(temp,t);
			
			rotate(temp,temp+1,temp+17);//Algorithm header
				
			temp[16] = a[len+1];
			
/*			cout<<"temp loop "<<len-16<<" :- "<<endl;
			
			for(int i=0;i<17;i++)
				cout<<temp[i]<<(i!=16?" ":"\n");
*/		}
		
		len += 1;
		
	}
	
/*	cout<<"temp after loop :- "<<endl;
	for(int i=0;i<17;i++)
		cout<<temp[i]<<(i!=16?" ":"\n");
*/
	if(temp[0] == 1)
	{
		ArrayXOR(temp,divisor,0,17);
		
		copy(temp+1,temp+17,remainder);
	}	
	else
	{
		copy(temp+1,temp+17,remainder);
	}
	
//	ArrayXOR(dividend+17,remainder,0,16);

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
	
//############################################################################################################################

	int token = 1;//Current frame number in receiver window 
	ofstream f("output.txt");
	int packetCount = 0;
	char ack0[] = "ACK0";
	char ack1[] = "ACK1";
	char sack0[] = "ACK0S";
	char sack1[] = "ACK1S";
	char sdack0[] = "ACK0SD";
	char sdack1[] = "ACK1SD";
	char dack0[] = "ACK0D";
	char dack1[] = "ACK1D";
	char nack[] = "NACK";
	char dnack[] = "NACKD";

	while(packetCount < 3)//8)
	{
		int received_int[33];

		temp = read(clientSocketId, received_int, sizeof(received_int));
		
		cout<<"Received frame token No. :- "<<received_int[0]<<endl;
		
/*		cout<<"Received Message :-"<<endl;
		for(int i=0;i<33;i++)
			cout<<received_int[i]<<(i!=32)?" ":"\n";
*/		
		if(temp == -1)
		{
			cout<<"Data could not be received :"<<endl;
			perror("The following error occurred");	
		}
		else if(temp == 0)
			cout<<"Connection closed by server."<<endl;

		int delay = RandomNumberGenerator(2);
		int drop = RandomNumberGenerator(2);

		if(received_int[0] != token )
		{
			int remainder[16]={0};

			CRC16(received_int, remainder);
		
			int s = sum(remainder, 16);
			
			cout<<"Sum is : "<<s<<endl;

			if(s)//s != 0 :- Presence of error
			{
				cout<<"Packet : "<<packetCount+1<<" has error"<<endl;
			
				if(!drop)
				{
					if(delay)
					{
						cout<<"NACK Delayed for packet : "<<packetCount+1<<endl;
					
						usleep(3150000);
						
						temp = send(clientSocketId, dnack, sizeof(dnack), 0);
					}
					else
						temp = send(clientSocketId, nack, sizeof(nack), 0);

					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent NACK for packet "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
				}
				else
					cout<<"NACK Dropped for packet : "<<packetCount+1<<endl;
			}
			else//s == 0 :- No error
			{
				token = received_int[0];	

				if(!drop)
				{
					if(delay)
					{
						cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
					
						usleep(3250000);
					
						if(token)
							temp = send(clientSocketId, dack0, sizeof(dack0), 0);
						else
							temp = send(clientSocketId, dack1, sizeof(dack1), 0);

					}
					else
					{
						if(token)
							temp = send(clientSocketId, ack0, sizeof(ack0), 0);
						else
							temp = send(clientSocketId, ack1, sizeof(ack1), 0);

					}
					
					if(temp == -1)
					{
						perror("The following error occurred");	
						return -1;
					}
					else
						cout<<"Sent ACK"<<!token<<" for packet : "<<packetCount+2<<"\nsent : "<<temp<<" bytes."<<endl;
						
					cout<<"Received message"<<endl;
					for(int i=0;i<16;i++)						
						cout<<received_int[i+1]<<((i!=15)?" ":"\n");	
				}
				else
					cout<<"ACK Dropped for packet : "<<packetCount+1<<endl;

				for(int i=0;i<16;i++)
					f<<received_int[i+1]<<" ";
			
				packetCount++;
			}
		}
		else
		{
			if(!drop)
			{
				if(delay)
				{
					cout<<"ACK Delayed for packet : "<<packetCount+1<<endl;
				
					usleep(3250000);
				
					if(token)
						temp = send(clientSocketId, sdack0, sizeof(sdack0), 0);
					else
						temp = send(clientSocketId, sdack1, sizeof(sdack1), 0);

				}
				else
				{
					if(token)
						temp = send(clientSocketId, sack0, sizeof(sack0), 0);
					else
						temp = send(clientSocketId, sack1, sizeof(sack1), 0);

				}
				
				
				if(temp == -1)
				{
					perror("The following error occurred");	
					return -1;
				}
				else if(temp > 0)
					cout<<"(Received same frame)Sent ACK"<<!token<<" for packet : "<<packetCount+1<<"\nsent : "<<temp<<" bytes."<<endl;
			}
			else
				cout<<"ACK Dropped for packet : "<<packetCount+1<<endl;
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
