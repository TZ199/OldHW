// Simple example (single event source only)
// of how to handle DNSServiceDiscovery events using a select() loop

#include <dns_sd.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <errno.h>          // For errno, EINTR
#include <time.h>
#include <err.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/select.h>
#include <fcntl.h>
#include <math.h>


#ifdef _WIN32
#include <process.h>
typedef	int	pid_t;
#define	getpid	_getpid
#define	strcasecmp	_stricmp
#define snprintf _snprintf
#else
#include <sys/time.h>		// For struct timeval
#include <unistd.h>         // For getopt() and optind
#include <arpa/inet.h>		// For inet_addr()


#endif

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
		int result = select(maxfd+1, &readfds, NULL, NULL, NULL);
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
                printf("accept..." );
                if(listens <0)
                {
            	   perror("Problem in accept");
            	   exit(-1);
                }
                FD_SET(listens, &master);
            }
            
            //If we get the listen socket numebr
            if(maxfd < listens)
            {
            	maxfd = listens;
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
