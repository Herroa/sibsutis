#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <server_ip> <port> <iterations>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *server_ip = argv[1];
  int port = atoi(argv[2]);
  int iterations = atoi(argv[3]);

  int client_socket;
  struct sockaddr_in server_addr;

  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation failed");
    return EXIT_FAILURE;
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    close(client_socket);
    return EXIT_FAILURE;
  }

  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) < 0) {
    perror("Connection failed");
    close(client_socket);
    return EXIT_FAILURE;
  }

  printf("Connected to server: %s:%d\n", server_ip, port);

  for (int i = 1; i <= iterations; i++) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "Iteration %d\n", i);

    if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
      perror("Send failed");
      break;
    }

    printf("Sent: %s", buffer);
    sleep(i);
  }

  close(client_socket);
  return 0;
}