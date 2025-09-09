#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <vector>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  std::vector<int> client_sockets;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("Setsockopt failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = 0;

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  socklen_t len = sizeof(address);
  getsockname(server_fd, (struct sockaddr *)&address, &len);
  std::cout << "Server is running on port: " << ntohs(address.sin_port)
            << std::endl;

  if (listen(server_fd, MAX_CLIENTS) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  fd_set readfds;
  int max_sd;

  while (true) {
    FD_ZERO(&readfds);

    FD_SET(server_fd, &readfds);
    max_sd = server_fd;

    for (int sd : client_sockets) {
      FD_SET(sd, &readfds);
      if (sd > max_sd) {
        max_sd = sd;
      }
    }

    int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    if (activity < 0 && errno != EINTR) {
      perror("Select error");
    }

    if (FD_ISSET(server_fd, &readfds)) {
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
      }
      std::cout << "New connection, socket fd: " << new_socket << std::endl;

      client_sockets.push_back(new_socket);
    }

    for (auto it = client_sockets.begin(); it != client_sockets.end(); ++it) {
      int sd = *it;

      if (FD_ISSET(sd, &readfds)) {
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(sd, buffer, BUFFER_SIZE);

        if (valread == 0) {
          std::cout << "Client disconnected, socket fd: " << sd << std::endl;
          close(sd);
          client_sockets.erase(it);
          --it;
        } else {
          std::cout << "Received from client (" << sd << "): " << buffer
                    << std::endl;
        }
      }
    }
  }

  return 0;
}