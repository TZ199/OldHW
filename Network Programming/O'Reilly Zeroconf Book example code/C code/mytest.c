// Simple example (single event source only)
// of how to handle DNSServiceDiscovery events using a select() loop

#include <dns_sd.h>
#include <stdio.h>			// For stdout, stderr
#include <string.h>			// For strlen(), strcpy(), bzero()
#include <errno.h>          // For errno, EINTR
#include <time.h>

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

// Note: the select() implementation on Windows (Winsock2)
//fails with any timeout much larger than this
#define LONG_TIME 100000000

static volatile int stopNow = 0;
static volatile int timeOut = LONG_TIME;

void HandleEvents(DNSServiceRef serviceRef)
{

    int sockfd, new, maxfd, on = 1, nready, i;
    
    struct addrinfo *res0, *res, hints;
    
    char buffer[BUFSIZ];
    
    fd_set master, readfds;
    
    int error;
    
    ssize_t nbytes;
    
    (void)memset(&hints, '\0', sizeof(struct addrinfo));
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    
    if (0 != (error = getaddrinfo(NULL, PORT, &hints, &res0)))
        errx(EXIT_FAILURE, "%s", gai_strerror(error));
    
    for (res = res0; res; res = res->ai_next)
    {
        if (-1 == (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)))
        {
            perror("socket()");
            continue;
        }
        
        if (-1 == (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(int))))
        {
            perror("setsockopt()");
            continue;
        }
        
        if (-1 == (bind(sockfd, res->ai_addr, res->ai_addrlen)))
        {
            perror("bind");
            continue;
        }
        
        break;
        
    }
    
    if (-1 == sockfd)
        exit(EXIT_FAILURE);
    
    freeaddrinfo(res0);
    
    if (-1 == (listen(sockfd, 32)))
        die("listen()");
    
    if (-1 == (fcntl(sockfd, F_SETFD, O_NONBLOCK)))
        die("fcntl()");
    
    FD_ZERO(&master);
    FD_ZERO(&readfds);
    
    FD_SET(sockfd, &master);
    
    maxfd = sockfd;
    int count = 0;
    while (1)
    {
        memcpy(&readfds, &master, sizeof(master));
        
        (void)printf("running select()\n");
        
        if (-1 == (nready = select(maxfd+1, &readfds, NULL, NULL, NULL)))
            die("select()");
        
        (void)printf("Number of ready descriptor: %d\n", nready);
        
        for (i=0; i<=maxfd && nready>0; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                nready--;
                
                if (i == sockfd)
                {
                    (void)printf("Trying to accept() new connection(s)\n");
                    
                    if (-1 == (new = accept(sockfd, NULL, NULL)))
                    {
                        if (EWOULDBLOCK != errno)
                            die("accept()");
                        
                        break;
                    }
                    
                    else
                    {
                        
                        if (-1 == (fcntl(new, F_SETFD, O_NONBLOCK)))
                            die("fcntl()");
                        
                        FD_SET(new, &master);
                        
                        if (maxfd < new)
                            maxfd = new;
                    }
                }
                
                else
                {
                    (void)printf("recv() data from one of descriptors(s)\n");
                    
                    nbytes = recv(i, buffer, sizeof(buffer), 0);
                    if (nbytes <= 0)
                    {
                        if (EWOULDBLOCK != errno)
                            die("recv()");
                        
                        break;
                    }
                    
                    buffer[nbytes] = '\0';
                    printf("%s", buffer);
                    char *ch;
                    ch = strtok(buffer, " ");
                    printf("%s\n", ch);
                    ch = strtok(NULL, " ");
                    printf("check %s\n",ch);
                    int num = atoi(ch);
                    printf("CHECK%d\n", num);
                    int answer = 50;
                    if(num > answer)
                    {
                        send(i,"too big\n",7,0);
                        count++;
                    }
                    else if(num < answer)
                    {
                        send(i,"too small\n",10,0);
                        count++;
                    }
                    else
                    {
                        send(i,"Great guess\n",12,0   );
                        close(i);
                        FD_CLR(i, &master);
                    }
                    (void)printf("%zi bytes received.\n", nbytes);
                    
                    
                    
                }
            }
            
        }
        
    }
}
