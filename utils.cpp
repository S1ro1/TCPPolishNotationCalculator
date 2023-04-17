//
// Created by matej on 4/14/2023.
//

#include "utils.h"

char *TcpFormatException::what() {
  char const* msg = "Invalid TCP message format";
  return (char *)msg;
}

void exit_with_error(const char *msg, int error_code) {
  std::cerr << msg << std::endl;
  exit(error_code);
};


std::tuple<const char *, long, std::string> parse_arguments(char *argv[], int argc) {
  std::string mode;
  long port_number = 0;
  const char *server_hostname = nullptr;

  if (argc == 2) {
    if (strcmp(argv[1], "--help") == 0) {
      exit_with_error("Usage: ./ipkcpd [-h] [host] [-p] [port] [-m] [mode]", 0);
    }
    exit_with_error("Invalid args", 1);
  }

  if (argc != 7) {
    exit_with_error("Invalid args", 1);
  }

  if (strcmp(argv[1], "-h") != 0 or strcmp(argv[3], "-p") != 0 or strcmp(argv[5], "-m") != 0) {
    exit_with_error("Invalid args", 1);
  }

  server_hostname = argv[2];

  char *rest = nullptr;
  port_number = strtol(argv[4], &rest, 10);
  if (*rest || port_number < 0 || port_number > 65535) {
    exit_with_error("Invalid port number", 1);
  }
  mode = argv[6];

  if (mode != "udp" and mode != "tcp") {
    exit_with_error("Invalid mode", 1);
  }

  return std::make_tuple(server_hostname, port_number, mode);
}

// help function to reduce boiler plate with unix network functions
int check(int func) {
  if (func < 0) {
    perror("Error: ");
  }
  return func;
}

// function to check if message is in correct format and returns it stripped
std::string check_tcp_format(std::string msg) {
  if (!msg.starts_with("SOLVE ")) {
    throw TcpFormatException();
  }

  msg.erase(0, 6);
  return msg;
}