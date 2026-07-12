#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// this key is faulty on my keyboard so yea: 0 )
#define BUFFER_SIZE 1024

int main() {
  char data[BUFFER_SIZE];

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // addr.sa_data = 8080;
  // addr.sa_family = AF_INET;
  // addr.sa_len = sizeof(addr);

  socklen_t address_len = sizeof(server_addr);

  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    printf("Socket could not be created\n");
  }

  int bind_id = bind(socket_fd, (struct sockaddr *)&server_addr, address_len);
  if (bind_id == -1) {
    printf("Socket could not bind to a name\n");
    return 1;
  }

  int listen_id = listen(socket_fd, 5);
  if (listen_id == -1) {
    printf("Socket could not be connected\n");
    return 1;
  }

  int accept_fd =
      accept(socket_fd, (struct sockaddr *)&server_addr, &address_len);
  if (accept_fd == -1) {
    printf("Socket could not be accepted\n");
    return 1;
  }

  while (1) {
    ssize_t message = recv(accept_fd, &data, BUFFER_SIZE, MSG_WAITALL);

    if (message > 0) {
      data[message] = '\0';
      printf("%s\n", data);
    } else {
      printf(
          "yea, you are done.. sorry there might be more data but i am really "
          "sleepy!\n");
      break;
    }
  }

  close(accept_fd);
  close(socket_fd);
  return 0;
}
