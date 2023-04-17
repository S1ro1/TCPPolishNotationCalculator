//
// Created by matej on 4/14/2023.
//

#include "Token.h"
#include "utils.h"

Token::Token(int v) : val(v) {}

Token::Token(char t) : type(t){};

bool Token::operator==(const Token &other) const { return (this->type.has_value() && other.type.has_value() && other.type == this->type); }
