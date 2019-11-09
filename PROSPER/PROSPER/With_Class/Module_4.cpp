#include<iostream>
#include<locale>
#include<string>
#include<cstdlib>
#include<vector>//vector
#include<iterator>
#include<algorithm>//sort
#include"Module_4.h"

using namespace std;



void Module_4::move(int disk,string First,string Last)
{

cout<<"Move disk "<<disk<<" from "<<First<<" to "<<Last<<"."<<endl<<endl;

}



void Module_4::Tower_Of_Hanoi(int disk , string Begin , string End , string Intermediate )//Tower of Hanoi Recursive
{
if(disk==1)
	move(disk,Begin,End);
else
	{

	Tower_Of_Hanoi(disk-1,Begin,Intermediate,End);
	move(disk,Begin,End);
	Tower_Of_Hanoi(disk-1,Intermediate,End,Begin);

	}

}



void Module_4::Step_Count(int steps)//For printing and counting the steps
{
	int *a,One_Step=steps,Two_Steps=0;
	int temp=steps;

	a=new int[steps];

	for(int l_counter=0;l_counter<steps;l_counter++)
		a[l_counter]=1;


	for(int l_counter=0;l_counter<steps;l_counter++)
		cout<<a[l_counter]<<"\t";


	cout<<endl<<endl<<endl;


	for(int l_counter=0;l_counter<steps/2;l_counter++)
	{
		One_Step-=2;
		Two_Steps++;

		if( ((One_Step*1) + (Two_Steps*2)) == steps )
		{

			int *a1;

			a1=new int[--temp];

			int l_counter1=0;

			for( l_counter1=0;l_counter1<One_Step;l_counter1++)
				a1[l_counter1]=1;


			for(int l_counter=l_counter1;l_counter<temp;l_counter++)
				a1[l_counter]=2;


			do {

				for(int l_counter=0;l_counter<temp;l_counter++)
					cout<<a1[l_counter]<<"\t";
				
				cout<<endl<<endl;

			  } while ( std::next_permutation(a1,a1+temp) );


			cout<<endl<<endl;

			delete[] a1;
		}
	}
delete[] a;
}



void Module_4::Staircase_Problem()//Helper function
{
int steps;

cout<<"Enter the Number of Steps"<<endl;
cin>>steps;
cout<<endl<<endl<<endl;

Step_Count(steps);

}




void Module_4::Hanoi_Problem()//Helper function
{

int disks;
string s1="From-Pole",s2="Help-Pole",s3="To-Pole";

cout<<"Enter the number of disks"<<endl;
cin>>disks;

Tower_Of_Hanoi(disks,s1,s3,s2);

}



struct node{

int height,weight;

};



bool Module_4::compare1(struct node a , struct node b)//Compare function for Descending order
{

if(a.height > b.height)
	return true;
else if(a.height < b.height)
	return false;
else if(a.height == b.height)
{
	if(a.weight > b.weight)
		return true;
	else if(a.weight < b.weight)
		return false;
}

return true;

}



bool Module_4::compare2(struct node a , struct node b)//Compare function for Ascending order
{

if(a.height < b.height)
	return true;
else if(a.height > b.height)
	return false;
else if(a.height == b.height)
{
	if(a.weight < b.weight)
		return true;
	else if(a.weight > b.weight)
		return false;
}

return true;

}



vector<struct node> Module_4::maxCount_Desc(vector<struct node> &list)//Counting height of tower with Desending ordered vector
{
vector<struct node> max,current;
vector<struct node>::iterator iter,iter1;

for(iter = list.begin() ;iter != list.end();iter++)
{

	struct node temp = *iter;
	current.push_back(temp);

	for(iter1 = iter+1 ;iter1 != list.end();iter1++)
	{

		if( current.back().height > (*iter1).height && current.back().weight > (*iter1).weight )
		{
		temp = *iter1;
		current.push_back(temp);
		}
		else
			continue;

	}

	if( current.size() > max.size() )
	{
		max.swap(current);
	}

	current.clear();

}

return max;
}



vector<struct node> Module_4::maxCount_Asc(vector<struct node> &list)//Counting height of tower with Asending ordered vector
{
vector<struct node> max,current;
vector<struct node>::iterator iter,iter1;

for(iter = list.begin() ;iter != list.end();iter++)
{

	struct node temp = *iter;
	current.push_back(temp);

	for(iter1 = iter+1 ;iter1 != list.end();iter1++)
	{

		if( current.back().height < (*iter1).height && current.back().weight < (*iter1).weight )
		{
		temp = *iter1;
		current.push_back(temp);
		}
		else
			continue;

	}

	if( current.size() > max.size() )
	{
		max.swap(current);
	}

	current.clear();

}

return max;
}



void Module_4::Circus_Problem()//HELPER FUNCTION
{
int Total=0;
struct node man;
vector<struct node> persons,max,max1,max2;

cout<<endl<<"Enter the Number of persons."<<endl;
cin>>Total;

for(int l_counter=0;l_counter<Total;l_counter++)
{

int count=0;
string s1,t_height,t_weight;

cout<<"Enter height and weight of the person"<<endl<<endl;
cin>>s1;

int i=0;

while(i<s1.length())//For storing height and weight part of the string
{
	if(isdigit(s1[i]) && count==0)
	{
		while(isdigit(s1[i]))
		{
			t_height += s1[i++];
			count++;
		}
	}

	if(isdigit(s1[i]) && count!=0)
	{
		while(isdigit(s1[i]))
			t_weight += s1[i++];
	}

	i++;
}
man.height = stoi(t_height,nullptr,10);//converts string to int
man.weight = stoi(t_weight,nullptr,10);//converts string to int

persons.push_back(man);

cout<<endl<<endl<<"Height is :- "<<man.height<<endl<<"Weight is :- "<<man.weight<<endl;

}

vector<struct node>::iterator iter;
cout<<"the entered persons heights and weights are :-"<<endl<<endl;
for(iter = persons.begin() ;iter != persons.end();iter++)
	cout<<"height is :- "<<(*iter).height<<endl<<"weight is :-"<<(*iter).weight<<endl<<endl;


std::sort(persons.begin(),persons.end(),compare1);
cout<<endl<<endl<<"the entered persons heights and weights after SORTING in Descending order are :-"<<endl<<endl;
for(iter = persons.begin() ;iter != persons.end();iter++)
	cout<<"height is :- "<<(*iter).height<<endl<<"weight is :-"<<(*iter).weight<<endl<<endl;
max1 = maxCount_Desc(persons);


std::sort(persons.begin(),persons.end(),compare2);
cout<<endl<<endl<<"the entered persons heights and weights after SORTING in Ascending order are :-"<<endl<<endl;
for(iter = persons.begin() ;iter != persons.end();iter++)
	cout<<"height is :- "<<(*iter).height<<endl<<"weight is :-"<<(*iter).weight<<endl<<endl;
max2 = maxCount_Asc(persons);



if(max1.size() >= max2.size())
	max.swap(max1);
else
	max.swap(max2);



cout<<endl<<endl<<"Largest possible number of persons in a tower is :- "<<max.size()<<endl<<endl;

cout<<"they are in the order of (height,weight) :-"<<endl<<endl;
for(iter = max.begin() ;iter != max.end();iter++)
	cout<<"( "<<(*iter).height<<", "<<(*iter).weight<<" )"<<endl;

}
