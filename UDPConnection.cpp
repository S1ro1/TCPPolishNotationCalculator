//
// Created by matej on 4/14/2023.
//

#include "UDPConnection.h"

UDPConnection::UDPConnection(const char *host, long port_num) {
  struct sockaddr_in server_address = {};

  // create socket
  int server_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_sock <= 0) perror("ERR: ");

  int opt = 1;
  setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(int));

  // init server_address
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons((unsigned short) port_num);

  if ((server_address.sin_addr.s_addr = inet_addr(host)) == INADDR_ANY) exit_with_error("Invalid host", 1);
  check(bind(server_sock, (struct sockaddr *) &server_address, sizeof(server_address)));

  // set server socket
  this->server_socket = server_sock;
};

void UDPConnection::listen() const {
  char buf[512] = {};
  struct sockaddr_in client_address = {};
  socklen_t client_len = 0;
  std::string result;
  char status_byte{};

  // run loop
  while (1) {
    client_len = sizeof(client_address);
    check(recvfrom(this->server_socket, buf, 512, 0, (struct sockaddr *) &client_address, &client_len));

    // message was a response
    if (buf[0] == '\1') {
      check(sendto(server_socket, "\01\01\05Error", 8, 0, (struct sockaddr *) &client_address, client_len));
      continue;
    }

    std::string message{buf + 2};
    int length = (uint8_t)buf[1];
    // truncate message to length specified in 2nd byte
    message = message.substr(0, length);
    try {
      int val = Parser::parse_expr(message);
      if (val < 0) throw ParseException();
      result = std::to_string(val);

      status_byte = '\0';
    } catch (ParseException &e) {
      result = std::string(e.what());
      status_byte = '\1';
    }

    // reply to client
    std::string reply{'\1', status_byte, (char) result.size()};
    reply += result;

    check(sendto(server_socket, reply.c_str(), reply.size(), 0, (struct sockaddr *) &client_address, client_len));

    bzero(buf, 512);
  }
}