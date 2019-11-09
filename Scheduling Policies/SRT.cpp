#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>

/*0 3
6 2
7 4
2 5*/

using namespace std;



//struct for getting input of each process
struct node{
	int arrivalTime,serviceTime,remainingTime,waitTime,finishTime,pid;
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



bool check(vector<struct node> *temp)
{
	vector<struct node>::iterator it;

	for(it = temp->begin();it!=temp->end();it++)
	{
		if((*it).remainingTime != 0)
			return true;
	}

	return false;
}



void waitTimeCalculate(vector<struct node> *temp, int limit, int ex)
{
	vector<struct node>::iterator it;

	for(it = temp->begin();(*it).arrivalTime < limit && it != temp->end();it++)
	{
			if((*it).remainingTime != 0 && (*it).pid != ex)
					(*it).waitTime += 1;
	}
}



int main()//(int argc, char *argv[])
{
	fstream f;
	f.open("process.txt",ios::in);

	int processes = 4;//atoi((argv[1]));//Taking in the #processes

	vector<struct node>Ftemp;
	struct node temp;
	int a,b;

	for(int i=0;i<processes;i++)//Taking input from the file & into a vector
	{
		f>>a>>b;
		temp.arrivalTime = a;
		temp.serviceTime = b;
		temp.remainingTime = b;
		temp.waitTime = 0;
		temp.finishTime = 0;
		temp.pid = i+1;

		Ftemp.push_back(temp);
	}

	sort(Ftemp.begin(),Ftemp.end(),compare);//Sorting in increasing order of arrivaltime and servicetime(if arrivaltime are equal)

	int currentTime = Ftemp.front().arrivalTime, min = 99999999;

	vector<struct node>::iterator it,pos;
		
	while(check(&Ftemp))//====================================================================
	{
		min = 99999999;

		for(it = Ftemp.begin();(*it).arrivalTime<=currentTime && it != Ftemp.end();it++)
		{
			//cout<<(*it).remainingTime<<endl;

			if( (*it).remainingTime < min && (*it).remainingTime !=0 )
			{
				min = (*it).remainingTime;
				pos = it;
			}
		}

			currentTime += 1;
			(*pos).remainingTime -= 1;

			if( ((*pos).remainingTime) == 0)
				(*pos).finishTime = currentTime;

			waitTimeCalculate( &Ftemp, currentTime, (*pos).pid );
	}

	//Print the output
	for(it = Ftemp.begin();it != Ftemp.end();it++)
		cout<<(*it).pid<<". "<<"AT : "<<(*it).arrivalTime<<" | "<<"ST : "<<(*it).serviceTime<<" | "<<"WT : "<<(*it).waitTime<<" | "<<"FT : "
		    <<(*it).finishTime<<" ."<<endl<<endl;


	return 0;
}
