#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cctype>
#include <iostream>
#include <string>

#include "hotring.hpp"

#define SOCK_PORT 8000
#define MAX_CONN_LIMIT 8

int main() {
  hotring::HotRing ring(8);  // 2^8 = 256

  // create socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(server_fd != -1);

  // set address
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(SOCK_PORT);

  // listen
  bind(server_fd, (struct sockaddr*)&address, sizeof(address));
  listen(server_fd, MAX_CONN_LIMIT);
  std::cout << "Server is listening...\n";

  while (true) {
    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
      continue;
    } else {
      while (true) {
        char buffer[1024] = {0};
        int valread = read(client_fd, buffer, 1024);

        if (valread > 0) {
          std::string request(buffer);
          std::string operation, key, value;

          size_t pos = request.find(' ');
          operation = request.substr(0, pos);
          request.erase(0, pos + 1);
          pos = request.find(' ');
          key = request.substr(0, pos);

          if (operation == "put") {
            value = request.substr(pos + 1);
            ring.put(key, value);
            write(client_fd, "OK", 2);
          } else {
            auto result = ring.read(key);
            if (result.first) {
              write(client_fd, result.second.c_str(), result.second.length());
            } else {
              write(client_fd, "", 1);
            }
          }
        } else {
          break;
        }
      }
      close(client_fd);
    }
  }

  close(server_fd);
  return 0;
}
