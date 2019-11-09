#include<iostream>
#include<ctime>
#include<cstdlib>
#include"Module_1.h"

using namespace std;



int binarySearch(int *a, int item, int low, int high)
{
	if (high <= low)
	{
		if(item > a[low])
		return low+1;
		else
		return low;
	}
 
	int mid = (low + high)/2;
 
	if(item == a[mid])
		return mid+1;
 
	if(item > a[mid])
		return binarySearch(a, item, mid+1, high);
	else
		return binarySearch(a, item, low, mid-1);
}
 


void binary_Insertion_Sort(int *array , int first , int last)
{

int key=0,i=0,pos=0;

for(int j= first+1;j<=last;j++)
{
	key = array[j];
	i = j-1;

	pos = binarySearch(array,key,first,i);

	while(i >= pos)
	{
		array[i+1] = array[i];
		i--;
	}

	array[i+1] = key;
}

}



void Insertion_Sort_Iteration(int *array , int first , int last)
{
int key=0,i=0;

for(int j= first+1;j<=last;j++)
{
	key = array[j];
	i = j-1;
	while(i>=first && array[i]>key)
	{
		array[i+1]=array[i];
		i--;
	}
	array[i+1] = key;
}

}



void countInversions(int *array,int size)
{
int first=0,last=size-1,count=0;

int l_counter1=0;
for( l_counter1=0;l_counter1<size;l_counter1++)
	for(int l_counter2=size-1;l_counter2>l_counter1;l_counter2--)
	{
		if( l_counter1 < l_counter2 && array[l_counter1] > array[l_counter2] )
			count++;
	}

cout<<endl<<"The total number of inversions present are :- "<<count<<endl;
}



void print(int *array , int size)
{

cout<<endl<<endl<<"Array :-"<<endl<<endl;

for(int loop_counter=0;loop_counter<size;loop_counter++)
	cout<<array[loop_counter]<<"\t";

cout<<endl<<endl;

}


//start = starting index of the array , first = the element upto which the array is sorted
void Insertion_Sort_Recursion(int *array , int first , int start , int tsize) 
{
    if (first < tsize)
    {
        int j;
        int temp = array[first];
 
        for (j = first; j > start && array[j-1] > temp; j--)
            array[j] = array[j-1];

        array[j] = temp;
 
        Insertion_Sort_Recursion(array , ++first , start , tsize);
    }
}
