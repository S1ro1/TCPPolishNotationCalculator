//
// Created by matej on 4/14/2023.
//

#ifndef IPK_PARSER_TOKEN_H
#define IPK_PARSER_TOKEN_H

#include <optional>
struct Token {
  public:
  std::optional<int> val;
  std::optional<char> type;

  explicit Token(int v);

  explicit Token(char t);

  bool operator==(const Token &other) const;

  int operator*(Token &other) const;
};


#endif//IPK_PARSER_TOKEN_H
