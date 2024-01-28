//Rayed Muhammad Saeed
//20i-1822
//CS-F
//Task 1
//PDC

#define _GNU_SOURCE //Libraries
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
#define MAXLINE 1000

int arraySize=65536; //declaring the size of the array
typedef struct{ //declaring structure that will hold the start and end index of the array
	int startIndex;
	int endIndex;
}indices; //structure object
double *arr; //global array that will be used

void * populate(void * args) //populate function that populates the array with random values
{
	indices * object_indexes = (indices *) args;
	int start = object_indexes->startIndex;
	int end = object_indexes->endIndex;	
	//printf("~ Assigning random elements...\n");
	int min = 0, max = 16000;
	for(int i = start; i < end; i++)
	{
		arr[i]  = (rand() % (max - min + 1)) + min;
		//printf("%d ",arr[i]);
	}
	pthread_exit("NULL");
}  

void * calcSqrt(void * args) //square root function
{
	indices * object_indexes = (indices *) args;
	int start = object_indexes->startIndex;
	int end = object_indexes->endIndex;	
	for(int i = start; i < end; i++)
	{
		arr[i]  = sqrt(arr[i]);
	}
	pthread_exit("NULL");
}

void func(int cpus) //function not using affinity.
{
	int half=arraySize/2; //holds the half of the total size of the array
	for (int i=0;i<arraySize;i++) //loop that will initialize the array with 0's
	{
		arr[i]=0;
	}
	indices *p[cpus]; //structure object that will store start and end indexes
	for(int i=0;i<cpus;i++)
	{
		if(i==0)
		{
			p[i] = (indices  *) malloc(sizeof(indices )); 
			p[i]->startIndex = 0;
			p[i]->endIndex = half-1;
		}
		else
		{
			p[i] = (indices  *) malloc(sizeof(indices )); 
			p[i]->startIndex = half;
			p[i]->endIndex = arraySize;
		}
				
	}
	
/*	indices *p[2];*/
/*	p[0] = (indices  *) malloc(sizeof(indices )); */
/*		p[0]->startIndex = 0;*/
/*    	p[0]->endIndex = 32767;*/
/*    	*/
/*    	p[1] = (indices  *) malloc(sizeof(indices ));*/
/*    	p[1]->startIndex = 32768;*/
/*    	p[1]->endIndex = 65535;*/
	
	/*indices  * p1 = (indices  *) malloc(sizeof(indices )); 
		p1->startIndex = 0;
    	p1->endIndex = 32767;
    	
    	indices  * p2 = (indices  *) malloc(sizeof(indices ));
    	p2->startIndex = 32768;
    	p2->endIndex = 65535;*/
    	
	pthread_t threads[cpus]; //decalaring the threads equal to the number of cores
	//threads=pthread_self();
		
	for (int i=0;i<cpus;i++)
	{ 
		pthread_create(&threads[i], NULL, populate,(void *)p[i]);
	}
	for(int i=0;i<cpus;i++)
	{
		pthread_join(threads[i], NULL);
		//pthread_join(threads[i], NULL);
	}
	for(int i=0;i<cpus;i++)
	{
		pthread_create(&threads[i], NULL, calcSqrt,(void *)p[i]);
	}
	for(int i=0;i<cpus;i++)
	{
		pthread_join(threads[i], NULL);
		//pthread_join(threads[i], NULL);
	}
}

void func2(int cpus) //function using affinity.
{
	int half=arraySize/2; //holds the half of the total size of the array
	for (int i=0;i<arraySize;i++) //loop that will initialize the array with 0's
	{
		arr[i]=0;
	}
	indices *p[cpus];
	for(int i=0;i<cpus;i++)
	{
		if(i==0)
		{
			p[i] = (indices  *) malloc(sizeof(indices )); 
			p[i]->startIndex = 0;
			p[i]->endIndex = half-1;
		}
		else
		{
			p[i] = (indices  *) malloc(sizeof(indices )); 
			p[i]->startIndex = half;
			p[i]->endIndex = arraySize;
		}
	}

    	
	pthread_t threads[cpus]; //decalaring the threads equal to the number of cores
	//threads=pthread_self();

	for (int i=0;i<cpus;i++)
	{ 
		pthread_create(&threads[i], NULL, populate,(void *)p[i]);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(i,&cpuset);
		int s = pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset); //using affinity to allocate to differnt cores
	}
	for(int i=0;i<cpus;i++)
	{
		pthread_join(threads[i], NULL);
		//pthread_join(threads[i], NULL);
	}
	for(int i=0;i<cpus;i++)
	{
		pthread_create(&threads[i], NULL, calcSqrt,(void *)p[i]);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(i,&cpuset);
		int s = pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);
	}
	for(int i=0;i<cpus;i++)
	{
		pthread_join(threads[i], NULL);
		//pthread_join(threads[i], NULL);
	}
}

int main( int argc, char **argv ) //Main function
{	
	srand(time(0));
	int cpus=atoi(argv[1]); //taking the number of CPUS from the terminal and storing them in a variable
	printf("Number of CPUs is: %d\n",cpus);
	arr = (double *)malloc(sizeof(double)*arraySize); //dynamically allocating memory to the array
	int operation_count=arraySize*2;//number of op
    struct timeval start, end;

    gettimeofday( &start, NULL );

    func2(cpus); //calling function that will populate and compute with use of affinity

    gettimeofday( &end, NULL );

    /* convert time to Gflop/s */
    printf("The time taken for execution using affinity is: \n");
    double seconds = (end.tv_sec - start.tv_sec) +
            1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflops = 2e-9*operation_count/seconds;
    printf( " Performance in Gflops %.3f Gflop/s\n\n", Gflops );
    
   //func 2 with set affinity
    gettimeofday( &start, NULL );

    func(cpus); //calling function that will populate and compute without use of affinity
 
    gettimeofday( &end, NULL );

    /* convert time to Gflop/s */
    printf("The time taken for execution without using affinity is: \n");
    double seconds2 = (end.tv_sec - start.tv_sec) +
            1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflops2 = 2e-9*operation_count/seconds2;
    printf( " Performance in Gflops %.3f Gflop/s\n\n", Gflops2 );
    
    double mean = (Gflops+Gflops2)/2;
	printf("The mean of the performances in Gflops/s is: %f\n",mean);
	
	double difference=Gflops-Gflops2;
	printf("The difference between the two performances in Gflops/s is: %f\n",difference);
	
	//calculating the sample variance of the performance where number of observations is the number of cores that is 2 x bar is the mean
	//calculated above 
	double sampleVariance=pow((Gflops-mean),2);
	sampleVariance+=pow((Gflops2-mean),2);
	printf("The sample variance of the performance in Gflops/s is: %f\n",sampleVariance);
    
    printf("The resultant array is: \n");
    for(int i=0;i<arraySize;i++)
    {
    	if(i%20==0)
    	{
    		printf("\n");
    	}
    	printf("%f ",arr[i]);
    }
	printf("\n\n");
	
	return 0;
}



