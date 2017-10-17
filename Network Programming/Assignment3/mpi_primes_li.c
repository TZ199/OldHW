#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

int end_now = 0;
#define LIMIT 500
#define maxof(a,b) ((a) > (b) ? (a) : (b))

void sig_handler(int signo) {
    if (signo == SIGUSR1) {
        end_now = 1;
    }
}

int isprime(int n) {
    if (n == 1 || n == 0) return 0;
    if (n == 2 || n == 3) return 1;
    int i, squareroot;
    squareroot = (int) sqrt(n);
    for (i = 3; i <= squareroot; i += 2) {
        if ((n%i) == 0) return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    int ntasks, rank, mystart, stride, count, global_count, n, max, global_max,printnum;
    double start_time,end_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start_time = MPI_Wtime(); 
    mystart = (rank*2)+1;
    stride = ntasks*2;
    count = 0; 
    max = 0;
    printnum =10;
    
    signal(SIGUSR1, sig_handler);



    
    if (rank == 0) {
        for (n = mystart; n <= LIMIT; n += stride) 
        {
        if (end_now == 1) {
            break;
        }
        if (isprime(n)) {
            count++;
            max = n;
        }

        }
        MPI_Reduce(&count,&global_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        MPI_Reduce(&max,&global_max,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
        end_time=MPI_Wtime();
        MPI_Finalize();
        global_max = maxof(2, global_max);
        
        printf("num prime: %d; max prime %d\n", global_count, global_max);
    }
    if(rank > 0)
    {
        for (n = mystart; n <= LIMIT; n += stride) 
        {
        if (end_now == 1) {
            break;
        }
        if (isprime(n)) {
            count++;
            max = n;
        }

        }
        MPI_Reduce(&count,&global_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        MPI_Reduce(&max,&global_max,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
        
        global_max = maxof(2, global_max);
        if(count == printnum)
        {
            printf("num prime: %d; max prime %d\n", global_count, global_max);
            printnum = 10*printnum;
        }
    }
    MPI_Finalize();





    
    return 0;
}
