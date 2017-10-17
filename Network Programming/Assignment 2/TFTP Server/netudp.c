/* netudp.c 
   Copyright (c) 2013 James Northway
*/

#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "tftp.h"

struct sockaddr_in ftCli;
struct sockaddr_in ftSrv;
struct sockaddr_in xfCli;
struct sockaddr_in xfSrv;

void netudp_bind_server(int *ssock, char *port){
    *ssock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (*ssock == -1) {
        printf("Socket creation failed: %s\n",strerror(errno));
        exit(1);
    } 
   
    ftSrv.sin_family = AF_INET;
    ftSrv.sin_addr.s_addr = INADDR_ANY;
    ftSrv.sin_port = htons(atoi(port));
   
    if ((bind(*ssock, (struct sockaddr *)&ftSrv, sizeof(ftSrv))) == -1){
        printf("Could not bind server to socket: %s\n",strerror(errno));
        exit(1);
    }
    printf("TFTP Server [PID %d]: New socket bind to port %d\n", getpid(), ntohs(ftSrv.sin_port));
    
}

void netudp_rebind_server(int *ssock){
    *ssock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (*ssock == -1) {
        printf("Socket creation failed: %s\n",strerror(errno));
        exit(1);
    } 
   
    xfSrv.sin_family = AF_INET;
    xfSrv.sin_addr.s_addr = INADDR_ANY;
    xfSrv.sin_port = 0;
   
    if ((bind(*ssock, (struct sockaddr *)&xfSrv, sizeof(xfSrv))) == -1){
        printf("Could not bind server to socket: %s\n",strerror(errno));
        exit(1);
    }
    
}

void netudp_send_packet(int *ssock, struct sockaddr* dest, 
        packetbuffer_t ** packet_out, int * length){      
    int bytes = sendto(*ssock, *packet_out, *length, 0,
            (struct sockaddr *)dest, sizeof(*dest));
            
    if (bytes == -1){        
        printf("Could not send packet via socket: %s\n",strerror(errno));
        exit(1);
    }
    printf("TFTP Server [PID %d]: sendto() %s port %d\n", getpid(), inet_ntoa(((struct sockaddr_in *)dest)->sin_addr), ntohs(((struct sockaddr_in *)dest)->sin_port));
      
}