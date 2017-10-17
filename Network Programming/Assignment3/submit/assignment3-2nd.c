// Assignment3
// Team: Haoming Li (lih14) and Tianxin Zhou (zhout3)

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>

#define LIMIT 4294967291 // largest 32-bit prime
int end_now = 0;

void sig_handler(int signo) {
    if (signo == SIGUSR1) end_now = 1;
}

//The function to check if the number is prime number
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
    int ntasks, rank, mystart, stride, count, global_count, n, print_number;
    //Begin the MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    mystart = (rank*2)+1;
    stride = ntasks*2;
    print_number = 10;
    //The signal handler
    signal(SIGUSR1, sig_handler);
    //Print the title.
    if (rank == 0)
        printf("                N           Primes\n");
    //Loop untill the print_number >= limit
    while (print_number < LIMIT) {    
        count = 0;
        for (n = mystart; n <= print_number; n += stride) {
            if (isprime(n)) count++;
            if (end_now == 1) {
                print_number = n;
                break;
            }
        }
        //Add the count to global_count
        MPI_Reduce(&count,&global_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        if(rank==0 && end_now)
        {
            printf("<Signal received>\n");
        }
        if (rank == 0)
            printf("%17d%17d\n", print_number, global_count+1);
        if (end_now) break;
        print_number *= 10;
    }
    //End the MPI.
    MPI_Finalize();    
    return 0;
}
