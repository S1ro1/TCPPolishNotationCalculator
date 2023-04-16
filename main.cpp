#include <deque>
#include <string>

#include "TCPServer.h"
#include "UDPConnection.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  auto [server_host, port_number, mode] = parse_arguments(argv, argc);

  if (mode == "udp") {
    UDPConnection connection(server_host, port_number);
    connection.listen();
  } else {
    TCPServer *server = new TCPServer(server_host, port_number);
    server->WaitConnections();
  }
}
