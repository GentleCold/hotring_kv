#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "utils.hpp"

#define SOCK_PORT 2024
#define NUM_KEYS 50000

void five_minute_run(DataSet &data, double hot_ratio, int client_fd,
                     std::ofstream &output_file) {
  data.transform(hot_ratio);
  for (int minute = 0; minute < 5; minute++) {
    int minute_load_count = 0;
    auto start = std::chrono::system_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now() - start)
               .count() <= 60000) {  // per minute
      // 1. put opera
      auto kv = data.get();
      std::string request = "put " + kv.first + " " + kv.second;
      write(client_fd, request.c_str(), request.length());

      char buffer[3] = {0};
      read(client_fd, buffer, 2);
      if (std::string(buffer) == "OK") {
        // 2. read opera
        kv = data.get();
        request = "read " + kv.first + " ";
        write(client_fd, request.c_str(), request.length());

        char buffer_read[128] = {0};
        read(client_fd, buffer_read, 128);
        minute_load_count++;
      } else {
        exit(1);
      }
      // std::cout << minute_load_count << '\n';
    }

    std::cout << "minute load count: " << minute_load_count << '\n';
    output_file << minute_load_count << '\n';
  }

  // restart kv store
  std::string request = "restart ";
  write(client_fd, request.c_str(), request.length());
  char buffer[3] = {0};
  read(client_fd, buffer, 2);
  if (std::string(buffer) != "OK") {
    exit(1);
  };
}

int main(int argc, char *argv[]) {
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

  // prepare for dataset
  DataSet data(NUM_KEYS);

  // 5-minute loop
  std::string file_name = "./utils/" + std::string(argv[1]);

  std::cout << "start test1..." << '\n';
  std::ofstream output_file_1(file_name + "_20.txt", std::ios::out);
  five_minute_run(data, 0.2, client_fd, output_file_1);
  output_file_1.close();

  std::cout << "start test2..." << '\n';
  std::ofstream output_file_2(file_name + "_40.txt", std::ios::out);
  five_minute_run(data, 0.4, client_fd, output_file_2);
  output_file_2.close();

  std::cout << "start test3..." << '\n';
  std::ofstream output_file_3(file_name + "_100.txt", std::ios::out);
  five_minute_run(data, 1.0, client_fd, output_file_3);
  output_file_3.close();

  close(client_fd);

  return 0;
}
