//
// Created by matej on 4/14/2023.
//

#ifndef IPK_PARSER_UTILS_H
#define IPK_PARSER_UTILS_H

#include <cstring>
#include <iostream>
#include <tuple>

#include "TCPConnection.h"

#define log(x) std::cout << x << std::endl;

class TcpFormatException : public std::exception {
  public:
  char *what();
};

std::string check_tcp_format(std::string);

void exit_with_error(const char *msg, int error_code);

std::tuple<const char *, long, std::string> parse_arguments(char *argv[], int argc);

int check(int);

#endif//IPK_PARSER_UTILS_H
