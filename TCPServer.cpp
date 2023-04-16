//
// Created by matej on 4/16/2023.
//

#include "TCPServer.h"

TCPServer::TCPServer(const char *host_name, long port_num) {
  struct sockaddr_in server_address = {};
  int server_sock{};

  check((server_sock = socket(AF_INET, SOCK_STREAM, 0)));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons((unsigned short) port_num);
  if ((server_address.sin_addr.s_addr = inet_addr(host_name)) == INADDR_ANY) exit_with_error("Invalid host", 1);

  check(bind(server_sock, (struct sockaddr *) &server_address, sizeof(server_address)));

  check(listen(server_sock, MAX_CONNECTIONS));

  this->server_socket = server_sock;
  std::fill_n(this->connections.begin(), MAX_CONNECTIONS, TCPConnection());
}

void TCPServer::WaitConnections() {
  fd_set readfs;
  int addrlen{};
  struct sockaddr_in client_address {};

  while (1) {
    FD_ZERO(&readfs);
    FD_SET(this->server_socket, &readfs);
    int max_fd = this->server_socket;

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
      int sd = this->connections[i].socket_num;
      if (sd > 0) FD_SET(sd, &readfs);
      if (sd > max_fd) max_fd = sd;
    }

    int activity = select(max_fd + 1, &readfs, nullptr, nullptr, nullptr);

    if ((activity < 0) && (errno != EINTR)) {
      std::cout << "Select error" << std::endl;
    }

    if (FD_ISSET(this->server_socket, &readfs)) {
      int client_socket = accept(this->server_socket, (struct sockaddr *) &client_address, (socklen_t *) (&addrlen));
      check(client_socket);

      for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (this->connections[i].socket_num == 0) {
          this->connections[i] = TCPConnection(client_socket);
          break;
        }
      }
    }
    for (int i = 0; i < MAX_CONNECTIONS; i++) {

      if (this->connections[i].socket_num == 0) continue;
      if (FD_ISSET(this->connections[i].socket_num, &readfs)) this->connections[i].HandleConnection();

      if (this->connections[i].status == DISCONNECTED) {
        close(this->connections[i].socket_num);
        this->connections[i] = TCPConnection();
      }
    }
  }
}
