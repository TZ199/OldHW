
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

int help (char temp[],int index, int end, char a[])
{    
     int digits;
     int finaldigits[16];
     int answer = 0;
     int lindex = end;
     int numleft = 0;
     int numright = 0;
     int i = index;
     char operation;
    while(i<end)
    {
          if (temp[i] == '(') 
          {
               numleft++;
          }
          else if (temp[i] ==')')
          { 
               numright++;
          }
          if (numleft == numright)
          {
               break;
          }
          i++;
     }
     int charlen = i-index+2;
     char cpytemp[charlen];
     memcpy(cpytemp,&temp[index], (charlen-1));
     cpytemp[charlen-1] = '\0';
     printf("PID %d: My expression is \"%s\"\n",getpid(),cpytemp);
     fflush(NULL);
     strcpy(a,cpytemp);
     //finished the first prinf, then we need to find the operation.
     
     //Begin operation printf
     int cpyindex = index + 1;
     if (temp[cpyindex]=='*' ||temp[cpyindex]=='/' ||temp[cpyindex]=='+' ||temp[cpyindex]=='-' )
     {
          operation = temp[cpyindex];
          printf("PID %d: Starting \"%c\" operation\n",getpid(),temp[cpyindex]);
          fflush(NULL);
     }

     //finished the operationion printf
     
     
     //Starting main loop for children
     int newindex = cpyindex + 1; // Then the first index should point to a digit.
     int j = newindex;
     int digitnum = 0;
     
     while(j<lindex)
     {
          int * p = (int *)calloc( 2, sizeof( int ) );
          int rc = pipe( p );

          if ( rc == -1 )
          {
            perror( "pipe() failed" );
            return EXIT_FAILURE;
          }
          

          //detect negative numers.
          if (isdigit (temp[j]) || temp[j] == '-' )
          {    

               char digittemp[16];
               int digittemp2 = 0;
               int k=0;
               bool ifneg = false;

               if(temp[j] == '-')
               {

                    ifneg = true;
                    j++;
               }
               while(isdigit(temp[j]) && !isspace(temp[j]))
               {

                    digittemp[k] = temp[j];

                    j++;
                    k++;
               }
               digittemp[k] = '\0';
               digittemp2 = atoi(digittemp);
               if (ifneg)
               {
                    digittemp2 = digittemp2 *(-1);
               }
               j--;
               digits = digittemp2;
          }
          //now we can get digit in digits
          
          //return
          if(temp[j] == ')')
          {
               if(digitnum < 2)
               {
                    printf("PID %d: ERROR: not enough operands; exiting", getpid());
                    exit(-1);
               }
               if (operation == '+')
               {
                    int l = 1;
                    answer = finaldigits[0];
                    while(l<digitnum)
                    {
                         answer = answer + finaldigits[l];
                         l++;
                    }
               }

               else if (operation == '-')
               {
                    int l = 1;
                    answer = finaldigits[0];
                    while(l<digitnum)
                    {
                         answer = answer - finaldigits[l];
                         l++;
                    }
               }
               
               else if (operation == '*')
               {
                    int l = 1;
                    answer = finaldigits[0];
                    while(l<digitnum)
                    {
                         answer = answer * finaldigits[l];
                         l++;
                    }
               }          
               
               else if (operation == '/')
               {
                    int l = 1;
                    answer = finaldigits[0];
                    while(l<digitnum)
                    {
                         if(finaldigits[l] == 0)
                         {
                              printf("PID %d: ERROR: division by zero is not allowed; exiting\n",getpid());
                              exit(-1);
                         }
                         else
                         {
                              answer = answer / finaldigits[l];
                         }
                         l++;
                    }
               }
               return answer;
               
               
          }
          //Start fork()
          if(temp[j] == '(' || isdigit(temp[j]))
          {
               int pid; 
               fflush(NULL);
               pid = fork();
               // printf("fork once\n");
               //fork failed
               if ( pid == -1 )
               {
                    perror( "fork() failed" );
                    return EXIT_FAILURE;
               }
               
               if ( pid == 0 )
               {
                    if(isdigit(temp[j]))
                    {
                         close(p[0]);
                         p[0] = -1;     
                         write(p[1], &digits,sizeof(int));
                         printf("PID %d: My expression is \"%d\"\n",getpid(),digits);
                         fflush(NULL);
                         printf("PID %d: Sending \"%d\" on pipe to parent\n",getpid(),digits);
                         fflush(NULL);
                         exit( EXIT_SUCCESS );
                    }
                    
                    //if we find another '('
                    else if(temp[j] == '(')
                    {
                         if (pid == 0)
                         {
                              p[0] = -1;
                              int childanswer = help(temp,j,lindex,a);
                              close(p[0]);
                              write(p[1], &childanswer,sizeof(int));
                              // printf("; sending \"%d\" on pipe to parent",childanswer);
                              printf("PID %d: Processed \"%s\"; sending \"%d\" on pipe to parent\n",getpid(),a,childanswer);
                              fflush(NULL);
                         }
                    }
               exit(0);
                    
               }
               
               
               
          else if(pid>0)
               {
                    if(temp[j] =='(')
                    {
                         while(temp[j] != ')' ){
                              j++;
                              
                         }
                    }
                    close(p[1]);
                    p[1] = -1;
                    int status;
                    pid = wait( &status );
                    if ( WIFSIGNALED( status ) )   /* core dump or kill or kill -9 */
                    {
                         printf( "PID %d: child <%d> terminated abnormally\n",getpid(),(int)pid );
                    }
                    else if ( WIFEXITED( status ) )   /* child called return or exit() */
                    {
                         int rc = WEXITSTATUS( status );
                         if (rc!=0)
                         printf( "PID %d: child <%d> terminated with nonzero exit status <%d>",getpid(),(int)pid,rc );
                    }
                    read(p[0],&finaldigits[digitnum],sizeof(int));
                    digitnum++;
                    // printf("final digit %d\n,digitnum = %d\n",finaldigits[digitnum-1],digitnum);
                    

                    
               }

          }

          j++;
     }
     

     
     return answer;
}

int main( int argc, char * argv[] )
{
     if(argc != 2)
     {
          fprintf(stderr, "ERROR: Invalid arguments \nUSAGE: ./a.out <input-file>\n" );
          return EXIT_FAILURE;
     }
     FILE * file;
     file = fopen (argv[1], "r");
     if (file == NULL)
          {
               perror("Error: ");
               return EXIT_FAILURE;
          }
     char temp[60];
     int answer = 0; 
     int index  = 0;

     while (fgets(temp, 60, file) != NULL)
     {
          if (temp[0] == '#')
          {
               continue;
          }
          else if(temp[0] == '(')
          {
               int end = strlen(temp);
               int numleft = 0;
               int numright = 0;
               int i = index;
               while(i<end)
               {
                    if (temp[i] == '(') 
                    {
                         numleft++;
                    }
                    else if (temp[i] ==')')
                    { 
                         numright++;
                    }
                    if (numleft == numright)
                    {
                         break;
                    }
                    i++;
               }
               int charlen = i-index+2;
               char cpytemp[charlen];
               memcpy(cpytemp,&temp[index], (charlen-1));
               cpytemp[charlen-1] = '\0';
               
               
               
               
               
               char a[16];
               answer = help(temp,index,end,a);
               printf("PID %d: Processed \"%s\"; final answer is \"%d\"",getpid(),cpytemp,answer);
          }
          
          

     }
     return 0;
     
}