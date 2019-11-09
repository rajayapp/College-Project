#include<iostream>
#include<cstdlib>//rand function
#include<ctime>//time function
#include"Module_2.h"

using namespace std;



int Module_2::Random_Number_Generator(int first , int last)
{
srand(time(NULL));

return first + rand() % (last - first);

}



void Module_2::swap(int *a , int *b)
{

int temp = *a;
*a = *b;
*b = temp;

}



int Module_2::Classic_Partition(int *parray , int pfirst , int plast)
{

int pivot = parray[plast];
int i = pfirst-1;

for(int j=pfirst;j<plast;j++)
{
	if(parray[j]<=pivot)
	{
		i += 1;
		swap(&parray[i],&parray[j]);
	}
}

swap(&parray[i+1],&parray[plast]);

return i+1;
}



/*int First_Partition(int *parray , int pfirst , int plast)
{

int pivot = parray[pfirst];
int i = pfirst;

for(int j=pfirst+1;j<=plast;j++)
{
	if(parray[j]<=pivot)
	{
		i += 1;
		swap(&parray[i],&parray[j]);
	}
}

swap(&parray[i],&parray[pfirst]);

return i;
}*/



void Module_2::Quicksort_Classic(int *qarray , int first , int last)
{

if(first<last)
{
	int current_pivot = Classic_Partition(qarray,first,last);
	Quicksort_Classic(qarray,first,current_pivot-1);
	Quicksort_Classic(qarray,current_pivot+1,last);
}
else
	return;

}



void Module_2::Quicksort_First(int *qarray , int first , int last)
{

if(first<last)
{
	swap(&qarray[first],&qarray[last]);
	int current_pivot = Classic_Partition(qarray,first,last);//First_Partition(qarray,first,last);
	Quicksort_First(qarray,first,current_pivot-1);
	Quicksort_First(qarray,current_pivot+1,last);
}
else
	return;

}



void Module_2::Quicksort_Middle(int *qarray , int first , int last)
{

if(first<last)
{
	int temp = (first+last)/2;
	swap(&qarray[temp],&qarray[last]);
	int current_pivot = Classic_Partition(qarray,first,last);//First_Partition(qarray,first,last);
	Quicksort_Middle(qarray,first,current_pivot-1);
	Quicksort_Middle(qarray,current_pivot+1,last);
}
else
	return;

}



void Module_2::Quicksort_Random(int *qarray , int first , int last)
{

if(first<last)
{
	int temp=Random_Number_Generator(first,last);//(last-first+1);
	cout<<endl<<temp<<endl;
	swap(&qarray[temp],&qarray[last]);
	int current_pivot = Classic_Partition(qarray,first,last);//First_Partition(qarray,first,last);
	Quicksort_Random(qarray,first,current_pivot-1);
	Quicksort_Random(qarray,current_pivot+1,last);
}
else
	return;

}

