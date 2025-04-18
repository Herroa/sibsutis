#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#define MSG_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>\n";
    return 1;
  }

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    std::cerr << "Socket creation error\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(std::atoi(argv[2]));

  if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
    std::cerr << "Wrong ip\n";
    close(client_socket);
    return 1;
  }

  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    std::cerr << "Connection error\n";
    close(client_socket);
    return 1;
  }

  char msg[MSG_SIZE] = "test\n";

  if (send(client_socket, msg, sizeof(msg), 0) == -1) {
    std::cerr << "Send data error\n";
  }

  close(client_socket);
  return 0;
}