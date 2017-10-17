// DNSServiceRegister example

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
extern void HandleEvents(DNSServiceRef,int);

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
