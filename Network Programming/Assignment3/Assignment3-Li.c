#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

int end_now = 0;
#define LIMIT 4294967291 // max 32-bit prime
#define maxof(a,b) ((a) > (b) ? (a) : (b))
#define minof(a,b) ((a) < (b) ? (a) : (b))

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
    int ntasks, rank, mystart, stride, count, global_count, n, i;
    int interval;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    mystart = (rank*2)+1;
    stride = ntasks*2;
    count = 0; 
    
    signal(SIGUSR1, sig_handler);

    printf("           N\t      Primes\n");
    for (n = mystart; n <= LIMIT; n += stride) {
        if (end_now == 1) break;
        if (isprime(n)) {
            count++;
        }
        interval = pow(10,ceil(log10(n)));
        if (n == 1) interval = 10;
        if ((n+stride) >= interval) {
            if (rank != 0) { // non-root: send to root
                MPI_Send(&count,1,MPI_INT,0,interval,MPI_COMM_WORLD);
            }
            else { // root: add up current counts
                global_count = count;
                int tmp = 0, tmp2 = minof(ntasks, interval/2);
                for (i = 1; i < tmp2; ++i) {
                    MPI_Recv(&tmp,1,MPI_INT,i,interval,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                    global_count += tmp;
                }
                global_count += 1; // add 2
                printf("%12d\t%12d\n", interval, global_count);
            }
        }
    }
    MPI_Reduce(&count,&global_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Finalize();

    // root print end result
    if (rank == 0) {
        global_count += 1; // add 2
        printf("%12d\t%12d\n", n, global_count);
    }
    
    return 0;
}
