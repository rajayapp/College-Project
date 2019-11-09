#include<iostream>
#include<locale>
#include<string>
#include<cstdlib>
#include<vector>//vector
#include<iterator>
#include<algorithm>//sort
#include"Module_4.h"

using namespace std;



void move(int disk,string First,string Last)
{

cout<<"Move disk "<<disk<<" from "<<First<<" to "<<Last<<"."<<endl<<endl;

}



void Tower_Of_Hanoi(int disk , string Begin , string End , string Intermediate )//Tower of Hanoi Recursive
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



void Step_Count(int steps)//For printing and counting the steps
{
	int *a,One_Step=steps,Two_Steps=0,count=1;
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
				
				count++;
				cout<<endl<<endl;

			  } while ( std::next_permutation(a1,a1+temp) );


			cout<<endl<<endl;

			delete[] a1;
		}
	}

cout<<"The number of steps : "<<count<<endl;

delete[] a;
}



void Staircase_Problem()//Helper function
{
int steps;

cout<<"Enter the Number of Steps"<<endl;
cin>>steps;
cout<<endl<<endl<<endl;

Step_Count(steps);

}




void Hanoi_Problem()//Helper function
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



bool compare1(struct node a , struct node b)//Compare function for Descending order
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



bool compare2(struct node a , struct node b)//Compare function for Ascending order
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



vector<struct node> maxCount_Desc(vector<struct node> &list)//Counting height of tower with Desending ordered vector
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



vector<struct node> maxCount_Asc(vector<struct node> &list)//Counting height of tower with Asending ordered vector
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



void Circus_Problem()//HELPER FUNCTION
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


sort(persons.begin(),persons.end(),compare1);
cout<<endl<<endl<<"the entered persons heights and weights after SORTING in Descending order are :-"<<endl<<endl;
for(iter = persons.begin() ;iter != persons.end();iter++)
	cout<<"height is :- "<<(*iter).height<<endl<<"weight is :-"<<(*iter).weight<<endl<<endl;
max1 = maxCount_Desc(persons);


sort(persons.begin(),persons.end(),compare2);
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


int matrix[100][100],n_rows,n_columns,r_rows,r_columns,counter;

        void get_matrix()
           {
               int i,j;
               cout<<"enter the size of matrix rows and columns respectively"<<endl;
               cin>>n_rows>>n_columns;
               for(i=0;i<n_rows;i++)
                {
                   for(j=0;j<n_columns;j++)
                     {
                        matrix[i][j]=0;
                     }
                }
           }
           
        void remove_a_cell()
           {
               cout<<"enter the row and column of cell which you want to remove"<<endl;
               cin>>r_rows>>r_columns;
               matrix[r_rows][r_columns]=-1;
           }
        
        void insert_values_using_triomino()
           {
              counter=1;
              partition(0,0,n_rows,n_columns);
           }
           
        void partition(int start_row , int start_column , int end_row , int end_column)
           {
              if((end_row - start_row) > 1 && (end_column - start_column) > 1 && 
              end_row <= n_rows && end_column <= n_columns)
               {
                    int p =(end_row + start_row-1)/2;
                    int q = (end_column + start_column-1)/2;
                    //cout<<start_row<<" "<<start_column<<" "<<end_row<<" "<<end_column<<endl;
                    place_triomino(p,q,start_row,start_column);
                    partition(start_row,start_column,p+1,q+1);
                    partition(start_row,q+1,p+1,end_column);
                    partition(p+1,q+1,end_row,end_column);
                    partition(p+1,start_column,end_row,q+1);
                    /*partition(start_row,q+1,p+1,2*(q+1));
                    partition(p+1,q+1,2*(p+1),2*(q+1));
                    partition(p+1,start_column,2*(p+1),q+1);*/
               }
              else
               {
                return;
               }
            }
               
         void place_triomino(int p, int q, int start_row, int start_column)
           {
               int i,j;
               //cout<<","<<endl;
               if((p-start_row+1) < 2 && (q-start_column+1) < 2)
                 {
                     for(i=start_row;i<=p+1;i++)
                       {
                          for(j=start_column;j<=q+1;j++)
                            {
                               if(matrix[i][j]==0)
                                matrix[i][j]=counter;
                               else
                                continue;
                            }
                       }
                 }
               else
                 {
                    if(r_rows <= p && r_columns <= q)
                      {
                         matrix[p+1][q+1]=counter;
                         matrix[p+1][q]=counter;
                         matrix[p][q+1]=counter;
                      }
                    else if(r_rows <= p && r_columns > q)
                      {
                         matrix[p][q]=counter;
                         matrix[p+1][q]=counter;
                         matrix[p+1][q+1]=counter;
                      }
                    else if(r_rows > p && r_columns <= q)
                      {
                         matrix[p+1][q+1]=counter;
                         matrix[p][q]=counter;
                         matrix[p][q+1]=counter; 
                      }
                    else
                      {
                         matrix[p][q]=counter;
                         matrix[p+1][q]=counter;
                         matrix[p][q+1]=counter;
                      }
                 }
               counter++;
           }
           
         void print_matrix()
           {
               int i,j;
               cout<<"the new matrix is"<<endl;
               for(i=0;i<n_rows;i++)
                 {
                    for(j=0;j<n_columns;j++)
                      {
                         cout<<matrix[i][j]<<"\t";
                      }
                    cout<<endl;
                 }
           }
 
void Triomino(void)
  {
   get_matrix();
   remove_a_cell();
   insert_values_using_triomino();
   print_matrix();
  }
