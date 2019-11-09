#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{

//	int processes = (int)(*argv[1])-48,startTime=0,executionTime=0,currentTime=0,waitTime=0,i=0;
	//printf("%d\n",processes);
	int processes = atoi((argv[1])),startTime=0,executionTime=0,currentTime=0,waitTime=0,i=0;

	//printf("here\nprocess:%d",processes);	
	
	FILE *f = fopen(argv[2] , "r+");
	//cout<<argv[1]<<endl;
	//printf("here\n");
	printf("\n");

	for(i=0;i< processes;i++)
	{
	//printf("here\n");
		waitTime=0;

		fscanf(f,"%d %d",&startTime,&executionTime);

		if(currentTime < startTime)
		{
			currentTime = startTime;
		}
		else
		{
			waitTime = currentTime - startTime;
		}

		printf("Process %d :- AT:%d WT:%d ST:%d SRT:%d FT:%d.\n\n",i+1,startTime,waitTime,currentTime,executionTime,currentTime+executionTime);

		currentTime += executionTime;
	}

	return 0;
}
