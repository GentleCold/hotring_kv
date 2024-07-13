#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <iostream>
#include <string>

#define SOCK_PORT 2024

int main() {
  // create socket
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(client_fd != -1);

  // set address
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(SOCK_PORT);

  // connect
  connect(client_fd, (struct sockaddr *)&address, sizeof(address));

  std::string operation, key, value;
  while (true) {
    std::cout << "Enter operation (PUT/READ) and key/value: ";
    std::cin >> operation;

    std::transform(operation.begin(), operation.end(), operation.begin(),
                   ::tolower);

    if (operation == "put") {
      std::cin >> key >> value;
      std::string request = operation + " " + key + " " + value;
      write(client_fd, request.c_str(), request.length());

      char buffer[3] = {0};
      read(client_fd, buffer, 2);
      if (std::string(buffer) == "OK") {
        std::cout << "Put successful" << std::endl;
      } else {
        std::cerr << "Put failed" << std::endl;
      }
    } else if (operation == "read") {
      std::cin >> key;
      std::string request = operation + " " + key + " ";
      write(client_fd, request.c_str(), request.length());
      char buffer[1024] = {0};
      int valread = read(client_fd, buffer, 1024);
      if (valread > 0) {
        std::cout << "Read value: " << buffer << std::endl;
      } else {
        std::cerr << "Read failed" << std::endl;
      }
    } else {
      std::cerr << "Unknown operation" << std::endl;
    }
  }

  close(client_fd);
  return 0;
}
