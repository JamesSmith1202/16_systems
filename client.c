#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;
  char input[BUFFER_SIZE];
  from_server = client_handshake( &to_server );
  while (1) {
    printf("Type your message: ");
    fgets(input, sizeof(input), stdin);
    
    // Replace newline
    size_t length = strlen(input); 
    if (input[length - 1] == '\n') {
      input[length - 1] = '\0';
    }
    
    write(to_server, input, sizeof(input));
    read(from_server, input, sizeof(input));
    printf("Received: %s\n", input);
  }
}
