#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>

// server config
#define SOCK_PORT 2024
#define MAX_CONN_LIMIT 8

int main() {
  std::unordered_map<std::string, std::string> table;
  table.max_load_factor(1.3);

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
      int s = 0;
      while (true) {
        char buffer[128] = {0};
        int valread = read(client_fd, buffer, 128);

        if (valread > 0) {
          std::string request(buffer);
          std::string operation, key, value;
          // std::cout << "request: " << request << '\n';

          size_t pos = request.find(' ');
          operation = request.substr(0, pos);
          request.erase(0, pos + 1);

          if (operation == "put") {
            pos = request.find(' ');
            key = request.substr(0, pos);
            value = request.substr(pos + 1);
            table[key] = value;
            write(client_fd, "OK", 2);
          } else if (operation == "read") {
            pos = request.find(' ');
            key = request.substr(0, pos);
            auto find = table.find(key);
            if (find != table.end()) {
              auto ret =
                  write(client_fd, find->second.c_str(), find->second.length());
            } else {
              auto ret = write(client_fd, " ", 1);
            }
          } else if (operation == "restart") {
            table.clear();
            write(client_fd, "OK", 2);
          } else if (operation == "info") {
            std::cout << "bucket size: " << table.bucket_count() << '\n';
            write(client_fd, "OK", 2);
          } else if (operation == "kill") {
            exit(0);
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
