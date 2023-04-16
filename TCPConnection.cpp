//
// Created by matej on 4/16/2023.
//


#include "TCPConnection.h"

TCPConnection::TCPConnection(int socket_num) {
  this->socket_num = socket_num;
  this->status = INITIALIZED;
}

void TCPConnection::HandleConnection() {
  char buffer[1024] = {0};
  if (recv(socket_num, buffer, 1024, 0) <= 0) {
    status = DISCONNECTED;
    return;
  }
  buffer[1023] = '\0';

  std::string message{buffer};
  message.pop_back();

  switch (status) {
    case INITIALIZED:
      if (message != "HELLO") {
        send(socket_num, "BYE\n", 4, 0);
        status = DISCONNECTED;
        break;
      } else {
        send(socket_num, "HELLO\n", 6, 0);
        status = CONNECTED;
        break;
      }
    case CONNECTED:
      if (message == "BYE") {
        send(socket_num, "BYE\n", 4, 0);
        status = DISCONNECTED;
        break;
      } else {
        try {
          message = check_tcp_format(message);
          int result = Parser::parse_expr(message);
          std::string result_string = std::to_string(result) + '\n';

          send(socket_num, result_string.c_str(), result_string.length(), 0);
        } catch (std::exception &e) {
          send(socket_num, "BYE\n", 4, 0);
          break;
        }
      }
    default:
      break;
  }
}
