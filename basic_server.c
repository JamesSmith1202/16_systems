#include "pipe_networking.h"

void uppercase(char *s) {
  while ((*s = toupper(*s))) {
    s++;
  }
}

int main() {

  int to_client;
  int from_client;
  char input[BUFFER_SIZE];
  from_client = server_handshake( &to_client );
  while (1) {
    read(from_client, input, sizeof(input));
    printf("Received: %s\n", input);
    uppercase(input);
    write(to_client, input, sizeof(input));
  }
  
}
