#include <iostream>
#include <vector>
#include <sys/sendfile.h>//sendfile()
#include <sys/stat.h> 
#include <fcntl.h>
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



struct node{
	struct sockaddr_in clientSocket;
	int clientSocketId;
};



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


	int fdmax = 0;
	fd_set master, read_fds;
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	

	int num = 2;
	
	vector<struct node> connections;
	struct node dummy;
	
	socklen_t addrlen = sizeof(struct sockaddr);
	
	while(true)
	{
		dummy.clientSocketId = accept(serverSocketId, (struct sockaddr *)&(dummy.clientSocket), &addrlen);

		if(dummy.clientSocketId == -1)
		{
			perror("The following error occurred");	
			return -1;
		}
		else
		{
			cout<<endl<<endl<<"Connection accepted"<<endl;
			cout<<"Client IP address : "<<inet_ntoa(dummy.clientSocket.sin_addr)<<endl<<endl;
		}
		
		FD_SET(dummy.clientSocketId, &master);
			
		if(dummy.clientSocketId > fdmax)
			fdmax = dummy.clientSocketId;	
			
		connections.push_back(dummy);
		
		if(connections.size() == num)
			break;
	}


	struct timeval tv;
	
	tv.tv_sec = 1;//2;
	tv.tv_usec = 1000000;
	
	int bytesSent;
	char receivedBuff[100];
	
	
	while(true)
	{
		read_fds = master;
		
		int numFds = select(fdmax+1, &read_fds, NULL, NULL, &tv);	
	
		if(numFds == -1)
		{
			perror("Select() ");	
			return -1;
		}
		else if(numFds > 0)
		{
		
			if(FD_ISSET(connections[0].clientSocketId, &read_fds))
			{
				memset(receivedBuff, '\0', 100);
			
				temp = recv(connections[0].clientSocketId, receivedBuff, sizeof(receivedBuff), 0);

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
/*				else
				{
					cout<<"Data successfully received."<<endl;
					cout << "Msg :-  " << receivedBuff << endl;
				}
*/		
		
				bytesSent = send(connections[1].clientSocketId, receivedBuff, strlen(receivedBuff)+1, 0);
		
				if(bytesSent == -1)
				{
					perror("send ");	
					return -1;
				}
//				else
//					cout<<"Sent "<<bytesSent<<" bytes."<<endl;
			}
			
			
			
			if(FD_ISSET(connections[1].clientSocketId, &read_fds))
			{
				memset(receivedBuff, '\0', 100);
			
				temp = recv(connections[1].clientSocketId, receivedBuff, sizeof(receivedBuff), 0);

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
/*				else
				{
					cout<<"Data successfully received."<<endl;
					cout << "Msg :-  " << receivedBuff << endl;
				}
*/		
		
				bytesSent = send(connections[0].clientSocketId, receivedBuff, strlen(receivedBuff)+1, 0);
		
				if(bytesSent == -1)
				{
					perror("send ");	
					return -1;
				}
//				else
//					cout<<"Sent "<<bytesSent<<" bytes."<<endl;
			}
		
		
		}
		
	}
	

	
	close(serverSocketId);

	for(int i=0;i<num;i++)
	{
		close(connections[i].clientSocketId);
	}

	return 0;
}
