//Assignment 1 
//Group of Tianxin Zhou and Haoming Li.
//Service name:Guess
//Compile command: gcc hw1_main.c hw1_handle.c
//Running command: ./a.out
//On another terminal: dns-sd -L "Guess" _gtn._tcp local
//telnet localhost portnumber.
//Resources consulted: test2.c, test5.c, select.c, code chunk on piazza.
//Tianxin Zhou did the handleevents part and details implementation
//Haoming Li did the main function part and design workflow.
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <dns_sd.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <stdlib.h>
#include <dns_sd.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
void HandleEvents(DNSServiceRef serviceRef, int socketfd)
{
	//set the true answer
    srand(time(NULL));   // should only be called once
    int answer = rand()%100 + 1;
    printf("Correct answer = %d\n", answer);

	float count = 0;
	int maxfd, listens;
	maxfd = socketfd;
	char buffer[BUFSIZ];
	fd_set master, readfds;
	int dns_sd_fd = DNSServiceRefSockFD(serviceRef);
    //set the fd's to sero and set the sockfd to master, the dns_sd_fd to readfds.
	FD_ZERO(&master);
    FD_ZERO(&readfds);
    FD_SET(socketfd, &master);
	//set the dns_sd_fd
	FD_SET(dns_sd_fd, &master);

	
	//set the stop argument
	int nfds = dns_sd_fd + 1;
	
	while (1)
	{
		
        
        int temp = sizeof(struct sockaddr_in);
		struct sockaddr_in temp2;
		readfds = master;
		int result = select(maxfd+1, &readfds, (fd_set*)NULL, (fd_set*)NULL, NULL);
		if (result > 0)
			{
			DNSServiceErrorType err = kDNSServiceErr_NoError;
			if (FD_ISSET(dns_sd_fd, &readfds))
                  err = DNSServiceProcessResult(serviceRef);
			if (err) { fprintf(stderr,
                           "DNSServiceProcessResult returned %d\n", err);
                        break; }
            //Accept socket here
            if(FD_ISSET(socketfd, &readfds))
            {
                listens = accept(socketfd,(struct sockaddr *) &temp2, (socklen_t *)&temp);
                printf("accept successfully" );
                if(listens  == (-1))
                {
            	   perror("Problem in accept");
            	   exit(-1);
                }
                else
                    {
                        FD_SET(listens, &master);
                        if(maxfd < listens)
                        {
                            maxfd = listens;
                        }
                    }
            }

            //The processing part.
            int nbytes = recv(maxfd, buffer, sizeof(buffer), 0);
            if (nbytes <= 0)
            {
                if (EWOULDBLOCK != errno)
                    perror("recv()");
                break;
            }
            //Processing the input command.
            buffer[nbytes] = '\0';
            char *ch;
            ch = strtok(buffer, " ");
            if(strcmp(ch,"guess")!=0 || ch == NULL)
            {
                send(maxfd,"???\n", 4,0);
            }
            //Process the number and begin the guess.
            else
                {
                    ch = strtok(NULL, " ");
                    if(ch == NULL || atoi(ch) ==0)
                    {
                        send(maxfd,"???\n", 4,0);
                    }
                    else
                    {   
                        printf("check %s\n",ch);
                        int num = atoi(ch);
                        printf("CHECK%d\n", num);
                        if(num > answer)
                        {
                            send(maxfd,"SMALLER\n",8,0);
                        	count++;
                        }
                        else if(num < answer)
                        {
                            send(maxfd,"BIGGER\n",7,0);
                            count++;
                        }
                        else
                        {
            	            printf("xxx is %f\n",log(100.0)/log(2.0));
                            count++;
                            if(count < (log(100.0)/log(2.0))-1)

                                send(maxfd,"Great guess\n",12,0   );
                            else if(count > (log(100.0)/log(2.0)) +1)
                                send(maxfd, "BETTER LUCK NEXT TIME\n",22,0);
                            else
                                send(maxfd, "AVERAGE\n",8,0);
            	            close(maxfd);
            	            FD_CLR(maxfd, &master);
                            count = 0;

                        }
                        (void)printf("%zi bytes received.\n", nbytes);
                        
            			}
                    }
                }		
		
	}
}

static void
MyRegisterCallBack(DNSServiceRef service,
				   DNSServiceFlags flags,
				   DNSServiceErrorType errorCode,
				   const char * name,
				   const char * type,
				   const char * domain,
				   void * context)
	{
	#pragma unused(flags)
	#pragma unused(context)

	if (errorCode != kDNSServiceErr_NoError)
		fprintf(stderr, "MyRegisterCallBack returned %d\n", errorCode);
	else
		printf("%-15s %s.%s%s\n","REGISTER", name, type, domain);
	}

static DNSServiceErrorType MyDNSServiceRegister(struct sockaddr_in temp_addr,int listen_sock)
	{
	DNSServiceErrorType error;
	DNSServiceRef serviceRef;
	
	error = DNSServiceRegister(&serviceRef,
								0,                  // no flags
								0,                  // all network interfaces
								"Guess",  // name
								"_gtn._tcp",       // service type
								"",                 // register in default domain(s)
								NULL,               // use default host name
								temp_addr.sin_port,        // port number
								0,                  // length of TXT record
								NULL,               // no TXT record
								MyRegisterCallBack, // call back function
								NULL);              // no context
	
	if (error == kDNSServiceErr_NoError)
		{
		HandleEvents(serviceRef,listen_sock);
		DNSServiceRefDeallocate(serviceRef);
		}
	
	return error;
	}

int main (int argc, const char * argv[])
	{
	int listen_sock,sockfd,local_port;
	struct sockaddr_in server_addr,temp_addr;
	listen_sock = socket(AF_INET, SOCK_STREAM,0);
	bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(0);

    bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    socklen_t sz = sizeof(temp_addr);
    int ret = getsockname(listen_sock, (struct sockaddr*)&temp_addr, &sz);
    if (ret < 0) {
        printf("Problem!\n");
        exit(1);
    }

    local_port = ntohs(temp_addr.sin_port);
    printf("Port is %d\n", local_port);
    listen(listen_sock,1);

	DNSServiceErrorType error = MyDNSServiceRegister(temp_addr,listen_sock);
	fprintf(stderr, "DNSServiceDiscovery returned %d\n", error);
	return 0;
	}
