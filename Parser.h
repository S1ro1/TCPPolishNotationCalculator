//
// Created by matej on 4/14/2023.
//

#ifndef IPK_PARSER_PARSER_H
#define IPK_PARSER_PARSER_H

#include <string>
#include <vector>

#include "Token.h"
#include "utils.h"

enum State {
  T_START,
  T_LBRACE,
  T_OP,
  T_NUM,
  T_CONTINUE_EXPR,
  T_RBRACE_CONTINUE,
};

class ParseException : public std::exception {
  public:
  char *what();
};

class Parser {
  public:
  Parser();
  static int parse_expr(std::string &expr);

  private:
  static std::vector<Token> reduce_stack(std::vector<Token> expr);
};


#endif//IPK_PARSER_PARSER_H
