#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  int server_fd;
  struct sockaddr_in address;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = 0;

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  socklen_t len = sizeof(address);
  getsockname(server_fd, (struct sockaddr *)&address, &len);
  std::cout << "Server is running on port: " << ntohs(address.sin_port)
            << std::endl;

  return 0;
}