#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr,
            "Usage: %s <server_ip> <server_port> <number> <iterations>\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  char *server_ip = argv[1];
  int server_port = atoi(argv[2]);
  int number = atoi(argv[3]);
  int iterations = atoi(argv[4]);
  int sock;
  struct sockaddr_in server_addr;
  char buffer[1024];

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);

  if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Connection failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < iterations; i++) {
    snprintf(buffer, sizeof(buffer), "Iteration %d: %d", i + 1, number);
    write(sock, buffer, strlen(buffer));
    sleep(number);
  }

  close(sock);
  return 0;
}