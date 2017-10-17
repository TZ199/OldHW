/* Number of words that caused out-of-memory case: 8560142 */
// I tested my code on c9.io and the ram is 256mb. When I tried exceed 8560142 words, my workspace will crash XD.

//operating system homework 1
//by Tianxin Zhou


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



void erase(char *str)
{
    char *old = str;
    char * ne= str;
    while(*old)
    {
    	if (! ispunct((unsigned char)*old)) 
    	{
    		*ne = *old;
    		ne++;
    	}
    	old++;
    }
    *ne = 0;
}

int main( int argc, char * argv[] )
{
     

     //if we put invalid arguments.
     if(argc != 3)
     {
          fprintf(stderr, "Invalid input\n" );
          exit(1);
     }
     
     else{
          char *filename = argv[1];
          char * word = argv[2];
          FILE * file;
          file = fopen (filename, "r");
          //if the file isnot we want.
          if (file == NULL)
          {
               perror("Error: ");
               return(-1);
          }
          //begin the whole work
          else
          {    
               char temp[16];
               char **arra;
               int current=0;
               int siz=8;
               while (fscanf(file, " %s", temp) != EOF) 
               {
                    //initial the memory in array
                    if(current == 0 && siz == 8)
                    {
                         arra = (char**)calloc(8,sizeof(char*));
                         printf("Allocated initial array of 8 character pointers.\n");
                    }
                    //resize the memory in array
                    else if(current == siz)
                    {
                         siz = 2*siz;
                         printf("Re-allocated array of %d character pointers.\n", siz);
                         arra = (char**)realloc(arra, siz*sizeof(char*));
                    }
                    //initial the memory of words
                    arra[current] = (char*)malloc(16*sizeof(char));
                    erase(temp);
                    strcpy(arra[current],temp);
                    current++;
                    
               }
               printf("All done (successfully read %d words).\n",current);
               printf("Words containing substring \"%s\" are:\n", word);
               
              //print words contain hi
               int i = 0;
               while(i<current){
                    if(strstr(arra[i],word) != NULL)
                    {
                         printf("%s\n", arra[i]);
                    }
                    i++;
               }
               
               //freev memories
               for(i=0;i<current;i++)
               {
                    free(arra[i]);
               }
               free(arra);
          }
     
          fclose(file);

     }
     
     
     return EXIT_SUCCESS;
     
    
     
     
}
