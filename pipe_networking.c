#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int upstream;//FIFO FD
  char msg[HANDSHAKE_BUFFER_SIZE];//client fifo pipe
  if(mkfifo(PIPE_NAME, 0644)){//create the fifo
    printf("%s\n", strerror(errno));
  }
  if((upstream = open(PIPE_NAME, O_RDONLY)) == -1){//open it, blocking until client forms connection
    printf("%s\n", strerror(errno));
  }
  if(read(upstream, msg, sizeof(msg)) == -1){//read in the fifo name sent by client
    printf("%s\n", strerror(errno));
  }
  printf("client msg: %s\n", msg);
  if(remove(PIPE_NAME) == -1){//remove the fifo, making it an unnamed pipe
    printf("%s\n", strerror(errno));
  }
  if((*to_client = open(msg, O_WRONLY)) == -1){//Connect to the client fifo
    printf("%s\n", strerror(errno));
  }
  strcpy(msg, ACK);
  if((write(*to_client, msg, sizeof(msg))) == -1){//write the msg back to the pipe
    printf("%s\n", strerror(errno));
  }
  if ((read(upstream, msg, sizeof(msg))) == -1) {
    printf("%s\n", strerror(errno));
  }
  if (!strcmp(msg, ACK)) {
    printf("Connection established...\n");
  }
  return upstream;//Server has done its side of the handshake
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int downstream;
  char private_pipe[HANDSHAKE_BUFFER_SIZE], msg[HANDSHAKE_BUFFER_SIZE];
  sprintf(private_pipe, "%d", getpid());
  strcpy(msg, private_pipe);
  printf("msg: %s\n", private_pipe);
  if (mkfifo(private_pipe, 0644)) { // Make private pipe
    printf("%s\n", strerror(errno));
  }
  
  if ((*to_server = open(PIPE_NAME, O_WRONLY)) == -1) {
    printf("%s\n", strerror(errno));
  }
  if ((write(*to_server, msg, sizeof(msg))) == -1) {
    printf("%s\n", strerror(errno));
  }
  if ((downstream = open(private_pipe, O_RDONLY)) == -1) { // Open private pipe
    printf("%s\n", strerror(errno));
  }
  if ((read(downstream, msg, sizeof(msg))) == -1) { // Read from the private pipe
    printf("%s\n", strerror(errno));
  }
  if (!strcmp(msg, ACK)) {
    printf("Connection established...\n");
  }
  remove(private_pipe);  // Remove the private pipe

  write(*to_server, msg, sizeof(msg));
  
  return downstream;
}
