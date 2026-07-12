#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main() {
  char data[BUFFER_SIZE];
  struct sockaddr addr;

  addr.sa_family = AF_INET;
  addr.sa_len = sizeof(addr);
  unsigned int address_len = sizeof(addr) - 1;

  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);

  int bind_id = bind(socket_fd, &addr, address_len);
  int listen_id = listen(socket_fd, 0);
  int accept_fd = accept(socket_fd, &addr, &address_len);
  int connect_id = connect(accept_fd, &addr, address_len);

  while (1) {
    ssize_t message = recv(socket_fd, &data, BUFFER_SIZE, MSG_WAITALL);
    if (message > 0) {
      printf("%s\n", data);
    } else {
      printf("yea, you are done.. sorry there might be more data but i am "
             "really sleepy!\n");
    }
  }
  return 0;
}
