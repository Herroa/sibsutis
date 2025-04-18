#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#define BUFFER_SIZE 1024

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

  int number;
  do {
    std::cout << "Input 1 to 10 ";
    std::cin >> number;
  } while (number < 1 || number > 10);

  char buffer[BUFFER_SIZE];
  while (true) {
    snprintf(buffer, BUFFER_SIZE, "%d", number);
    if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
      std::cerr << "Send data error\n";
      break;
    }
    std::cout << "Sent number: " << number << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(number));
  }

  close(client_socket);
  return 0;
}