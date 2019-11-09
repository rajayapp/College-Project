#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>



using namespace std;



struct node{
	int pid,arrivalTime,serviceTime,finishTime,waitTime,priority,remainingTime,t_wait;
};



void WaitCounter(vector<struct node> *temp, int limit, int ex)//Increments waiting time of processes after each count of time
{
	vector<struct node>::iterator it;
	
	for(it=temp->begin(); (*it).arrivalTime < limit && it != temp->end(); it++)
		if((*it).remainingTime != 0 && (*it).pid != ex)
		{
			(*it).waitTime += 1;
			(*it).t_wait += 1;
		}
}



void TWaitCounter(vector<struct node> *temp, int limit, int ex, int t)//Increments waiting time of processes after each count of time
{
	vector<struct node>::iterator it;
	
	for(it=temp->begin(); (*it).arrivalTime < limit && it != temp->end(); it++)
		if((*it).remainingTime != 0 && (*it).pid != ex && (*it).t_wait==t)
		{
			(*it).t_wait = 0;
			(*it).priority -= 1; 
		}
}



bool check(vector<struct node> *temp)//Checks whether every process has been completely executed or not
{
	vector<struct node>::iterator it;
	
	for(it = temp->begin(); it != temp->end(); it++)
		if( (*it).remainingTime != 0 )
			return true;
			
	return false;
}



//Compare function for sort function
bool compare(struct node a,struct node b)
{
	if(a.arrivalTime < b.arrivalTime)
		return true;
	else if(a.arrivalTime > b.arrivalTime)
		return false;
	else{
		if(a.priority < b.priority)
			return true;
		else
			return false;
	}

}



int main(int argc, char **argv)
{

fstream f;
f.open(argv[2],ios::in);
//f.open("process_dp.txt",ios::in);

int process = atoi(argv[1]),threshold;
//int process = 4,threshold;

vector<struct node> Ftemp;
struct node temp;
int a,b,c;

for(int i=0;i<process;i++)
{
	f>>a>>b>>c;

	temp.pid = i+1;
	temp.arrivalTime = a;
	temp.serviceTime = b;
	temp.priority = c;
	temp.finishTime = 0;
	temp.waitTime = 0;
	temp.remainingTime = b;
	temp.t_wait = 0;
	
	Ftemp.push_back(temp);
}

//Sort the Vector HERE
sort(Ftemp.begin(),Ftemp.end(),compare);

cout<<"Enter Threshold Wait time."<<endl;
cin>>threshold;

int currentTime = 0, min;
a = 99999999;
vector<struct node>::iterator it,pos;

while(check(&Ftemp))
{
//	cout<<4<<endl;

	min = 99999999;
	a = 99999999;

	for(it=Ftemp.begin(); (*it).arrivalTime<=currentTime && it!=Ftemp.end(); it++)
	{
		if( (*it).priority < min && (*it).remainingTime != 0 )//&& (*it).arrivalTime < a )
		{
			min = (*it).priority;
			pos = it;
			a = (*it).arrivalTime;
		}
		else if((*it).priority == min && (*it).remainingTime != 0 && (*it).arrivalTime < a )
		{
			min = (*it).priority;
			pos = it;
			a = (*it).arrivalTime;
		}
		
	}
	
	(*pos).remainingTime -= 1;
	currentTime += 1;
	
	WaitCounter(&Ftemp, currentTime,(*pos).pid);
	TWaitCounter(&Ftemp, currentTime,(*pos).pid,threshold);

	if((*pos).remainingTime == 0)
		(*pos).finishTime = currentTime;
}

//Output Goes HERE
for(it = Ftemp.begin();it != Ftemp.end();it++)
		cout<<(*it).pid<<". "<<"AT : "<<(*it).arrivalTime<<" | "<<"ST : "<<(*it).serviceTime<<" | "<<"WT : "<<(*it).waitTime<<" | "<<"FT : "
		    <<(*it).finishTime<<" ."<<endl<<endl;


return 0;
}
