#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int fd;//FIFO FD
  long int * msg;//client fifo pipe
  if(mkfifo(PIPE_NAME, 0644)){//create the fifo
    printf("%s\n", strerror(errno));
  }
  if((fd = open(PIPE_NAME, O_RDONLY)) == -1){//open it, blocking until client forms connection
    printf("%s\n", strerror(errno));
  }
  if(read(fd, msg, sizeof(long int)) == -1){//read in the fifo name sent by client
    printf("%s\n", strerror(errno));
  }
  if(!remove(PIPE_NAME)){//remove the fifo, making it an unnamed pipe
    printf("%s\n", strerror(errno));
  }
  if((fd = open(msg, O_WRONLY)) == -1){//Connect to the client fifo
    printf("%s\n", strerror(errno));
  }
  if((write(fd, msg, sizeof(long int))) == -1){//write the msg back to the pipe
    printf("%s\n", strerror(errno));
  }
  return 0;//Server has done its side of the handshake
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  return 0;
}
