// Assignment 4
// Team: Haoming Li (lih14) and Tianxin Zhou (zhout3)

#define BUFFER_SIZE 1024
#define FILE_LIST_NAME "4220_file_list.txt"
#if defined(__APPLE__)
#  define COMMON_DIGEST_FOR_OPENSSL
#  include <CommonCrypto/CommonDigest.h>
#  define SHA1 CC_SHA1
#else
#  include <openssl/md5.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define FILE_LIST_NAME "4220_file_list.txt"
int port;
char dir_template[] = "XXXXXX";

struct file_info {
  char filename[BUFFER_SIZE];
  char md5_hash[BUFFER_SIZE];
  time_t m_time;
};

int startsWith(const char *a, const char *b);
void server_recv_put(const char* filename, int filelen, char* buffer, int n, int newsock); // recv content of file, send back ack
void server_recv_get(const char* filename, int newsock); // send back "PUT filename filelen\nfilecontent" cmd, recv ack
void server_recv_query(const char* filename, int newsock); // send back "mtime(8 bytes)", recv ack
void server_recv_contents(int newsock); // send back file_list, recv ack
void client_send_put(const char* filename, int newsock); // triggers server_recv_put
void client_send_get(const char* filename, int newsock); // triggers server_recv_get
void client_send_query(const char* filename, int newsock, time_t* result);
char* get_filename(char* buffer);
int get_filelen(char* buffer);

char* formatdate(char* str, time_t val) {
  strftime(str, 36, "%m-%d-%Y %H:%M:%S", localtime(&val));
  return str;
}

char* get_filename(char* cmd) {
  char* filename = (char*)calloc(BUFFER_SIZE, sizeof(char));
  int i, j;
  for (i = 0; cmd[i] != ' '; ++i); 
  ++i;
  for (j = i; cmd[j] != ' ' && cmd[j] != '\n'; ++j);
  strncpy(filename,cmd+i,j-i); filename[j-i] = '\0';
  return filename;
}

int get_filelen(char* cmd) {
  char tmp[BUFFER_SIZE];
  int i, j, k;
  for (i = 0; cmd[i] != ' '; ++i); 
  ++i;
  for (j = i; cmd[j] != ' '; ++j); 
  ++j;
  for (k = j; cmd[k] != '\n'; ++k);
  strncpy(tmp,cmd+j,k-j); tmp[k-j] = '\0';
  return atoi(tmp);
}

void print_md5_sum(unsigned char* md) {
  int i;
  for(i=0; i < MD5_DIGEST_LENGTH; i++) {
    printf("%02x",md[i]);
  }
  printf("\n");
}

void server_recv_put(const char* filename, int filelen, char* buffer, int n, int newsock) {

  // storing the file
  FILE* f = fopen(filename, "w");
  int to_store = filelen, i, j;

  // store the first packet
  fwrite(buffer, sizeof(char), n, f); 
  to_store -= n;

  // store rest of file
  while (to_store != 0) {
    n = recv( newsock, buffer, BUFFER_SIZE, 0 );
    printf("recved %d bytes\n", n);
    if ( n < 0 ) {
      perror( "recv() failed" );
      exit(EXIT_FAILURE);
    }
    else if ( n == 0 ) {
      break;
    }
    else {
      fwrite(buffer, sizeof(char), n, f); 
      to_store -= n;
    }
  }
  fclose(f);

  // get md5 hash
  unsigned char md5_hash[MD5_DIGEST_LENGTH];
  char* file_buffer = calloc(filelen, sizeof(char));
  f = fopen(filename, "r");
  fread(file_buffer, sizeof(char), filelen, f);
  fclose(f);
  MD5((unsigned char *)file_buffer, (unsigned long)filelen, md5_hash);
  free(file_buffer);
  //print_md5_sum(md5_hash);

  // write to file list
  FILE* file_list = fopen(FILE_LIST_NAME, "r");
  FILE* tmp_file_list = fopen("tmp.txt", "w");
  char line[BUFFER_SIZE];
  while (fgets(line, sizeof(line), file_list)) {
    for (i = 0; line[i] != ' '; ++i); 
    i += 4;
    for (j = i; line[j] != '\n'; ++j);
    if ((j-i) == strlen(filename)) {
      int n = memcmp(line+i, filename, j-i);
      if (n == 0) continue;
    }
    fputs(line, tmp_file_list);
  }
  for(i = 0; i < MD5_DIGEST_LENGTH; i++) {
    fprintf(tmp_file_list, "%02x", md5_hash[i]);
  }
  fwrite("    ", sizeof(char), 4, tmp_file_list);
  fwrite(filename, sizeof(char), strlen(filename), tmp_file_list);
  fwrite("\n", sizeof(char), 1, tmp_file_list);
  fclose(file_list);
  fclose(tmp_file_list);
  remove(FILE_LIST_NAME);
  rename("tmp.txt", FILE_LIST_NAME);

  // ack the client
  n = send( newsock, "ACK\n", 4, 0 );
  if (n != 4) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }
}

void server_recv_get(const char* filename, int newsock) {

  // read file to buffer
  char cmd[BUFFER_SIZE]; int cmdlen = 0;
  memcpy(cmd, "PUT ", 4); cmdlen += 4;
  memcpy(cmd+cmdlen, filename, strlen(filename)); cmdlen += strlen(filename);
  memcpy(cmd+cmdlen, " ", 1); cmdlen += 1;
  FILE* f = fopen(filename, "r");
  fseek(f, 0L, SEEK_END);
  int filelen = ftell(f), n;
  rewind(f);

  char filelen_a[BUFFER_SIZE];
  sprintf(filelen_a, "%d", filelen);
  memcpy(cmd+cmdlen, filelen_a, strlen(filelen_a)); cmdlen += strlen(filelen_a);
  memcpy(cmd+cmdlen, "\n", 1); cmdlen += 1;
  char buffer[cmdlen+filelen];
  memcpy(buffer, cmd, cmdlen);
  fread(buffer+cmdlen, sizeof(char), filelen, f);
  fclose(f);

  // send file to client
  n = send( newsock, buffer, cmdlen+filelen, 0 );
  if (n != (cmdlen+filelen)) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }

  // recv ack
  n = recv( newsock, buffer, BUFFER_SIZE, 0 );
  if (n < 0) {
    perror("recv failed");
    exit(EXIT_FAILURE);
  }
}
void server_recv_query(const char* filename, int newsock) {

  // prepare "mtime"
  int n;
  char buffer[sizeof(time_t)];
  struct stat stbuf;
  stat(filename, &stbuf);
  memcpy(buffer, &stbuf.st_mtime, sizeof(time_t));

  // send to client
  n = send( newsock, buffer, sizeof(time_t), 0 );
  if (n != sizeof(time_t)) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }

  // recv ack
  n = recv( newsock, buffer, BUFFER_SIZE, 0 );
  if (n < 0) {
    perror("recv failed");
    exit(EXIT_FAILURE);
  }
}
void server_recv_contents(int newsock) {
  server_recv_get(FILE_LIST_NAME, newsock);
}

void client_send_put(const char* filename, int newsock) {
  server_recv_get(filename, newsock);
}

void client_send_get(const char* filename, int newsock) {

  // send GET
  char buffer[BUFFER_SIZE]; int bufferlen = 0, n, i;
  memcpy(buffer, "GET ", 4); bufferlen += 4;
  memcpy(buffer+bufferlen, filename, strlen(filename)); bufferlen += strlen(filename);
  memcpy(buffer+bufferlen, "\n", 1); bufferlen += 1;
  n = send( newsock, buffer, bufferlen, 0 );
  if (n != bufferlen) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }

  // recv PUT
  n = recv( newsock, buffer, BUFFER_SIZE, 0 );
  int filelen = get_filelen(buffer);
  for (i = 0; buffer[i] != '\n'; ++i);
  memmove(buffer,buffer+i+1,n-i-1);
  n = n-i-1;

  // storing the file
  FILE* f = fopen(filename, "w");
  if (f == NULL) {
    perror("fopen failed");
    exit(EXIT_FAILURE);
  }
  int to_store = filelen;

  // store the first packet
  fwrite(buffer, sizeof(char), n, f);
  to_store -= n;

  // store rest of file
  while (to_store != 0) {
    n = recv( newsock, buffer, BUFFER_SIZE, 0 );
    printf("recved %d bytes\n", n);
    if ( n < 0 ) {
      perror( "recv() failed" );
      exit(EXIT_FAILURE);
    }
    else if ( n == 0 ) {
      break;
    }
    else {
      fwrite(buffer, sizeof(char), n, f); 
      to_store -= n;
    }
  }
  fclose(f);

  // ack the server
  n = send( newsock, "ACK\n", 4, 0 );
  if (n != 4) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }
}

void client_send_query(const char* filename, int newsock, time_t* result) {
  
  // send QUERY
  char buffer[BUFFER_SIZE]; int bufferlen = 0, n;
  memcpy(buffer, "QUERY ", 6); bufferlen += 6;
  memcpy(buffer+bufferlen, filename, strlen(filename)); bufferlen += strlen(filename);
  memcpy(buffer+bufferlen, "\n", 1); bufferlen += 1;
  n = send( newsock, buffer, bufferlen, 0 );
  if (n != bufferlen) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }

  // recv time_t
  n = recv( newsock, buffer, BUFFER_SIZE, 0 );
  if (n < 0) {
    perror("recv failed");
    exit(EXIT_FAILURE);
  }
  memcpy(result, buffer, sizeof(time_t));

  // ack the server
  n = send( newsock, "ACK\n", 4, 0 );
  if (n != 4) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }
}

int startsWith(const char *a, const char *b) {
  if (strncmp(a, b, strlen(b)) == 0) return 1;
  return 0;
}

void do_server() {

  // get started
  char* tmp_dir = mkdtemp(dir_template);
  if (tmp_dir == NULL) {
    perror("mkdtemp failed");
    exit(EXIT_FAILURE);
  }
  chdir(tmp_dir);
  FILE* file_list = fopen(FILE_LIST_NAME, "w");
  if (file_list == NULL) {
    perror("fopen failed");
    exit(EXIT_FAILURE);
  }
  fclose(file_list);

  // create listener socket
  int sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server;
  server.sin_family = PF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( port );
  if (bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror( "bind() failed" );
    exit( EXIT_FAILURE );
  }
  listen( sd, 5 );
  printf( "Started server; listening on port: %d\n", port );

  // main accept() loop
  struct sockaddr_in client;
  int fromlen = sizeof(client), n, i;
  char buffer[ BUFFER_SIZE ];
  char* filename = NULL;
  int filelen;
  while (1) {
    int newsock = accept(sd, (struct sockaddr *)&client, (socklen_t*)&fromlen);
    printf( "Received incoming connection\n");

    // server plays a passive role
    do {
      n = recv( newsock, buffer, BUFFER_SIZE, 0 );
      if ( n < 0 ) {
        perror( "recv() failed" );
      }
      else if ( n == 0 ) {
        printf( "Client disconnected\n");
      }
      else {
        // Possible cmd in buffer:
        // "PUT filename filelength\nfilecontent" followed by cmd 
        // "GET filename\n" followed by cmd
        // "QUERY filename\n" followed by cmd
        // "CONTENTS\n" followed by cmd
        if (startsWith(buffer,"PUT")) {
          filename = get_filename(buffer);
          filelen = get_filelen(buffer);
          for (i = 0; buffer[i] != '\n'; ++i);
          memmove(buffer,buffer+i+1,n-i-1);
          printf("Server: start PUT\n");
          server_recv_put(filename, filelen, buffer, n-i-1, newsock);
          printf("Server: end PUT\n");
          free(filename);
        }
        else if (startsWith(buffer,"GET")) {
          filename = get_filename(buffer);
          printf("Server: start GET\n");
          server_recv_get(filename, newsock);
          printf("Server: end GET\n");
          free(filename);
        }
        else if (startsWith(buffer,"QUERY")) {
          filename = get_filename(buffer);
          printf("Server: start QUERY\n");
          server_recv_query(filename, newsock);
          printf("Server: end QUERY\n");
          free(filename);
        }
        else if (startsWith(buffer,"CONTENTS")) {
          printf("Server: start CONTENTS\n");
          server_recv_contents(newsock);
          printf("Server: end CONTENTS\n");
        }
        else {
          printf("Bad command: %s\n", buffer);
        }
      }
    } while (n > 0);
    close(newsock);
    printf("Closed connection.\n");
  }
}

void do_client() {

  // create TCP client socket
  int sock = socket( PF_INET, SOCK_STREAM, 0 );
  if (sock < 0) {
    perror( "socket failed" );
    exit( EXIT_FAILURE );
  }
  struct hostent * hp = gethostbyname( "localhost" );
  if (hp == NULL) {
    perror( "gethostbyname failed" );
    exit( EXIT_FAILURE );
  }
  struct sockaddr_in server;
  server.sin_family = PF_INET;
  memcpy( (void *)&server.sin_addr, (void *)hp->h_addr, hp->h_length );
  server.sin_port = htons( port );
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror( "connect failed" );
    exit( EXIT_FAILURE );
  }
  
  // create my-file array (array of file_info)
  DIR *d; struct dirent *dir; int count = 0, i;
  d = opendir(".");
  while ((dir = readdir(d)) != NULL) {
    if (dir->d_name[0] == '.') continue;
    count++;
  }
  closedir(d);
  int my_files_num = count;
  struct file_info my_files[my_files_num]; 
  d = opendir(".");
  count = 0;
  while ((dir = readdir(d)) != NULL) {
    if (dir->d_name[0] == '.') continue;
    // filename
    strcpy(my_files[count].filename, dir->d_name);
    // m_time
    struct stat stbuf;
    stat(my_files[count].filename, &stbuf);
    //printf("%s %s\n", my_files[count].filename, ctime(&stbuf.st_mtime));
    memcpy(&(my_files[count].m_time), &stbuf.st_mtime, sizeof(time_t));
    //printf("%s %s\n", my_files[count].filename, ctime(&(my_files[count].m_time)));
    // md5_hash
    FILE* f = fopen(my_files[count].filename, "r");
    fseek(f, 0L, SEEK_END);
    int filelen = ftell(f);
    rewind(f);
    unsigned char md5_hash[MD5_DIGEST_LENGTH];
    char* file_buffer = calloc(filelen, sizeof(char));
    fread(file_buffer, sizeof(char), filelen, f);
    fclose(f);
    MD5((unsigned char *)file_buffer, (unsigned long)filelen, md5_hash);
    free(file_buffer);
    //printf("%s %s\n", my_files[count].filename, ctime(&(my_files[count].m_time)));
    
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) {
      sprintf(&(my_files[count].md5_hash)[i*2], "%02x", md5_hash[i]);
    }
    //printf("%s %s\n", my_files[count].filename, ctime(&(my_files[count].m_time)));
    
    // printf("%s %s %s\n", my_files[count].filename, my_files[count].md5_hash, ctime(&(my_files[count].m_time)));
    count++;
  }
  closedir(d);
  // for (i = 0; i < my_files_num; ++i) {
  //   printf("%s %s\n", my_files[i].filename, ctime(&(my_files[i].m_time)));
  // }

  // create your-file array (array of file_info)
  char buffer[BUFFER_SIZE]; int n, j;
  n = send( sock, "CONTENTS\n", 9, 0 );
  if (n != 9) {
    perror("send failed");
    exit(EXIT_FAILURE);
  }
  n = recv( sock, buffer, BUFFER_SIZE, 0 );
  send( sock, "ACK\n", 4, 0);

  int your_files_num = 0;
  for (i = 0; buffer[i] != '\n'; ++i);
  memmove(buffer,buffer+i+1,n-i-1);
  n = n-i-1;
  for (i = 0; i < n; ++i) {
    if (buffer[i] == '\n') your_files_num++;
  }

  struct file_info your_files[your_files_num];
  count = 0;
  for (i = 0, j = 0; j < n && i < n; ++j) {
    if (buffer[j] == '\n') {
      memcpy(&(your_files[count].md5_hash), buffer+i, 32);
      memcpy(&(your_files[count].filename), buffer+i+32+4, j-(i+32+4));
      (your_files[count].filename)[j-(i+32+4)] = '\0';
      i = j+1;
      count++;
    }
  }
  
  // ask for file list, CONTENTS
  // for every file "yourfile" in the list
  //   if I dont have "yourfile", GET - non-overlapped
  // for every file "myfile" I have
  //   if "myfile" not in list, PUT - non-overlapped
  //   else if "myfile" in list, QUERY
  //     if "myfile" newer, PUT - overlapped
  //     else if "myfile" older, GET - overlapped

  // for every file "yourfile" in the list
  for (i = 0; i < your_files_num; ++i) {
    // if I dont have "yourfile", GET - non-overlapped
    int yours_not_mine = 1;
    for (j = 0; j < my_files_num; ++j) {
      if (strcmp(your_files[i].filename, my_files[j].filename) == 0) {
        yours_not_mine = 0; break;
      }
    }
    if (yours_not_mine){
      printf("Server has \"%s\", client do not\n", your_files[i].filename);
      client_send_get(your_files[i].filename, sock);
    }
  }
  // for every file "myfile" I have
  for (i = 0; i < my_files_num; ++i) {
    // if "myfile" not in list, PUT - non-overlapped
    int mine_not_yours = 1;
    for (j = 0; j < your_files_num; ++j) {
      if (strcmp(my_files[i].filename, your_files[j].filename) == 0) {
        mine_not_yours = 0; break;
      }
    }
    if (mine_not_yours){
      printf("Client has \"%s\", server do not\n", my_files[i].filename);
      client_send_put(my_files[i].filename, sock);
    }
    else { // filenames are the same
      char date[36], date2[36];
      //printf("\"%s\"'s hash on server %s, \"%s\"'s hash on client %s\n", your_files[j].filename, your_files[j].md5_hash, my_files[i].filename, my_files[i].md5_hash);
      if (memcmp(your_files[j].md5_hash, my_files[i].md5_hash, 32) == 0) {
        printf("Exact same file \"%s\"\n", my_files[i].filename);
        continue; // exact same file
      }
      client_send_query(my_files[i].filename, sock, &your_files[j].m_time);
      //printf("difftime = %f\n", difftime(my_files[i].m_time, your_files[j].m_time));
      if (difftime(my_files[i].m_time, your_files[j].m_time) > 0){
        // myfile is newer
        printf("Client has newer \"%s\" at %s, server at %s\n", my_files[i].filename, formatdate(date, my_files[i].m_time), formatdate(date2, your_files[j].m_time));
        client_send_put(my_files[i].filename, sock);
      }
      else if (difftime(my_files[i].m_time, your_files[j].m_time) < 0){
        // myfile is older
        printf("Client has older \"%s\" at %s, server at %s\n", my_files[i].filename, formatdate(date, my_files[i].m_time), formatdate(date2, your_files[j].m_time));
        client_send_get(your_files[j].filename, sock);
      }
      else {
        printf("Exact same file \"%s\", exact same time\n", my_files[i].filename);
      }
    }
  }
  close(sock);
}

int main(int argc, char **argv) {
  
  if (argc != 3) {
    printf("usage: ./syncr [client | server] [port]\n");
    return 0;
  }

  port = atoi(argv[2]);
  if (strcmp(argv[1], "server") == 0)
    do_server();
  else if (strcmp(argv[1], "client") == 0)
    do_client();
  else
    printf("usage: ./syncr [client | server] [port]\n");

  return 0;
}
