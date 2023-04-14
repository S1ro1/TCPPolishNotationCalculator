//
// Created by matej on 4/14/2023.
//

#include "Token.h"
#include "utils.h"

Token::Token(int v) : val(v) {}

Token::Token(char t) : type(t){};

bool Token::operator==(Token &other) const { return (this->type.has_value() && other.type.has_value() && other.type == this->type); }

int Token::operator*(Token &other) const {
  if (!this->val.has_value() or !other.val.has_value()) {
    exit_with_error("Invalid token", 3);
  };
  return this->val.value() * other.val.value();
}