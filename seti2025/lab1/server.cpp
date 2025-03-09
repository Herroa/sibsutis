#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  char buffer[BUFFER_SIZE];

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = 0;

  if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  getsockname(sockfd, (struct sockaddr *)&server_addr, &client_len);
  std::cout << "port: " << ntohs(server_addr.sin_port) << std::endl;

  while (true) {
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr *)&client_addr, &client_len);
    buffer[n] = '\0';

    std::cout << "received from client: " << buffer << std::endl;
    std::cout << "client ip: " << inet_ntoa(client_addr.sin_addr)
              << ", port: " << ntohs(client_addr.sin_port) << std::endl;

    for (int i = 0; i < n; i++) {
      buffer[i] = toupper(buffer[i]);
    }

    sendto(sockfd, (const char *)buffer, strlen(buffer), 0,
           (const struct sockaddr *)&client_addr, client_len);
  }

  close(sockfd);
  return 0;
}