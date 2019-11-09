#include<iostream>
#include"Module_5.h"
#include"Module_4.h"

using namespace std;



void Instructions()
{
cout<<"Enter :- 1.Plot_Insertion_sort"<<endl
    <<"         2.Plot_Quick_sort()"<<endl
    <<"         3.Plot_Hybrid_Sort"<<endl
    <<"         4.Plot_Variant_Linear"<<endl
    <<"         5.Tower_Of_Hanoi"<<endl
    <<"         6.Staircase_Problem"<<endl
    <<"         7.Triomino"<<endl
    <<"         8.Circus_Problem"<<endl
    <<"         9.EXIT"<<endl;

}



int main()
{
int choice;

Instructions();
cin>>choice;


while(choice != 9)
{
	switch(choice)
	{
	case 1:
		Plot_Insertion_sort();
		break;

	case 2:
		Plot_Quick_sort();
		break;

	case 3:
		Plot_Hybrid_Sort();
		break;

	case 4:
		Plot_Variant_Linear();
		break;

	case 5:
		Hanoi_Problem();
		break;

	case 6:
		Staircase_Problem();
		break;

	case 7:
		Triomino();
		break;	

	case 8:
		Circus_Problem();
		break;

	default:
		cout<<"Enter a valid choice"<<endl;
	}
Instructions();
cin>>choice;
}


return 0;
}
