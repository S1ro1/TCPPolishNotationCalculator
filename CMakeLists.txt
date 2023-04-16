cmake_minimum_required(VERSION 3.25)
project(ipk_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

add_executable(ipk_parser main.cpp Token.cpp Token.h utils.h utils.cpp Parser.cpp Parser.h UDPConnection.cpp UDPConnection.h TCPServer.cpp TCPServer.h TCPConnection.cpp TCPConnection.h)
