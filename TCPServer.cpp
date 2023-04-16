//
// Created by matej on 4/16/2023.
//

#include "TCPServer.h"

void sig_handler(int signo) {
  run = 0;
}

TCPServer::TCPServer(const char *host_name, long port_num) {
  struct sockaddr_in server_address = {};
  int server_sock{};
  signal(SIGINT, sig_handler);

  check((server_sock = socket(AF_INET, SOCK_STREAM, 0)));
  int optval = 1;
  setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

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

  sigset_t zeromask;

  sigemptyset(&zeromask);

  while (run) {
    FD_ZERO(&readfs);
    FD_SET(this->server_socket, &readfs);
    int max_fd = this->server_socket;

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
      int sd = this->connections[i].socket_num;
      if (sd > 0) FD_SET(sd, &readfs);
      if (sd > max_fd) max_fd = sd;
    }

    int activity = pselect(max_fd + 1, &readfs, nullptr, nullptr, nullptr, &zeromask);

    if (!run) {
      delete this;
      return;
    }

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
TCPServer::~TCPServer() {
  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    if (this->connections[i].socket_num != 0) {
      log(i);
      send(this->connections[i].socket_num, "BYE\n", 4, 0);
      close(this->connections[i].socket_num);
    }
  }
  log("destroying server");
  log(this->server_socket);
  close(this->server_socket);
  log(this->server_socket);

  int error = 0;
  socklen_t len = sizeof (error);
  int retval = getsockopt (this->server_socket, SOL_SOCKET, SO_ERROR, &error, &len);

  if (retval != 0) {
    /* there was a problem getting the error code */
    fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
    return;
  }

  if (error != 0) {
    /* socket has a non zero error status */
    fprintf(stderr, "socket error: %s\n", strerror(error));
  }
}
