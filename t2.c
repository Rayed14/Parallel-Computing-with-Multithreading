//Rayed Muhammad Saeed
//20i-1822
//CS-F
//Task 2
//PDC

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include<math.h>

struct Array 
{
	//unsigned int *arr;
	unsigned int startIndexr;
	//unsigned int traverseIndex;
	unsigned int startIndexc;
	unsigned int endIndexr;
	unsigned int endIndexc;
};

unsigned int r=256,c=256;
unsigned int arrSum=0; 
unsigned int  **arr; //global 2d array that will be used

void * populate(void * args) //populate function that populates the array with random values
{
	struct Array * object_indexes = (struct Array *) args;
	unsigned int startr = object_indexes->startIndexr;
	unsigned int startc = object_indexes->startIndexc;	
	unsigned int endr=object_indexes->endIndexr;
	unsigned int endc=object_indexes->endIndexc;
	
	unsigned int min = 8000, max = 16000; 
	for(unsigned int i = startr; i < endr; i++)
	{
		for (unsigned int j=startc;j<endc;j++)
		{
			//printf("checked\n");
			arr[i][j]  = (rand() % (max - min + 1)) + min;
			//printf("%d ",arr[i][j]);
			//arrSum+=*(arr+i*c+j);
		}	
	}
	//pthread_exit(NULL);
}  

void * sum(void *args) //function to compute the sum of array 
{
	struct Array * object_indexes = (struct Array *) args;
	unsigned int startr = object_indexes->startIndexr;
	unsigned int startc = object_indexes->startIndexc;	
	unsigned int endr=object_indexes->endIndexr;
	unsigned int endc=object_indexes->endIndexc;
	
	unsigned int min = 8000, max = 16000; 
	for(unsigned int i = startr; i < endr; i++)
	{
		for (unsigned int j=startc;j<endc;j++)
		{
			arrSum+=arr[i][j];
		}	
	}
	//printf("The sum of the array is: %d\n",arrSum);
	//pthread_exit(NULL);
}

void blockwise(int blck) //function to create the threads
{
	unsigned int noThreads=(r*c)/blck*blck;
	pthread_t *threads = calloc(noThreads, sizeof(pthread_t));
    struct Array *obj = calloc(noThreads, sizeof(struct Array));
	//unsigned int startIndex=0;
	unsigned int rows=r/4; //storing the rows till we have to iterate
	unsigned int colStart=0; //defines start of the column
	unsigned int currRowIndex= 0;
	int threadCount=0; 
	
	for(int i=0;i<r;i+=blck)
	{
		colStart=0;
		while(colStart+blck-1<=c-1)
		{
		//	printf("check %d\n",i);
			obj[threadCount].startIndexr=currRowIndex;
			obj[threadCount].endIndexr=obj[threadCount].startIndexr+blck-1;
			// printf("start row = %d\n",obj[threadCount].startIndexr);
			// printf("end row = %d\n",obj[threadCount].endIndexr);
			obj[threadCount].startIndexc=colStart;
			obj[threadCount].endIndexc=obj[threadCount].startIndexc+blck-1;
			// printf("start col = %d\n",obj[threadCount].startIndexc);
			// printf("end col = %d\n",obj[threadCount].endIndexc);
			pthread_create(&threads[threadCount], NULL, populate,(void *) &obj[threadCount] );
			//pthread_join(threads[i], NULL);
			threadCount++;
			colStart+=blck;
		}
		//	printf("checks\n");
			currRowIndex+=blck;
	}
	for(unsigned int i=0;i<threadCount;i++)
	{
		pthread_join(threads[i], NULL);	
	}
	
	noThreads=(r*c)/blck*blck;
	threads = calloc(noThreads, sizeof(pthread_t));
    obj = calloc(noThreads, sizeof(struct Array));
	//unsigned int startIndex=0;
	rows=r/4; //storing the rows till we have to iterate
	colStart=0; //defines start of the column
	currRowIndex= 0;
	threadCount=0; 

	for(int i=0;i<r;i+=blck)
	{
		colStart=0;
		while(colStart+blck-1<=c-1)
		{
		//	printf("check %d\n",i);
			obj[threadCount].startIndexr=currRowIndex;
			obj[threadCount].endIndexr=obj[threadCount].startIndexr+blck-1;
			// printf("start row = %d\n",obj[threadCount].startIndexr);
			// printf("end row = %d\n",obj[threadCount].endIndexr);
			obj[threadCount].startIndexc=colStart;
			obj[threadCount].endIndexc=obj[threadCount].startIndexc+blck-1;
			// printf("start col = %d\n",obj[threadCount].startIndexc);
			// printf("end col = %d\n",obj[threadCount].endIndexc);
			pthread_create(&threads[threadCount], NULL, sum,(void *) &obj[threadCount] );
			//pthread_join(threads[i], NULL);
			threadCount++;
			colStart+=blck;
		}
		//	printf("checks\n");
			currRowIndex+=blck;
	}
	for(int i=0;i<threadCount;i++)
	{
		pthread_join(threads[i], NULL);	
	}

	printf("The sum of array is: %d \n",arrSum);
	
}

void cyclic()
{
	int noThreads=2;
	int rows=4;
	pthread_t *threads = calloc(noThreads, sizeof(pthread_t));
	struct Array *obj = calloc(noThreads, sizeof(struct Array));
	//pthread_t threads[noThreads];
	//struct Array obj;
	obj->startIndexr=0;
	obj->startIndexc=0;
	obj->endIndexr=rows-1;
	obj->endIndexc=c-1;

	while(obj->endIndexr<r)
	{
		for(int i=0;i<noThreads;i++)
		{
			pthread_create(&threads[i], NULL, populate,(void*) &obj );
			//pthread_join(threads[i],NULL);
			obj->startIndexr=obj->startIndexr+rows;
			obj->endIndexr=obj->endIndexr+rows;
		}
	}
	for(int i=0;i<noThreads;i++)
	{
		pthread_join(threads[i],NULL);
	}
	// printf("The populated array is: \n");
	// for (int i=0;i<r;i++)
	// {
	// 	for(int j=0;j<c;j++)
	// 	{
	// 		printf("%d ",arr[i][j]);
	// 	}
	// 	printf("\n");
	// }

	obj->startIndexr=0;
	obj->startIndexc=0;
	obj->endIndexr=rows-1;
	obj->endIndexc=c-1;

	while(obj->endIndexr<r)
	{
		for(int i=0;i<noThreads;i++)
		{
			pthread_create(&threads[i], NULL, sum,(void*) &obj );
			//pthread_join(threads[i],NULL);
			obj->startIndexr=obj->startIndexr+rows;
			obj->endIndexr=obj->endIndexr+rows;
		}
	}
	for(int i=0;i<noThreads;i++)
	{
		pthread_join(threads[i],NULL);
	}

	printf("The sum of array is: %d \n",arrSum);
}

int main()
{
	srand(time(0));
	arr = calloc(r, sizeof( int*));  //dynamic allocation of array
	for(int i=0;i<r;i++)
		arr[i]=calloc(c,sizeof( int));
/*   	printf("The matrix elements are:\n"); */
/*   	for (unsigned int i = 0; i < r; i++) //prunsigned inting the array elements*/
/*   	{*/
/*      	for (unsigned int j = 0; j < c; j++) */
/*      	{*/
/*         	printf("%d ", *(arr + i*c + j)); */
/*      	}*/
/*      printf("\n"); */
/*   	}*/
	
	unsigned int operation_count=r*c*2; //number of op
    struct timeval start, end;

    gettimeofday( &start, NULL );

    blockwise(4); //call to function 

    gettimeofday( &end, NULL );

    /* convert time to Gflop/s */
    printf("The performance for blockwise 2^2 is: \n");
    double seconds = (end.tv_sec - start.tv_sec) +
            1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflops = 2e-9*operation_count/seconds;
    printf( " Performance in Gflops %.8f Gflop/s\n\n", Gflops );

	gettimeofday( &start, NULL );

    blockwise(16); //call to function 

    gettimeofday( &end, NULL );

    /* convert time to Gflop/s */
    printf("The performance for blockwise 2^4 is: \n");
    seconds = (end.tv_sec - start.tv_sec) +
            1.0e-6 * (end.tv_usec - start.tv_usec);
    Gflops = 2e-9*operation_count/seconds;
    printf( " Performance in Gflops %.8f Gflop/s\n\n", Gflops );

	gettimeofday( &start, NULL );

    blockwise(256); //call to function 

    gettimeofday( &end, NULL );

    /* convert time to Gflop/s */
    printf("The performance for blockwise 2^8 is: \n");
    seconds = (end.tv_sec - start.tv_sec) +
            1.0e-6 * (end.tv_usec - start.tv_usec);
    Gflops = 2e-9*operation_count/seconds;
    printf( " Performance in Gflops %.8f Gflop/s\n\n", Gflops );

	gettimeofday( &start, NULL );

    cyclic(); //call to function 

    gettimeofday( &end, NULL );

    /* convert time to Gflop/s */
    printf("The performance for cyclic is: \n");
    seconds = (end.tv_sec - start.tv_sec) +
            1.0e-6 * (end.tv_usec - start.tv_usec);
    Gflops = 2e-9*operation_count/seconds;
    printf( " Performance in Gflops %.8f Gflop/s\n\n", Gflops );

	
	return 0;	
}







