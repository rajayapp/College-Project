#include <iostream>
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



using namespace std;



int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		cout<<"Usage syntax: ./client <ip_address> <port_no>"<<endl;
		return 0;
	}

	struct sockaddr_in clientSocket,serverSocket;
	int clientSocketId,serverSocketId;


	//Create a new Socket
	clientSocketId = socket(PF_INET, SOCK_STREAM, 0);

	if(clientSocketId == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout<<"Socket successfully created."<<endl;


	//Save Client Socket Address Information
	clientSocket.sin_family = AF_INET;
	clientSocket.sin_port = htons(atoi(argv[2]));
	clientSocket.sin_addr.s_addr = inet_addr(argv[1]);
	memset(clientSocket.sin_zero,'\0',sizeof(clientSocket.sin_zero));


	//Save Server Socket Address Information
	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(atoi(argv[2]));
	serverSocket.sin_addr.s_addr = inet_addr(argv[1]);
	memset(serverSocket.sin_zero,'\0',sizeof(serverSocket.sin_zero));


	//Connect To The Destination Server
	int temp = connect(clientSocketId, (struct sockaddr *)&serverSocket, sizeof(struct sockaddr));//=====================

	if(temp == -1)
	{
		perror("The following error occurred");	
		return -1;
	}
	else
		cout<<"Connection established with server"<<endl;


	//Send data to the connected Server

	pid_t pid = fork();
	

	if(pid > 0)
	{
		
		while(true)
		{
			char buff[50];
			cout<<"Client 1 : ";
			cin.getline(buff,50);

			int bytesSent;

			bytesSent = send(clientSocketId, buff, strlen(buff)+1, 0);//==========================?Which Socket Descriptor to be used

			if(bytesSent == -1)
			{
				perror("The following error occurred");	
				return -1;
			}
//			else
//				cout<<"Sent "<<bytesSent<<" bytes to the server."<<endl;
				
			
		}
	}
	else
	{
	
	
		while(true)
		{
			//Receive data from the server
			char receivedBuff[50];

			temp = recv(clientSocketId, receivedBuff, sizeof(receivedBuff), 0);

			if(temp == -1)
			{
				cout<<"Data could not be received :"<<endl;
				perror("The following error occurred");
				
				break;	
			}
			else if(temp == 0)
			{
				cout<<"Connection closed by server."<<endl;
				break;
			}
			else
			{
				//cout<<"Data successfully received."<<endl;
				cout <<endl<< "Server : " << receivedBuff << endl;
			}
			
		}
	}
	close(clientSocketId);


	return 0;
}
