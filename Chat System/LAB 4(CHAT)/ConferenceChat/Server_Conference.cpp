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
	

	int num = 4;
	
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
			
		connections.push_back(dummy);
		
		if(connections.size() == num)
			break;
	}
	
	
	
	cout<<"Enter the subset of connections."<<endl;
	
	string input;
//	cin.ignore();
//	cin.ignore();
	getline(cin, input);
	cout<<input<<endl;
	istringstream iss(input);
	int a;
	vector<int>subset;
	
	while(iss >> a)
	{
		subset.push_back(a);
	}
	
//	cout<<subset.size()<<endl;
	
	cout<<"Printing subsets:-"<<endl;
	for(vector<int>::iterator it = subset.begin(); it != subset.end(); it++)
		cout<<*it<<endl;
		
//	cout<<"checking if master is working"<<endl;	
	for(vector<int>::iterator it = subset.begin(); it != subset.end(); it++)
	{
		FD_SET(connections[(*it)-1].clientSocketId, &master);
			
//		cout<< connections[(*it)-1].clientSocketId<<endl;
			
		if(connections[(*it)-1].clientSocketId > fdmax)
			fdmax = connections[(*it)-1].clientSocketId;
	}
	
/*	cout<<"rechecking values of master"<<endl;
	for(vector<struct node>::iterator it = connections.begin(); it != connections.end(); it++)
		if(FD_ISSET((*it).clientSocketId, &master))
			cout<< (*it).clientSocketId<<endl;
*/
//cout<<"I am here"<<endl;

	struct timeval tv;
	
	tv.tv_sec = 1;//2;
	tv.tv_usec = 1000000;
	
	int bytesSent;
	char receivedBuff[100];


	//Send data to the connected Client
	
	while(true)
	{
		read_fds = master;
		
		int numFds = select(fdmax+1, &read_fds, NULL, NULL, &tv);	
	
		if(numFds == -1)
		{
			perror("Select() ");	
			return -1;
		}
		else if(numFds > 0)//at least one fd is ready
		{
			for(vector<struct node>::iterator it = connections.begin(); it != connections.end(); it++)//looping over all the fd's
			{
				if(FD_ISSET((*it).clientSocketId, &read_fds))//checking if the fd is ready or not
				{
					memset(receivedBuff, '\0', 100);//clear the buffer
			
					temp = recv((*it).clientSocketId, receivedBuff, sizeof(receivedBuff), 0);//receiving message

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
					
					for(vector<struct node>::iterator it1 = connections.begin(); it1 != connections.end(); it1++)
					{
						if(FD_ISSET((*it1).clientSocketId, &master) && it1 != it)//checking if the fd is involved in conference chat
						{
							bytesSent = send((*it1).clientSocketId, receivedBuff, strlen(receivedBuff)+1, 0);
		
							if(bytesSent == -1)
							{
								perror("send ");	
								return -1;
							}
						}
					}
				}
			}
		}
		
	}
	

	//Closing all the sockets
	close(serverSocketId);
	for(int i=0;i<num;i++)
	{
		close(connections[i].clientSocketId);
	}


	return 0;
}
