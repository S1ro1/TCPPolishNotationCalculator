//
// Created by matej on 4/14/2023.
//

#include "UDPConnection.h"

UDPConnection::UDPConnection(const char *host, long port_num) {
  struct sockaddr_in server_address = {};

  int server_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_sock <= 0) exit_with_error("Socket error", 1);

  int opt = 1;
  setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(int));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons((unsigned short) port_num);

  if ((server_address.sin_addr.s_addr = inet_addr(host)) == INADDR_ANY) exit_with_error("Invalid host", 1);
  check(bind(server_sock, (struct sockaddr *) &server_address, sizeof(server_address)));

  this->server_socket = server_sock;
};

void UDPConnection::listen() {
  char buf[512];
  int bytesrx;
  struct sockaddr_in client_address = {};
  socklen_t client_len = 0;

  while (1) {
    client_len = sizeof(client_address);
    check((bytesrx = recvfrom(this->server_socket, buf, 512, 0, (struct sockaddr *) &client_address, &client_len)));

    log(buf);

    bzero(buf, 512);
  }
}