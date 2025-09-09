#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
#define NAME_LENGTH 32

void *receive_messages(void *sock_ptr) {
  int sock = *(int *)sock_ptr;
  char buffer[BUFFER_SIZE];

  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
      printf("\nServer disconnected.\n");
      exit(EXIT_FAILURE);
    }
    printf("\n%s", buffer);
    printf("You: ");
    fflush(stdout);
  }
}

int main() {
  int sock;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];
  char name[NAME_LENGTH];
  pthread_t recv_thread;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("inet_pton failed");
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("connect failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Enter your name: ");
  fgets(name, NAME_LENGTH, stdin);
  name[strcspn(name, "\n")] = 0;

  send(sock, name, strlen(name), 0);

  printf("Connected to server as %s!\n", name);

  if (pthread_create(&recv_thread, NULL, receive_messages, (void *)&sock) !=
      0) {
    perror("pthread_create failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  while (1) {
    printf("You: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    send(sock, buffer, strlen(buffer), 0);
  }

  close(sock);
  return 0;
}