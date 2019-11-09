#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>



using namespace std;



//struct for getting input of each process
struct node{
	int pid,arrivalTime,serviceTime,executedTime,completed,finishTime,waitTime;
};



//Compare function for sort function
bool compare(struct node a,struct node b)
{
	if(a.arrivalTime < b.arrivalTime)
		return true;
	else if(a.arrivalTime > b.arrivalTime)
		return false;
	else{
		if(a.serviceTime < b.serviceTime)
			return true;
		else //if(a.serviceTime >= b.serviceTime)
			return false;
	}

}



bool check(vector<struct node> *temp, int limit)
{
	vector<struct node>::iterator it;

	for(it = temp->begin();(*it).arrivalTime <= limit && it != temp->end();it++)
	{
		if((*it).completed == 0)
			return true;
	}

	return false;
}



void waitTimeCalculate(vector<struct node> *temp, int limit, int toAdd, int ex)
{
	vector<struct node>::iterator it;

	for(it = temp->begin();(*it).arrivalTime < limit && it != temp->end();it++)
	{
/*		if( (limit - (*it).arrivalTime) < toAdd)
			(*it).waitTime += (limit - (*it).arrivalTime);
		else
*/			if((*it).completed != 1 && (*it).pid != ex)
			{
				if( (limit - (*it).arrivalTime) < toAdd)
					(*it).waitTime += (limit - (*it).arrivalTime);
				else	
					(*it).waitTime += toAdd;
			}
	}
}



int main(int argc, char *argv[])
{
	fstream f;
	f.open(argv[2],ios::in);

	int processes = atoi((argv[1]));//Taking in the #processes

	vector<struct node>Ftemp;
	struct node temp;
	int a,b;

	for(int i=0;i<processes;i++)//Taking input from the file & into a vector
	{
		f>>a>>b;
		temp.pid = i+1;
		temp.arrivalTime = a;
		temp.serviceTime = b;
		temp.executedTime = 0;
		temp.completed = 0;
		temp.finishTime = 0;
		temp.waitTime = 0;

		Ftemp.push_back(temp);
	}

	sort(Ftemp.begin(),Ftemp.end(),compare);//Sorting in increasing order of arrivaltime and servicetime(if arrivaltime are equal)
	
	int quantum;
	cout<<"Enter Quantum."<<endl;
	cin>>quantum;
	
	vector<struct node>::iterator it = Ftemp.begin();
	int currentTime = 0;
	
	while(check(&Ftemp,currentTime))
	{
		for(it = Ftemp.begin();(*it).arrivalTime <= currentTime && it != Ftemp.end();it++)
		{
			if((*it).completed != 1)
			{
				if( ((*it).serviceTime - (*it).executedTime) <= quantum )
				{
					currentTime += ((*it).serviceTime - (*it).executedTime);
					(*it).completed = 1;
					waitTimeCalculate( &Ftemp, currentTime, ((*it).serviceTime - (*it).executedTime), (*it).pid );
					(*it).executedTime = (*it).serviceTime;
					(*it).finishTime = currentTime;
					
					//cout<<(*it).pid<<". "<<"AT : "<<(*it).arrivalTime<<" | "<<""
				}
				else
				{
					currentTime += quantum;
					(*it).executedTime += quantum;
					waitTimeCalculate( &Ftemp, currentTime, quantum, (*it).pid );
				}
		
				/*if( it == (Ftemp.end()-1) )
					it = Ftemp.begin();*/
			}
			/*else	
				if( it == (Ftemp.end()-1) )
					it = Ftemp.begin();*/	
		}
		
	}
	
	//Output Goes HERE
	for(it = Ftemp.begin();it != Ftemp.end();it++)
		cout<<(*it).pid<<". "<<"AT : "<<(*it).arrivalTime<<" | "<<"ST : "<<(*it).serviceTime<<" | "<<"WT : "<<(*it).waitTime<<" | "<<"FT : "
		    <<(*it).finishTime<<" ."<<endl<<endl;
	

	return 0;
}
