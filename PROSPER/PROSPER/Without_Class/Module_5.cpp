#include<iostream>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include"Module_1.h"
#include"Module_2.h"
#include"Module_3.h"



using namespace std;



void Plot_Insertion_sort()
{
int size=20,*a1,*a2,*a3,*a4;


for(int size=20;size<=100;size+=20)
{

	srand(time(NULL));

	for(int j=0;j<100;j++)
	{

		a1 = new int[size];
		a2 = new int[size];
		a3 = new int[size];
		a4 = new int[size];

//		srand(time(NULL));
		for(int l_counter=0;l_counter<size;l_counter++)
		{
			a1[l_counter] = rand() % 500 + 1;
			a2[l_counter] = a1[l_counter];
			a3[l_counter] = a1[l_counter];
			a4[l_counter] = a1[l_counter];
		}

//		for(int i=0;i<size;i++)
//			cout<<a1[i]<<"\t";

		clock_t Start = clock();
		Insertion_Sort_Iteration( a1 , 0 , size-1);
		double time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Insertion_Sort_Iteration_20.txt","a");
				break;

			case 40:
				fptr=fopen("Insertion_Sort_Iteration_40.txt","a");
				break;

			case 60:
				fptr=fopen("Insertion_Sort_Iteration_60.txt","a");
				break;

			case 80:
				fptr=fopen("Insertion_Sort_Iteration_80.txt","a");
				break;

			case 100:
				fptr=fopen("Insertion_Sort_Iteration_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		Insertion_Sort_Recursion( a2 , 0 , 0 , size);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Insertion_Sort_Recursion_20.txt","a");
				break;

			case 40:
				fptr=fopen("Insertion_Sort_Recursion_40.txt","a");
				break;

			case 60:
				fptr=fopen("Insertion_Sort_Recursion_60.txt","a");
				break;

			case 80:
				fptr=fopen("Insertion_Sort_Recursion_80.txt","a");
				break;

			case 100:
				fptr=fopen("Insertion_Sort_Recursion_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		binary_Insertion_Sort( a3 , 0 , size-1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("binary_Insertion_Sort_20.txt","a");
				break;

			case 40:
				fptr=fopen("binary_Insertion_Sort_40.txt","a");
				break;

			case 60:
				fptr=fopen("binary_Insertion_Sort_60.txt","a");
				break;

			case 80:
				fptr=fopen("binary_Insertion_Sort_80.txt","a");
				break;

			case 100:
				fptr=fopen("binary_Insertion_Sort_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		countInversions( a4 , size);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("countInversions_20.txt","a");
				break;

			case 40:
				fptr=fopen("countInversions_40.txt","a");
				break;

			case 60:
				fptr=fopen("countInversions_60.txt","a");
				break;

			case 80:
				fptr=fopen("countInversions_80.txt","a");
				break;

			case 100:
				fptr=fopen("countInversions_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);
		delete[] a1;
		delete[] a2;
		delete[] a3;
		delete[] a4;
	}
}


}



void Plot_Quick_sort()
{

int size=20,*a1,*a2,*a3,*a4;


for(int size=20;size<=100;size+=20)
{

	srand(time(NULL));

	for(int j=0;j<100;j++)
	{

		a1 = new int[size];
		a2 = new int[size];
		a3 = new int[size];
		a4 = new int[size];

//		srand(time(NULL));
		for(int l_counter=0;l_counter<size;l_counter++)
		{
			a1[l_counter] = rand() % 500 + 1;
			a2[l_counter] = a1[l_counter];
			a3[l_counter] = a1[l_counter];
			a4[l_counter] = a1[l_counter];
		}

		clock_t Start = clock();
		Quicksort_Classic( a1 , 0 , size-1);
		double time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Quicksort_Classic_20.txt","a");
				break;

			case 40:
				fptr=fopen("Quicksort_Classic_40.txt","a");
				break;

			case 60:
				fptr=fopen("Quicksort_Classic_60.txt","a");
				break;

			case 80:
				fptr=fopen("Quicksort_Classic_80.txt","a");
				break;

			case 100:
				fptr=fopen("Quicksort_Classic_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		Quicksort_First( a2 , 0 , size-1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Quicksort_First_20.txt","a");
				break;

			case 40:
				fptr=fopen("Quicksort_First_40.txt","a");
				break;

			case 60:
				fptr=fopen("Quicksort_First_60.txt","a");
				break;

			case 80:
				fptr=fopen("Quicksort_First_80.txt","a");
				break;

			case 100:
				fptr=fopen("Quicksort_First_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		Quicksort_Middle( a3 , 0 , size-1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Quicksort_Middle_20.txt","a");
				break;

			case 40:
				fptr=fopen("Quicksort_Middle_40.txt","a");
				break;

			case 60:
				fptr=fopen("Quicksort_Middle_60.txt","a");
				break;

			case 80:
				fptr=fopen("Quicksort_Middle_80.txt","a");
				break;

			case 100:
				fptr=fopen("Quicksort_Middle_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



//		srand(time(NULL));
		Start = clock();
		Quicksort_Random( a4 , 0 , size-1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Quicksort_Random_20.txt","a");
				break;

			case 40:
				fptr=fopen("Quicksort_Random_40.txt","a");
				break;

			case 60:
				fptr=fopen("Quicksort_Random_60.txt","a");
				break;

			case 80:
				fptr=fopen("Quicksort_Random_80.txt","a");
				break;

			case 100:
				fptr=fopen("Quicksort_Random_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);
		delete[] a1;
		delete[] a2;
		delete[] a3;
		delete[] a4;
	}
}

}



void Plot_Hybrid_Sort()
{

int size=20,*a1,*a2,*a3,*a4;


for(int size=20;size<=100;size+=20)
{

	srand(time(NULL));

	for(int j=0;j<100;j++)
	{

		a1 = new int[size];
		a2 = new int[size];
		a3 = new int[size];
		a4 = new int[size];

//		srand(time(NULL));
		for(int l_counter=0;l_counter<size;l_counter++)
		{
			a1[l_counter] = rand() % 500 + 1;
			a2[l_counter] = a1[l_counter];
			a3[l_counter] = a1[l_counter];
			a4[l_counter] = a1[l_counter];
		}

		clock_t Start = clock();
		Insertion_Sort_Iteration( a1 , 0 , size-1);
		double time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Insertion_Sort_Iteration_20.txt","a");
				break;

			case 40:
				fptr=fopen("Insertion_Sort_Iteration_40.txt","a");
				break;

			case 60:
				fptr=fopen("Insertion_Sort_Iteration_60.txt","a");
				break;

			case 80:
				fptr=fopen("Insertion_Sort_Iteration_80.txt","a");
				break;

			case 100:
				fptr=fopen("Insertion_Sort_Iteration_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		Quicksort_Classic( a2 , 0 , size-1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Quicksort_Classic_20.txt","a");
				break;

			case 40:
				fptr=fopen("Quicksort_Classic_40.txt","a");
				break;

			case 60:
				fptr=fopen("Quicksort_Classic_60.txt","a");
				break;

			case 80:
				fptr=fopen("Quicksort_Classic_80.txt","a");
				break;

			case 100:
				fptr=fopen("Quicksort_Classic_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		Quicksort_Hybrid( a3 , 0 , size-1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Quicksort_Hybrid_20.txt","a");
				break;

			case 40:
				fptr=fopen("Quicksort_Hybrid_40.txt","a");
				break;

			case 60:
				fptr=fopen("Quicksort_Hybrid_60.txt","a");
				break;

			case 80:
				fptr=fopen("Quicksort_Hybrid_80.txt","a");
				break;

			case 100:
				fptr=fopen("Quicksort_Hybrid_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		Mergesort_Hybrid( a4 , 0 , size-1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Mergesort_Hybrid_20.txt","a");
				break;

			case 40:
				fptr=fopen("Mergesort_Hybrid_40.txt","a");
				break;

			case 60:
				fptr=fopen("Mergesort_Hybrid_60.txt","a");
				break;

			case 80:
				fptr=fopen("Mergesort_Hybrid_80.txt","a");
				break;

			case 100:
				fptr=fopen("Mergesort_Hybrid_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);
		delete[] a1;
		delete[] a2;
		delete[] a3;
		delete[] a4;
	}
}

}



void Plot_Variant_Linear()
{

int size=20,*a1,*a2;


for(int size=20;size<=100;size+=20)
{

	srand(time(NULL));

	for(int j=0;j<100;j++)
	{

		a1 = new int[size];
		a2 = new int[size];

//		srand(time(NULL));
		for(int l_counter=0;l_counter<size;l_counter++)
		{
			a1[l_counter] = rand() % 500 + 1;
			a2[l_counter] = a1[l_counter];
		}

		clock_t Start = clock();
		Variant_Linear_Search( a1 , size , rand() % 500 + 1);
		double time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Variant_Linear_Search_20.txt","a");
				break;

			case 40:
				fptr=fopen("Variant_Linear_Search_40.txt","a");
				break;

			case 60:
				fptr=fopen("Variant_Linear_Search_60.txt","a");
				break;

			case 80:
				fptr=fopen("Variant_Linear_Search_80.txt","a");
				break;

			case 100:
				fptr=fopen("Variant_Linear_Search_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);



		Start = clock();
		Linear_Search( a2 , size , rand() % 500 + 1);
		time = ((clock() - Start) / double(CLOCKS_PER_SEC))*1000;

//		FILE *fptr;

		switch(size)
		{
			case 20:
				fptr=fopen("Linear_Search_20.txt","a");
				break;

			case 40:
				fptr=fopen("Linear_Search_40.txt","a");
				break;

			case 60:
				fptr=fopen("Linear_Search_60.txt","a");
				break;

			case 80:
				fptr=fopen("Linear_Search_80.txt","a");
				break;

			case 100:
				fptr=fopen("Linear_Search_100.txt","a");
				break;
		}

		fprintf(fptr,"%f\n",time);
		fclose(fptr);
		
		delete[] a1;
		delete[] a2;
	}
}

}



/*int main()
{

Plot_Insertion_sort();

return 0;
}
*/
