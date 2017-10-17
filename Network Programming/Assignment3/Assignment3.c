//Assignment3
//Team: Haoming Li() and Tianxin Zhou(zhout3@rpi.edu)

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>

int end_now = 0;
#define LIMIT 4294967291

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
    int ntasks, rank, mystart, stride, count, global_count, n,print_number;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    mystart = (rank*2)+1;
    stride = ntasks*2;
     
    print_number = 10;
    signal(SIGUSR1, sig_handler);
    if(rank == 0)
    { 
        printf("                N           Primes\n");
    }
    while(print_number < LIMIT)
    {    
        
        count = 0;
        for (n = mystart; n <= print_number; n += stride) 
        {
 
            if (isprime(n)) 
            {
                count++;
                
            }
            if (end_now == 1) 
            {
                print_number = n;
                break;
            }
  
        }
        MPI_Reduce(&count,&global_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        if(end_now == 1 && rank ==0)
        {
            printf("<Signal received>\n");
            printf("%17d%17d\n", print_number,  global_count+1);
            
        }
        if(end_now == 1)
            break;
        if(rank == 0)
            printf("%17d%17d\n", print_number,  global_count+1);

        print_number = print_number*10;
        
    }

    MPI_Finalize();    
    return 0;
}
