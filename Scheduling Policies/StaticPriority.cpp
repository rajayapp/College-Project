#include<iostream>
#include<fstream>
#include<vector>



using namespace std;



struct node{
	int pid,arrivalTime,serviceTime,finishTime,waitTime,priority,remainingTime;
};



void WaitCounter(vector<struct node> *temp, int limit, int ex)//Increments waiting time of processes after each count of time
{
	vector<struct node>::iterator it;
	
	for(it=temp.begin(); (*it).arrivalTime < limit && it != temp.end(); it++)
		if((*it).remainingTime != 0 && (*it).pid != ex)
			(*it).waitTime += 1;
}



bool check(vector<struct node> *temp)//Checks whether every process has been completely executed or not
{
	vector<struct node>::iterator it;
	
	for(it = temp.begin(); it != temp.end(); it++)
		if( (*it).remainingTime != 0 )
			return true;
			
	return false;
}



bool compare(struct node a, struct node b)//compare function for sort
{
	if()
}



int main(int argc, char **argv)
{

fstream f;
f.open(argv[2],ios::in);

int process = atoi(argv[1]);

vector<struct node> Ftemp;
struct node temp;
int a,b,c;

for(int i=0;i<process;i++)
{
	f>>a>>b>>c

	temp.pid = i+1;
	temp.arrivalTime = a;
	temp.serviceTime = b;
	temp.priority = c;
	temp.finishTime = 0;
	temp.waitTime = 0;
	temp.remainingTime = b;
	
	Ftemp.push_back(temp);
}

//Sort the Vector HERE

int currentTime = 0,min;
vector<struct node>::iterator it,pos;

while(check(&Ftemp))
{
	min = 99999999;

	for(it=Ftemp.begin(); (*it).arrivalTime<=currentTime && it!=Ftemp.end(); it++)
	{
		if( (*it).priority < min && (*it).remainingTime != 0)
		{
			min = (*it).priority;
			pos = it;
		}
		
	}
	
	(*pos).remainingTime -= 1;
	currentTime += 1;
	WaitCounter(&Ftemp, currentTime,(*pos).pid);

	if((*pos).remainingTime == 0)
		(*pos).finishTime = currentTime;
}

//Output Goes HERE

return 0;
}
