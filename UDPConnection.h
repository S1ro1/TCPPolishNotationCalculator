//
// Created by matej on 4/14/2023.
//

#ifndef IPK_PARSER_UDPCONNECTION_H
#define IPK_PARSER_UDPCONNECTION_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "utils.h"
#include "Parser.h"

class UDPConnection {
  private:
  int server_socket;

  public:
  UDPConnection(const char *host_name, long port_num);
  void listen() const;
};


#endif//IPK_PARSER_UDPCONNECTION_H
