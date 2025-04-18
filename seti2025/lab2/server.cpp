#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CLIENTS 10

void handle_client(int client_socket) {
  char buffer[1024];
  int n;

  while ((n = read(client_socket, buffer, sizeof(buffer))) > 0) {
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);
  }

  close(client_socket);
  exit(0);
}

void zombie_handler(int sig) {
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  socklen_t server_len = sizeof(server_addr);
  pid_t pid;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
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

  if (getsockname(server_socket, (struct sockaddr *)&server_addr, &server_len) <
      0) {
    perror("getsockname failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port %d...\n", ntohs(server_addr.sin_port));

  if (listen(server_socket, MAX_CLIENTS) < 0) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  // signal(SIGCHLD, zombie_handler);

  while (1) {
    client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
      perror("Accept failed");
      continue;
    }

    pid = fork();
    if (pid < 0) {
      perror("Fork failed");
      close(client_socket);
    } else if (pid == 0) {
      close(server_socket);
      handle_client(client_socket);
    } else {
      close(client_socket);
    }
  }

  close(server_socket);
  return 0;
}