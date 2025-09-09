#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int server_socket;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
  int client_socket = *(int *)arg;
  char buffer[BUFFER_SIZE];
  int bytes_received;

  while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) >
         0) {
    buffer[bytes_received] = '\0';
    printf("Received: %s\n", buffer);

    pthread_mutex_lock(&file_mutex);
    FILE *file = fopen("shared_data.txt", "a");
    if (file) {
      fprintf(file, "%s", buffer);
      fclose(file);
    }
    pthread_mutex_unlock(&file_mutex);
  }

  close(client_socket);
  free(arg);
  pthread_exit(NULL);
}

int main() {
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  int port = 0;

  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = 0;

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("Bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  if (getsockname(server_socket, (struct sockaddr *)&server_addr, &client_len) <
      0) {
    perror("Getsockname failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }
  port = ntohs(server_addr.sin_port);
  printf("Server is running on port: %d\n", port);

  if (listen(server_socket, MAX_CLIENTS) < 0) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Waiting for connections...\n");

  while (1) {
    int *client_socket = (int *)malloc(sizeof(int));
    *client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (*client_socket < 0) {
      perror("Accept failed");
      free(client_socket);
      continue;
    }

    printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    pthread_t thread;
    if (pthread_create(&thread, NULL, handle_client, client_socket) != 0) {
      perror("Thread creation failed");
      close(*client_socket);
      free(client_socket);
    } else {
      pthread_detach(thread);
    }
  }

  close(server_socket);
  return 0;
}