#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " <server_ip> <server_port>"
              << std::endl;
    return 1;
  }

  int sockfd;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  int i = 0;
  std::string messages[3] = {"first", "second", "third"};
  while (i < 3) {
    std::string message = messages[i];
    sendto(sockfd, message.c_str(), message.length(), 0,
           (const struct sockaddr *)&server_addr, sizeof(server_addr));
    std::cout << "sent to server: " << message << std::endl;

    int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, 0, NULL, NULL);
    buffer[n] = '\0';
    std::cout << "received from server: " << buffer << std::endl;

    sleep(1);
    i++;
  }

  close(sockfd);
  return 0;
}