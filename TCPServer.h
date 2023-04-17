//
// Created by matej on 4/16/2023.
//

#ifndef IPK_PARSER_TCPSERVER_H
#define IPK_PARSER_TCPSERVER_H

#define MAX_CONNECTIONS 32

#include <arpa/inet.h>
#include <array>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <csignal>

#include "Parser.h"
#include "TCPConnection.h"
#include "utils.h"


class TCPServer {
  private:
  int server_socket;
  std::array<TCPConnection, MAX_CONNECTIONS> connections;

  public:
  TCPServer(const char *host_name, long port_num);
  ~TCPServer();
  void WaitConnections();
};


#endif//IPK_PARSER_TCPSERVER_H
