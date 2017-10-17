/* fork.c */

/* example showing how to create a new (child) process */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t pid;   /* process id (unsigned int) */

  pid = fork();   /* create a child process */

  /* this fork() system call, if successful, will
      return twice.....once in the parent process,
       and once in the child process */

  /* from "man fork": " On success, the PID of the
      child process is returned in the parent, and
       0 is returned in the child. */

  if ( pid == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }

  printf( "Yay! fork() worked\n" );

  if ( pid == 0 )   /* note that 0 is the mechanism
                        for us to know we are in the child */
  {
    printf( "CHILD: happy birthday to myself\n" );

#if 0
sleep( 5 );
#endif

    return 12;

#if 0
    int * xyz;
    *xyz = 1234;
#endif
  }
  else /* pid > 0 */
  {
    printf( "PARENT: my child's pid is %d\n", pid );

#if 0
sleep( 15 );
#endif

    /* wait for the child process to terminate */
    int status;
    pid_t child_pid = wait( &status );
                   /* wait() blocks indefinitely until
                       exactly one child process terminates */
#if 0
printf( "PARENT: status is %d\n", status );
#endif

    printf( "PARENT: child %d terminated...", child_pid );

    if ( WIFSIGNALED( status ) )   /* core dump or kill or kill -9 */
    {
      printf( "abnormally\n" );
    }
    else if ( WIFEXITED( status ) )   /* child called return or exit() */
    {
      int child_return_value = WEXITSTATUS( status );
      printf( "successfully with exit status %d\n", child_return_value );
    }

#if 0
sleep( 5 );
#endif

  }


  /* things that get duplicated from parent to child process:
     -- program counter (PC)
     -- variables and their values (anything in memory, including
                                     the stack and the heap)
     -- file descriptors (fd table)
     -- environment (env variables)
     -- process priority
     -- controlling terminal
     -- current working directory
     -- signal handlers
     -- etc.

     things that do NOT get duplicated from parent to child process:
     -- process id (pid)
     -- parent process id (ppid)
     -- list of child processes (NULL in the child)
     -- data on resource allocation (CPU usage, total runtime)
     -- memory location
     -- pending signals
     -- etc.
   */

  return EXIT_SUCCESS;
}