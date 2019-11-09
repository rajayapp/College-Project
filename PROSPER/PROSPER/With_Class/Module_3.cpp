#include<iostream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include"Module_1.h"
#include"Module_2.h"
#include"Module_3.h"



using namespace std;



Module_2 M2;
Module_1 M1;



void Module_3::Linear_Search(int *a , int size , int key)
{
int l_counter=0;

cout<<"key is : "<<key<<endl<<endl;

for(l_counter=0;l_counter<size;l_counter++)
{
	if(a[l_counter]==key)
	{
		cout<<"Key found at position : "<<l_counter+1<<endl;
		break;
	}

}

if(l_counter==size)
	cout<<"Key is not present in the array"<<endl;

}



void Module_3::Quicksort_Hybrid(int *qarray , int first , int last)
{

if(first<last)
{
	int current_pivot = M2.Classic_Partition(qarray,first,last);
	if(current_pivot - first > 20)
		Quicksort_Hybrid(qarray,first,current_pivot-1);
	else
		M1.Insertion_Sort_Iteration(qarray,first,current_pivot-1);

	if(last - current_pivot > 20)
		Quicksort_Hybrid(qarray,current_pivot+1,last);
	else
		M1.Insertion_Sort_Iteration(qarray,current_pivot+1,last);
}
else
	return;

}



void Module_3::Merge(int *tarray,int tfirst,int t_q , int tlast)
{
int n1 = t_q-tfirst+1;
int n2 = tlast-t_q;

int *left = new int[n1+1];
int *right = new int[n2+1];

for(int loop_counter=0;loop_counter<n1;loop_counter++)
	left[loop_counter]=tarray[tfirst+loop_counter];

for(int loop_counter=0;loop_counter<n2;loop_counter++)
	right[loop_counter]=tarray[t_q+1+loop_counter];

left[n1]=99999999;
right[n2]=99999999;

int index1=0,index2=0;

for(int loop_counter=tfirst;loop_counter<=tlast;loop_counter++)
{
	if(left[index1]<=right[index2])// && index1!=NULL && index2!=NULL)
	{
		tarray[loop_counter] = left[index1];
		index1++;
	}
	else if(left[index1]>right[index2])// && index1!=NULL && index2!=NULL)
	{
		tarray[loop_counter] = right[index2];
		index2++;
	}
	else
		break;
}

}



void Module_3::Mergesort_Hybrid(int *marray,int first,int last)
{

if(first < last)
{
	int q = (first+last)/2;//floor((first+last)/2);

	if(q - first+1 > 20)
		Mergesort_Hybrid(marray,first,q);
	else
		M1.Insertion_Sort_Iteration(marray,first,q);

	if(last - q > 20)
		Mergesort_Hybrid(marray,q+1,last);
	else
		M1.Insertion_Sort_Iteration(marray,q+1,last);

	Merge(marray,first,q,last);
}
else
	return;
}



void Module_3::Variant_Linear_Search(int *a,int tsize,int key)
{
int first=0,last=tsize-1;

cout<<"key is : "<<key<<endl<<endl;

while(first<=last)
{

	if(a[first]==key)
	{
		cout<<"key found at : "<<first+1<<endl;
		break;
	}

	if(a[last]==key)
	{
		cout<<"key found at : "<<last+1<<endl;
		break;
	}

	first++;
	last--;
}

if(first>last)
{
	cout<<"Key not present"<<endl;
}

}
