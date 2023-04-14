#include <deque>
#include <functional>
#include <iostream>
#include <string>

#include "Parser.h"

int main() {
  std::string expr{"(+ (+ 1 2) 3)"};
  std::string expr2{"(+ 3 4)"};

  int result = Parser::parse_expr(expr);
  int result2 = Parser::parse_expr(expr2);

  log(result);
  log(result2);
}
