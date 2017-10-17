/* 
 * A TFTP Server (RFC1350)
 * Haoming Li (lih14@rpi.edu) & Tianxin Zhou (zhout3@rpi.edu) 
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
#include <poll.h>

#define TFTP_OPCODE_RRQ   1
#define IS_RRQ(op)   ((op) == TFTP_OPCODE_RRQ) /*Evaluates as true or false*/
#define TFTP_OPCODE_WRQ   2
#define IS_WRQ(op)   ((op) == TFTP_OPCODE_WRQ)
#define TFTP_OPCODE_DATA  3
#define IS_DATA(op)  ((op) == TFTP_OPCODE_DATA)
#define TFTP_OPCODE_ACK   4
#define IS_ACK(op)   ((op) == TFTP_OPCODE_ACK)
#define TFTP_OPCODE_ERROR 5
#define IS_ERROR(op) ((op) == TFTP_OPCODE_ERROR)

#define TFTP_ERR_NOT_DEFINED   0
#define TFTP_ERR_NOT_FOUND     1
#define TFTP_ERR_ACCESS_DENIED 2
#define TFTP_ERR_DISK_FULL     3
#define TFTP_ERR_UNKNOWN_TID   4
#define TFTP_ERR_ILLEGAL_OP    5
#define TFTP_ERR_FILE_EXISTS   6
#define TFTP_ERR_NO_SUCH_USER  7

#define TFTP_BLOCKSIZE        512
#define TFTP_MAX_MSGSIZE      (4 + TFTP_BLOCKSIZE)
#define TFTP_TIMEOUT_ABORT    10
#define TFTP_TIMEOUT_RETRY    1

typedef struct tftp_packet{
  char filename[TFTP_MAX_MSGSIZE];
  short opcode;
  char mode[TFTP_MAX_MSGSIZE];
  char data[TFTP_BLOCKSIZE];
  int data_length;
  short blocknum;
  short ecode;
  char estring[TFTP_MAX_MSGSIZE];
  int estring_length;
} packet_t;

struct sockaddr_in tftpSrv;
struct sockaddr_in tftpSrv2;
struct sockaddr_in tftpCli;
socklen_t addrlen;
int sockfd;
int sockfd2;
char* port;

struct tftp_packet packet;
char packet_in_buffer[TFTP_MAX_MSGSIZE];
char *packet_in = packet_in_buffer;
int packet_in_length;
char *packet_out;
int packet_out_length;


void packet_extract_opcode(packet_t *packet, const char *pbuf){ 
  memmove(&packet->opcode,pbuf,sizeof(short));
  packet->opcode = ntohs(packet->opcode);
}

void packet_parse_rq(packet_t *packet, const char *pbuf){  
  strcpy(packet->filename, pbuf+sizeof(short));
  strcpy(packet->mode, pbuf+sizeof(short)+strlen(packet->filename)+1);
}

void packet_parse_data(packet_t *packet, const char *pbuf, int * data_length) {       
  memcpy(&packet->blocknum,pbuf+sizeof(short),sizeof(short));
  packet->blocknum = ntohs(packet->blocknum);
  packet->data_length = *data_length - sizeof(short) - sizeof(short);
  memcpy(packet->data, pbuf+sizeof(short)+sizeof(short), packet->data_length);
}

void packet_parse_ack(packet_t *packet, const char *pbuf){
  memcpy(&packet->blocknum,pbuf+sizeof(short),sizeof(short));
  packet->blocknum = ntohs(packet->blocknum);
}

void packet_parse_error(packet_t *packet, const char *pbuf, int * data_length){       
  memcpy(&packet->ecode,pbuf+sizeof(short),sizeof(short));
  packet->ecode = ntohs(packet->ecode);
  packet->estring_length = *data_length - sizeof(short) - sizeof(short);
  memcpy(packet->estring, pbuf+sizeof(short)+sizeof(short), packet->estring_length);
}

void packet_receive_invalid();

void packet_parse(packet_t *packet, const char *pbuf, int *packet_in_length){   
  packet_extract_opcode(packet,pbuf);
  if (IS_RRQ(packet->opcode) || IS_WRQ(packet->opcode)){
    packet_parse_rq(packet, pbuf);
  }else if (IS_ACK(packet->opcode)){
    packet_parse_ack(packet, pbuf);
  }else if (IS_DATA(packet->opcode)){
    packet_parse_data(packet, pbuf, packet_in_length);
  }else if (IS_ERROR(packet->opcode)){
    packet_parse_error(packet, pbuf, packet_in_length);
  }else{
    packet_receive_invalid();
  }
}

void netudp_bind_server(int *ssock, char *port){
  *ssock = socket(AF_INET, SOCK_DGRAM, 0);
  if (*ssock == -1) {
    printf("Socket creation failed: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  } 
  tftpSrv.sin_family = AF_INET;
  tftpSrv.sin_addr.s_addr = INADDR_ANY;
  tftpSrv.sin_port = htons(atoi(port));
  if ((bind(*ssock, (struct sockaddr *)&tftpSrv, sizeof(tftpSrv))) == -1){
    printf("Could not bind server to socket: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("server [PID %d]: New socket bind to port %d\n", getpid(), ntohs((tftpSrv).sin_port));
}

void netudp_rebind_server(int *ssock){
  *ssock = socket(AF_INET, SOCK_DGRAM, 0);
  if (*ssock == -1) {
    printf("Socket creation failed: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  tftpSrv2.sin_family = AF_INET;
  tftpSrv2.sin_addr.s_addr = INADDR_ANY;
  tftpSrv2.sin_port = 0;
  if ((bind(*ssock, (struct sockaddr *)&tftpSrv2, sizeof(tftpSrv2))) == -1){
    printf("Could not bind server to socket: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  socklen_t new_addrlen = sizeof( tftpSrv2 );
  if (getsockname(*ssock, (struct sockaddr *) &tftpSrv2, &new_addrlen ) < 0 ) {
    perror( "server: getsockname" );
    exit(EXIT_FAILURE);
  }
  printf("server [PID %d]: New socket bind to port %d\n", getpid(), ntohs((tftpSrv2).sin_port));
}

void recvfrom_new_client() {
  addrlen = sizeof(tftpCli);
  printf("server [PID %d]: waiting to recvfrom...\n", getpid());
  if ((packet_in_length = recvfrom(sockfd, packet_in, TFTP_MAX_MSGSIZE , 0, (struct sockaddr *)&tftpCli, &addrlen)) == -1) {
    perror("recvfrom");
    exit(EXIT_FAILURE);
  }
  printf("server [PID %d]: recvfrom() %s port %d, %d bytes\n", getpid(), inet_ntoa(tftpCli.sin_addr), ntohs(tftpCli.sin_port), packet_in_length);
}

char* append_to_packet(const void *data, const int data_size){
  packet_out = realloc(packet_out, sizeof(char)*packet_out_length + data_size);
  memmove(packet_out + packet_out_length, data, data_size);
  packet_out_length += data_size;
  return packet_out;
}

void packet_form_data(FILE* f, int offset, int block_num){ 
  fseek(f, offset, SEEK_SET);
  char filebuffer[TFTP_BLOCKSIZE];
  int filebuffer_length = fread(filebuffer, sizeof(char), TFTP_BLOCKSIZE, f);
  short opcode_out = htons(TFTP_OPCODE_DATA);
  short new_blocknum = htons(block_num);
  append_to_packet(&opcode_out, sizeof(short));
  append_to_packet(&new_blocknum, sizeof(short));
  append_to_packet(&filebuffer, filebuffer_length);
}

void packet_form_ack(int block_num){
  short opcode_out = htons(TFTP_OPCODE_ACK);
  short new_blocknum = htons(block_num);
  append_to_packet(&opcode_out,sizeof(short));
  append_to_packet(&new_blocknum, sizeof(short));
}

void packet_form_error(short ecode, char* estring){
  short opcode_out = htons(TFTP_OPCODE_ERROR);
  short new_ecode = htons(ecode);
  append_to_packet(&opcode_out, sizeof(short));
  append_to_packet(&new_ecode, sizeof(short));
  append_to_packet(estring, strlen(estring)+1); 
}

void netudp_send_packet(int *ssock, struct sockaddr* dest, char** packet_out, int * length){      
  int bytes = sendto(*ssock, *packet_out, *length, 0, (struct sockaddr *)dest, sizeof(*dest));
      
  if (bytes == -1){        
    printf("Could not send packet via socket: %s\n",strerror(errno));
    exit(1);
  }
  printf("server [PID %d]: sendto() %s port %d, %d bytes\n", getpid(), inet_ntoa(((struct sockaddr_in *)dest)->sin_addr), ntohs(((struct sockaddr_in *)dest)->sin_port), bytes); 
}

void packet_free(){
  if (packet_out_length > 0){
    free(packet_out);
    packet_out_length = 0;
    packet_out = NULL;
  }
}

void packet_receive_rrq(){
  int offset = 0, block_num = 0;
  if ( access( packet.filename, F_OK ) == -1 ) {
    packet_free();
    packet_form_error(TFTP_ERR_NOT_FOUND, "File not found.");
    netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);
    exit(EXIT_SUCCESS);
  }
  if ( access( packet.filename, R_OK ) == -1 ) {
    packet_free();
    packet_form_error(TFTP_ERR_ACCESS_DENIED, "Access violation.");
    netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);
    exit(EXIT_SUCCESS);
  }
  FILE* f = fopen(packet.filename, "rb");

  do {
    packet_free();
    packet_form_data(f, offset, ++block_num);
    int count = 0;
senddata:
    netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);
    while (1) {
      struct pollfd fd;
      int res;
      fd.fd = sockfd2;
      fd.events = POLLIN;
      res = poll(&fd, 1, TFTP_TIMEOUT_RETRY*1000); // 1000 ms timeout
      if (res == 0) {
        count++;
        if (count == TFTP_TIMEOUT_ABORT) {
          printf("10s TIMOUT! ABORT!\n");
          exit(EXIT_SUCCESS);
        }
        goto senddata;
      }
      else if (res == -1) {
        perror("poll");
        exit(EXIT_FAILURE);
      }
      else {
        if ((packet_in_length = recvfrom(sockfd2, packet_in, TFTP_MAX_MSGSIZE , 0, (struct sockaddr *)&tftpCli, &addrlen)) == -1) {
          perror("recvfrom");
          exit(EXIT_FAILURE);
        }
        printf("server [PID %d]: recvfrom() %s port %d, %d bytes\n", getpid(), inet_ntoa(tftpCli.sin_addr), ntohs(tftpCli.sin_port), packet_in_length);
        packet_parse(&packet, packet_in, &packet_in_length);
        if (!IS_ACK(packet.opcode)) {
          packet_receive_invalid();
        }
        offset += TFTP_BLOCKSIZE;
        break;
      }
    }
  } while (packet_out_length == TFTP_MAX_MSGSIZE);
  packet_free();
  printf("server [PID %d]: %d block(s) of data has been sent\n", getpid(), block_num);
  fclose(f);
}

void packet_receive_wrq(){
  int block_num = 0;
  if( access( packet.filename, F_OK ) != -1 ) {
    packet_free();
    packet_form_error(TFTP_ERR_FILE_EXISTS, "File exists.");
    netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);
    exit(EXIT_SUCCESS);
  }
  FILE* f = fopen(packet.filename, "wb");
  if( f == NULL ) {
    packet_free();
    packet_form_error(TFTP_ERR_DISK_FULL, "Uable to create the file.");
    netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);
    exit(EXIT_SUCCESS);
  }

  do {
    packet_free();
    packet_form_ack(block_num);
    int count = 0;
sendack:
    netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);
    while (1) {
      struct pollfd fd;
      int res;
      fd.fd = sockfd2;
      fd.events = POLLIN;
      res = poll(&fd, 1, TFTP_TIMEOUT_RETRY*1000); // 1000 ms timeout
      if (res == 0) {
        count++;
        if (count == TFTP_TIMEOUT_ABORT) {
          printf("10s TIMOUT! ABORT!\n");
          exit(EXIT_SUCCESS);
        }
        goto sendack;
      }
      else if (res == -1) {
        perror("poll");
        exit(EXIT_FAILURE);
      }
      else {
        if ((packet_in_length = recvfrom(sockfd2, packet_in, TFTP_MAX_MSGSIZE , 0, (struct sockaddr *)&tftpCli, &addrlen)) == -1) {
          perror("recvfrom");
          exit(EXIT_FAILURE);
        }
        printf("server [PID %d]: recvfrom() %s port %d, %d bytes\n", getpid(), inet_ntoa(tftpCli.sin_addr), ntohs(tftpCli.sin_port), packet_in_length);
        packet_parse(&packet, packet_in, &packet_in_length);
        if (!IS_ACK(packet.opcode)) {
          packet_receive_invalid();
        }
        block_num = packet.blocknum;
        fwrite(packet.data, sizeof(char), packet.data_length, f);
        break;
      }
    }
  } while (packet_in_length == TFTP_MAX_MSGSIZE);

  // ack the last packet!
  packet_free();
  packet_form_ack(block_num);
  netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);

  packet_free();
  printf("server [PID %d]: %d block(s) of data has been received\n", getpid(), block_num);
  fclose(f);
}

void packet_receive_error() {
  fprintf(stderr, "server [PID %d]: Received error %i: %s\n", getpid(), packet.ecode, packet.estring);
  exit(EXIT_SUCCESS);
}

void packet_receive_invalid() {
  packet_free();
  packet_form_error(TFTP_ERR_ILLEGAL_OP, "Invalid request.");
  netudp_send_packet(&sockfd2, (struct sockaddr *) &tftpCli, &packet_out,  &packet_out_length);
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){

  // create UDP socket at random port
  netudp_rebind_server(&sockfd);

  // Main recvfrom() loop
  while (1) {
    recvfrom_new_client();
    int pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // in child process
      
      // Create an ephemeral port to handle this client
      netudp_rebind_server(&sockfd2);
      // parse the initial request
      packet_parse(&packet, packet_in, &packet_in_length);
      printf("server [PID %d]: Opcode is %hi, filename is \"%s\"\n", getpid(), packet.opcode, packet.filename);
      
      // handle this client
      if (IS_RRQ(packet.opcode))
        packet_receive_rrq();       
      else if (IS_WRQ(packet.opcode))
        packet_receive_wrq();
      else
        packet_receive_invalid();

      // done with this client. Exit child process
      close(sockfd2);
      exit(EXIT_SUCCESS);
    }
    else { // in parent process
      // do nothing
    }
  }
  close(sockfd);

  return 0;
}