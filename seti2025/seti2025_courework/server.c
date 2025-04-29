#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define NAME_LENGTH 32

typedef struct {
  int socket;
  char name[NAME_LENGTH];
} Client;

Client clients[MAX_CLIENTS] = {0};
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(int sender_fd, const char *message) {
  char formatted_message[BUFFER_SIZE + NAME_LENGTH] = {0};
  char sender_name[NAME_LENGTH] = "Unknown";

  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i].socket == sender_fd) {
      strncpy(sender_name, clients[i].name, NAME_LENGTH);
      break;
    }
  }

  snprintf(formatted_message, sizeof(formatted_message), "%s: %s", sender_name,
           message);

  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i].socket != 0 && clients[i].socket != sender_fd) {
      send(clients[i].socket, formatted_message, strlen(formatted_message), 0);
    }
  }
  pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
  int client_fd = *(int *)arg;
  char buffer[BUFFER_SIZE];
  char name[NAME_LENGTH];

  memset(buffer, 0, BUFFER_SIZE);
  ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_received <= 0) {
    close(client_fd);
    return NULL;
  }

  buffer[strcspn(buffer, "\n")] = 0;
  strncpy(name, buffer, NAME_LENGTH);

  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i].socket == 0) {
      clients[i].socket = client_fd;
      strncpy(clients[i].name, name, NAME_LENGTH);
      break;
    }
  }
  pthread_mutex_unlock(&clients_mutex);

  printf("%s connected (fd: %d)\n", name, client_fd);
  broadcast_message(client_fd, "has joined the chat!\n");

  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
      printf("%s disconnected\n", name);
      pthread_mutex_lock(&clients_mutex);
      for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == client_fd) {
          clients[i].socket = 0;
          memset(clients[i].name, 0, NAME_LENGTH);
          break;
        }
      }
      pthread_mutex_unlock(&clients_mutex);
      broadcast_message(client_fd, "has left the chat.\n");
      close(client_fd);
      return NULL;
    }

    printf("%s: %s", name, buffer);
    broadcast_message(client_fd, buffer);
  }
}

int main() {
  int server_fd, new_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, MAX_CLIENTS) == -1) {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server is running on port %d. Waiting for connections...\n", PORT);

  while (1) {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr,
                             &addr_len)) == -1) {
      perror("accept failed");
      continue;
    }

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_client, (void *)&new_socket) !=
        0) {
      perror("pthread_create failed");
      close(new_socket);
    }
    pthread_detach(thread_id);
  }

  close(server_fd);
  return 0;
}