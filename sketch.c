#include <sys/time.h>
#include <time.h>
#include <stdio.h>
int main( int argc, char **argv ) {

    int operation_count=0;//number of op
    struct timeval start, end;

    gettimeofday( &start, NULL );

    //call your function here

    gettimeofday( &end, NULL );

    /* convert time to Gflop/s */
    double seconds = (end.tv_sec - start.tv_sec) +
            1.0e-6 * (end.tv_usec - start.tv_usec);
    double Gflops = 2e-9*operation_count/seconds;
    printf( " Performance in Gflops %.3f Gflop/s\n", Gflops );
    }