//
// Created by matej on 4/16/2023.
//

#ifndef IPK_PARSER_TCPCONNECTION_H
#define IPK_PARSER_TCPCONNECTION_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Parser.h"
#include "utils.h"

enum ConnectionStatus {
  NOT_CONNECTED,
  INITIALIZED,
  CONNECTED,
  DISCONNECTED,
};


class TCPConnection {
  public:
  int socket_num;
  ConnectionStatus status;

  TCPConnection() = default;
  explicit TCPConnection(int);
  void HandleConnection();
};


#endif//IPK_PARSER_TCPCONNECTION_H
