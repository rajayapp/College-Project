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
	int arrivalTime,serviceTime;
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



int main(int argc, char *argv[])
{
	fstream f;
	f.open("process.txt",ios::in);//=============================================================================

	int processes = atoi((argv[1]));//Taking in the #processes

	vector<struct node>Ftemp;
	struct node temp;
	int a,b;

	for(int i=0;i<processes;i++)//Taking input from the file & into a vector
	{
		f>>a>>b;
		temp.arrivalTime = a;
		temp.serviceTime = b;

		Ftemp.push_back(temp);
	}

	sort(Ftemp.begin(),Ftemp.end(),compare);//=====================================================================Sorting in increasing order of arrivaltime and servicetime(if arrivaltime are equal)

	int currentTime=0,counter=0;

	//Printing the first processes
	currentTime = Ftemp.front().arrivalTime + Ftemp.front().serviceTime;
	cout<<++counter<<". "<<"AT : "<<Ftemp.front().arrivalTime<<" , "<<"ST : "<<Ftemp.front().serviceTime<<" , "
		<<"WT : 0"<<" , "<<"FT : "<<currentTime<<endl;
	Ftemp.erase(Ftemp.begin());

	//Printing the remaining processes
	while(Ftemp.size())
	{
		vector<struct node>::iterator it = Ftemp.begin(),pos;
		int min = 99999999;
		
		//selecting the process with minimum servicetime
		while( (*it).arrivalTime <= currentTime && it!=Ftemp.end() )
		{
			if((*it).serviceTime < min)
			{
				min = (*it).serviceTime;
				pos = it;
				it++;
			}
			else
				it++;
		}

		currentTime += (*pos).serviceTime;
		cout<<++counter<<". "<<"AT : "<<(*pos).arrivalTime<<" , "<<"ST : "<<(*pos).serviceTime<<" , "
			<<"WT : "<<currentTime - (*pos).arrivalTime - (*pos).serviceTime<<" , "<<"FT : "<<currentTime<<endl;
		Ftemp.erase(pos);	

	}

	return 0;
}
