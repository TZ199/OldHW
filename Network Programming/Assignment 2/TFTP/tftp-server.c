/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT 69    // the port users will be connecting to
#define RRQ 01
#define WRQ 02
#define DATA 03
#define ACK 04
#define ERROR 05

#define ERROR_NOT_DEFINED 0
#define ERROR_FILE_NOT_FOUND 1
#define ERROR_ACCESS_VIOLATION 2
#define ERROR_DISK_FULL 3
#define ERROR_ILLEGAL_OPERATION 4
#define ERROR_UNKNOWN_TRANSFER_ID 5
#define ERROR_FILE_EXIST 6
#define ERROR_NO_SUCH_USER 7

#define MAXBUFLEN 100
#define OPCODELEN 2
#define ERRORCODELEN 2
#define DATALEN 512
#define BLOCKNUMLEN 2
#define TIMEOUT_ABORT 10
#define TIMEOUT_RETRANS 1

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handle_RRQ(int sockfd, char* filename, struct sockaddr_in client_addr) {
	socklen_t client_addr_len = sizeof client_addr;
	
	// open the file requested
	char buf[OPCODELEN+BLOCKNUMLEN+DATALEN];
	char ack[OPCODELEN+BLOCKNUMLEN];
	short block_num = 0, opcode = DATA, block_num_ns, opcode_ns = htons(opcode);
	int offset = 0, numbytes, numbytes2;
	short* block_num_bytes = &block_num_ns; 
	short* opcode_bytes = &opcode_ns;
	FILE* f = fopen(filename, "rb");
	if (f == NULL) {
		short opcode_error = htons(ERROR), error_code = htons(ERROR_FILE_NOT_FOUND);
		short* tmp = &opcode_error;
		memcpy(buf, tmp, OPCODELEN);
		tmp = &error_code;
		memcpy(buf+OPCODELEN, tmp, ERRORCODELEN);
		buf[OPCODELEN+ERRORCODELEN] = '\0';
		if (sendto(sockfd, buf, OPCODELEN+ERRORCODELEN+1, 0, (struct sockaddr *)&client_addr, client_addr_len) < 0) {
			perror("server: sendto() failed");
			exit(EXIT_FAILURE);
		}
		printf("server [CHILD %d]: sent to client; opcode %hi, errorcode %hi\n", getpid(), ntohs(opcode_error), ntohs(error_code));
		return;
	}
	fseek(f, 0, SEEK_SET);

	// set timeout
	struct timeval tv;
	tv.tv_sec = TIMEOUT_ABORT;
	tv.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	// reply with data to start a response to RRQ
	do {
		// construct a datagram
		block_num++;
		block_num_ns = htons(block_num);
		memcpy(buf, opcode_bytes, OPCODELEN);
		memcpy(buf+OPCODELEN, block_num_bytes, BLOCKNUMLEN);
		numbytes = fread(buf+OPCODELEN+BLOCKNUMLEN, sizeof(char), DATALEN, f);
		printf("server [CHILD %d]: read %d bytes (%d to %d) from \"%s\"\n", getpid(), numbytes, offset, offset+numbytes-1, filename);
		
		// send out the datagram
		if (sendto(sockfd, buf, OPCODELEN+BLOCKNUMLEN+numbytes, 0, (struct sockaddr *)&client_addr, client_addr_len) < 0) {
			perror("server: sendto() failed");
			exit(EXIT_FAILURE);
		}
		printf("server [CHILD %d]: sent to client; opcode %hi, blocknum %hi\n", getpid(), opcode, block_num);
		offset += DATALEN;

		// block at recvfrom() for ack
		if ((numbytes2 = recvfrom(sockfd, ack, OPCODELEN+BLOCKNUMLEN, 0, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
			perror("server: recvfrom() failed");
			exit(EXIT_FAILURE);
		}
		printf("server [CHILD %d]: received opcode %hi, blocknum %hi\n", getpid(), ntohs(*(short*)ack), ntohs(*(short*)(ack+OPCODELEN)));

		// get ready for next read / sendto / recvfrom
		fseek(f, offset, SEEK_SET);
	} while (numbytes == DATALEN);

	printf("server [CHILD %d]: %d block(s) of data has been sent\n", getpid(), block_num);
	fclose(f);
}

void handle_WRQ(int sockfd, struct sockaddr_in client_addr) {
	socklen_t client_addr_len = sizeof client_addr;
	// reply with ack to start a response to WRQ
	while (1) {

	}
	sendto(sockfd, "GotWRQ\n", 7, 0, (struct sockaddr *)&client_addr, client_addr_len);
}

int main(void) {
	int sockfd, numbytes, pid;
	short opcode;
	struct sockaddr_in server_addr, client_addr;
	char buf[MAXBUFLEN], opcode_bytes[OPCODELEN];
	socklen_t server_addr_len, client_addr_len;

	// Create a socket bind to port 69
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
    perror("server: socket() failed");
    return EXIT_FAILURE;
  }
  server_addr.sin_family = AF_INET;  /* IPv4 */
  server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
  server_addr.sin_port = htons(MYPORT);
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("server: bind() failed");
    return EXIT_FAILURE;
  }
  server_addr_len = sizeof(server_addr);
  client_addr_len = sizeof(client_addr);
  if (getsockname(sockfd, (struct sockaddr *)&server_addr, &server_addr_len) < 0) {
    perror("server: getsockname() failed");
    return EXIT_FAILURE;
  }

	// Main recvfrom() loop
	while (1) {
		printf("server [PARENT %d]: waiting to recvfrom...\n", getpid());
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		//printf("server [PARENT %d]: got packet from %s\n", getpid(), inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
		printf("server [PARENT %d]: packet is %d bytes long\n", getpid(), numbytes);
		buf[numbytes] = '\0';
		printf("server [PARENT %d]: packet contains \"%s\"\n", getpid(), buf);

		pid = fork();
		if (pid < 0) {
			perror("server: fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) { // in child process
			// Check the opcode of initial request
			memcpy(opcode_bytes, &buf, OPCODELEN);
			opcode = ntohs(*(short*)opcode_bytes);
			if (opcode != RRQ && opcode != WRQ) {
				printf("server [CHILD %d]: opcode is %hi. Bad request\n", getpid(), opcode);
				sendto(sockfd, "0504\0", 5, 0, (struct sockaddr *)&client_addr, client_addr_len);
				exit(EXIT_SUCCESS);
			}

			// Create an ephemeral port to handle this client
			int newsockfd;
  		struct sockaddr_in newserver_addr;
  		socklen_t newserver_addr_len;
		  newsockfd = socket( AF_INET, SOCK_DGRAM, 0 );
      if ( newsockfd < 0 ) {
    		perror("server: socket");
    		exit(EXIT_FAILURE);
  		}
		  newserver_addr.sin_family = AF_INET;  /* IPv4 */
  		newserver_addr.sin_addr.s_addr = htonl( INADDR_ANY );
			newserver_addr.sin_port = htons( 0 );
  		if ( bind( newsockfd, (struct sockaddr *) &newserver_addr, sizeof( newserver_addr ) ) < 0 ) {
		    perror( "server: bind" );
    		exit(EXIT_FAILURE);
  		}
		  newserver_addr_len = sizeof( newserver_addr );
  		if ( getsockname( newsockfd, (struct sockaddr *) &newserver_addr, (socklen_t *) &newserver_addr_len ) < 0 ) {
		    perror( "server: getsockname" );
    		exit(EXIT_FAILURE);
  		}
			printf( "server [CHILD %d]: new UDP server at port number %d\n", getpid(), ntohs( newserver_addr.sin_port ) );
			
			// handle this client
			char filename[MAXBUFLEN];
			strcpy(filename, buf+OPCODELEN);
			printf("server [CHILD %d]: filename is \"%s\"\n", getpid(), filename);
			if (opcode == RRQ) {
				handle_RRQ(newsockfd, filename, client_addr);
				printf("server [CHILD %d]: done handling RRQ. Bye!\n", getpid());
			}
			else {
				handle_WRQ(newsockfd, client_addr);
			}

			// done with this client. Exit child process
			close(newsockfd);
			exit(EXIT_SUCCESS);
		}
		else { // in parent process
			// do nothing
		}
	}
	close(sockfd);

	return 0;
}