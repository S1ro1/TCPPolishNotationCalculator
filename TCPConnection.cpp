//
// Created by matej on 4/16/2023.
//


#include "TCPConnection.h"

TCPConnection::TCPConnection(int socket_num) {
  this->socket_num = socket_num;
  this->status = INITIALIZED;
}

void TCPConnection::HandleConnection() {

  std::stringstream stream;

  std::string curr_message;
  // read from recv until we get a newline
  do {
    char buffer[1024] = {0};
    if (recv(socket_num, buffer, 1024, 0) <= 0) {
      status = DISCONNECTED;
      return;
    }
    buffer[1023] = '\0';
    curr_message = buffer;
    stream << buffer;
  } while (curr_message.find('\n') == std::string::npos);

  // iterate over all lines in stream
  for (std::string message; std::getline(stream, message); message.clear()) {
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
            if (result < 0) throw ParseException();
            std::string result_string = "RESULT " + std::to_string(result) + '\n';

            send(socket_num, result_string.c_str(), result_string.length(), 0);
          } catch (std::exception &e) {
            send(socket_num, "BYE\n", 4, 0);
            status = DISCONNECTED;
            break;
          }
        }
      default:
        break;
    }
  }
}
