#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/wait.h>
#include <ctype.h>

/* use -lpthread or -pthread on the gcc line */
/* bash$ gcc -Wall octuplets-threads.c -pthread */
#include <pthread.h>

typedef struct input_info
{
	char* input;
    int index;
    int end;
    signed int *result;
} info;

typedef struct num_info
{
	char oper;
    signed int num;
    signed int *result;
} n_info;

void *process(void *arg);
void *compute(void *arg);

int main(int argc, char* argv[]){
    if ( argc !=2){
		fprintf(stderr, "ERROR: Invalid arguments\nUSAGE: ./a.out <input-file> \n");
		return EXIT_FAILURE;
	}
	FILE *fd;
	char *ch;
	fd= fopen(argv[1],"r");
	if(fd == NULL){
		 perror(argv[1]); 
		 return EXIT_FAILURE;
	} 
	fseek(fd, 0, SEEK_END);
 	int size = ftell(fd);  
    rewind(fd); 
 	ch = (char*) malloc(sizeof(char)*size+1);
	size = fread(ch, sizeof(char), size, fd);
	ch[size] = '\0';
	pthread_t tid;
    
    info temp;
    temp.result = (signed int*)malloc(sizeof(signed int));
    *temp.result = 0;
    temp.input = ch;
    temp.end = size;
    temp.index=1;
	pthread_create(&tid, NULL, process, (void *)&temp);
	pthread_join(tid, NULL);
	signed int result = *temp.result;
	free(temp.result);
    printf("THREAD %u: the final result is %d\n", (unsigned int)pthread_self(), result);
  
    return EXIT_SUCCESS;
}

void *process(void *arg){
	info *data= (info *)arg;
	char operator;
	pthread_t pid;
    int i = data->index;
    int end = data->end;
    char* ch = data->input;
    if (ch[i]=='+'||ch[i]=='-'||ch[i]=='*'||ch[i]=='/'){
		operator = ch[i];
		printf( "THREAD %u: Starting '%c' operation\n",(unsigned int)pthread_self(),operator);
		i++;
	}
    int count = 0;
    while (i < end){
		if (isdigit(ch[i]) ||ch[i] == '-' || ch[i]=='('){
			signed int intermediate =0;
			if (isdigit(ch[i])){
				intermediate = ch[i] - '0';
				char c = data->input[i+1];
				while ( c >='0' && c<='9'){
					intermediate = intermediate*10 + c-'0';
					i++;
					c = ch[i+1];
				}
			}
			else if (ch[i]=='('){
				i++;
				int start = i;
				int count_a = 1;
				while( count_a!=0 && i<end){
					if (ch[i] == '('){
						count_a++;
					}
					else if (ch[i] == ')'){
						count_a--;
					}
					i++;
				}
				pthread_t tid;
				info temp;
		    	temp.result = (int*)malloc(sizeof(int));
	    		*temp.result = 0;
		    	temp.input = ch;
				temp.index = start;
				temp.end = i+1;
				pthread_create(&tid, NULL, process, (void *)&temp);
				count++;
				pthread_join(tid, NULL);
				intermediate = (*temp.result);
				free(temp.result);
			}
			else if (ch[i]=='-'){
				i++;
				intermediate = ch[i] - '0';
				char c = data->input[i+1];
				while ( c >='0' && c<='9'){
					intermediate = intermediate*10 + c-'0';
					i++;
					c = ch[i+1];
				}
				intermediate = intermediate*(-1);
			}
			n_info digit;
			digit.num = intermediate; 
			digit.oper= operator;
			digit.result = data->result;
			pthread_create(&pid, NULL, compute, (void *)&digit);
			pthread_join(pid,NULL);
			count++;
			data->result = digit.result;
		}
		else if (ch[i]==')'){
			if (count <2 ){
				printf("THREAD %u: ERROR: not enough operands\n",(unsigned int)pthread_self());
				pthread_exit(NULL);
			}
			else{
				printf("THREAD %u: Ended '%c' operation with result '%d'\n",(unsigned int)pthread_self(),operator,*data->result);
				pthread_exit(NULL);
			}		
		}
		i++;
    }
    pthread_exit(NULL);
}

void *compute(void* arg){
	n_info *data= (n_info *)arg;
	char operat = data->oper;
	signed int temp = data->num;
	if (operat == '+'){
		*data->result = (*data->result) + temp;
		printf("THREAD %u: Adding '%i'\n",(unsigned int)pthread_self(),temp);
	}
	else if (operat =='-'){
		if (*data->result==0){
			*data->result = temp;
			printf("THREAD %u: Starting with the first operand '%i'\n",(unsigned int)pthread_self(),temp);
		}
		else{
			*data->result = (*data->result) - temp;
			printf("THREAD %u: Subtracting '%d'\n",(unsigned int)pthread_self(),temp);
		}
	}
	else if (operat =='*'){
		if (*data->result==0){
			*data->result = 1;
		}
		*data->result = (*data->result) * temp;
		printf("THREAD %u: Mutiplying '%d'\n",(unsigned int)pthread_self(),temp);
	}
	else if (operat =='/'){
		if (*data->result==0){
			*data->result = temp;
			printf("THREAD %u: Starting with the first operand '%d'\n",(unsigned int)pthread_self(),temp);
		}
		else{
			*data->result = *data->result /temp;
			printf("THREAD %u: Dividing '%d'\n",(unsigned int)pthread_self(),temp);
		}
	}
    pthread_exit(NULL);
}
    
    