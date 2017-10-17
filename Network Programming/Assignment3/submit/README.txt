- Collaborators: 

Haoming Li (lih14) and Tianxin Zhou (zhout3)

- Some remarks: 

The current program does a MPI_Reduce on every thread when it's 
time to print. Every thread then RE-COUNTs the number of primes 
from the beginning, until it's time to print again. 

We had a more logical version using MPI_Recv and MPI_Send that 
does not re-count from the beginning, but Submitty cannot print 
the last line with it. 

The speed up graph is included in the zip file.
In general, the speed is faster with the bigger ranks.
Ranks	Prime1	5579052	12244884	2104710
