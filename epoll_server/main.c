#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// If compiling on Mac, brew install epoll-shim and include this header.
// If compiling on Linux, the standard <sys/epoll.h> handles it automatically.
#ifdef __APPLE__
#include "/usr/local/include/libepoll-shim/sys/epoll.h"
#else
#include <sys/epoll.h>
#endif

#define PORT 8080
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

// Helper function to make a socket non-blocking
int make_socket_non_blocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    return -1;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
  int server_fd, epoll_fd;
  struct sockaddr_in address;

  // 1. CREATE THE INITIAL LISTENING SOCKET
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options to reuse the address (prevents "Address already in use"
  // errors)
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // Bind socket to Port 8080
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  // Tell the OS to start listening for incoming connections
  if (listen(server_fd, SOMAXCONN) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  // Crucial: Make the master listening socket non-blocking
  make_socket_non_blocking(server_fd);
  printf("Server listening on port %d...\n", PORT);

  // 2. INITIALISE THE EPOLL INSTANCE (The Registry)
  epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1 failed");
    exit(EXIT_FAILURE);
  }

  // Register our master listening socket to the epoll interest list
  struct epoll_event ev;
  ev.events =
      EPOLLIN | EPOLLET;  // Look for incoming reads via Edge-Triggered mode
  ev.data.fd = server_fd; // Keep track of this specific file descriptor

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
    perror("epoll_ctl: server_fd registration failed");
    exit(EXIT_FAILURE);
  }

  // 3. INSTANTIATE THE POOL OF EVENTS
  // This is the array (bucket) we pass to the kernel to fill with active
  // sockets
  struct epoll_event events[MAX_EVENTS];

  // 4. THE INFINITE RUN FOREVER LOOP
  while (1) {
    // Sleep here completely until the OS notifies us that sockets have data
    int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (num_events == -1) {
      perror("epoll_wait broken");
      break;
    }

    // Loop through ONLY the small subset of sockets that are active
    for (int i = 0; i < num_events; i++) {
      int current_fd = events[i].data.fd;

      // CASUISTRY A: A brand-new customer is trying to connect
      if (current_fd == server_fd) {
        while (1) {
          struct sockaddr_in client_addr;
          socklen_t client_len = sizeof(client_addr);

          int client_fd =
              accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
          if (client_fd == -1) {
            // If errno is EAGAIN, it means we processed all pending connections
            break;
          }

          // Instantly make this new connection non-blocking
          make_socket_non_blocking(client_fd);

          // Register this new phone/customer socket to our epoll watcher list
          struct epoll_event client_ev;
          client_ev.events = EPOLLIN | EPOLLET; // Watch for data streams
          client_ev.data.fd = client_fd;

          epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev);
          printf("New client connected on socket FD: %d\n", client_fd);
        }
      }
      // CASUISTRY B: An existing customer socket just transmitted active
      // packets
      else if (events[i].events & EPOLLIN) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        // Stage 1: Read the raw bytes out of the kernel buffer
        ssize_t bytes_read = recv(current_fd, buffer, sizeof(buffer), 0);

        if (bytes_read > 0) {
          printf("Received raw packets (%ld bytes) on FD %d\n", bytes_read,
                 current_fd);

          // ─────────────────────────────────────────────────────────────
          // STAGE 2: THIS IS WHERE YOU TRIGGER YOUR PROTOCOL READER!
          // e.g., parse_custom_protocol(buffer, bytes_read);
          // ─────────────────────────────────────────────────────────────

          // Simple Echo response for testing: Send back what they sent
          send(current_fd, buffer, bytes_read, 0);
        } else if (bytes_read == 0) {
          // Client disconnected natively, clean up memory
          printf("Client on FD %d closed connection\n", current_fd);
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, current_fd, NULL);
          close(current_fd);
        }
      }
    }
  }

  // Cleanup resources before exit
  close(server_fd);
  close(epoll_fd);
  return 0;
}
