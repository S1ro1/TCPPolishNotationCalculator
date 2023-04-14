//
// Created by matej on 4/14/2023.
//

#include "Parser.h"
#include <numeric>

char *ParseException::what() {
  return "Couldn't parse expression!";
}

int make_op(int accum, const Token &token, char op) {
  switch (op) {
    case '*':
      return accum * token.val.value();
    case '/':
      return accum / token.val.value();
    case '+':
      return accum + token.val.value();
    case '-':
      return accum - token.val.value();
    default:
      exit_with_error("Invalid operator", 5);
  }
  return -1;
}

Parser::Parser() = default;

std::vector<Token>
Parser::reduce_stack(std::vector<Token> expr_stack) {
  auto lbrace = std::find(expr_stack.rbegin(), expr_stack.rend(), Token('('));
  if (lbrace == expr_stack.rbegin()) throw ParseException();

  std::vector<Token> prev_vec(expr_stack.begin(), lbrace.base() - 1);
  std::vector<Token> to_reduce(lbrace.base(), expr_stack.end());
  if (to_reduce.size() < 3) throw ParseException();

  Token op = to_reduce.at(0);

  if (!op.type.has_value()) throw ParseException();

  char op_val = op.type.value();
  int initial_val = to_reduce.at(1).val.value();
  int result = std::accumulate(to_reduce.begin() + 2, to_reduce.end(), initial_val, [op_val](int accum, const Token &token) { return make_op(accum, token, op_val); });

  prev_vec.emplace_back(result);

  return prev_vec;
}

int Parser::parse_expr(std::string &expr) {
  std::vector<Token> expr_stack;
  std::string curr_num;
  State state = T_START;
  int brace_count = 0;

  for (auto it = expr.begin(); it != expr.end(); ++it) {
    switch (state) {
      case T_START:
        if (*it == '(') {
          brace_count++;
          expr_stack.emplace_back(*it);
          state = T_LBRACE;
        } else {
          throw ParseException();
        }
        continue;
      case T_LBRACE:
        if (*it == '(') {
          brace_count++;
          expr_stack.emplace_back(*it);
          state = T_LBRACE;
        } else if (*it == '+' or *it == '-' or *it == '/' or *it == '*') {
          expr_stack.emplace_back(*it);
          state = T_OP;
        } else {
          throw ParseException();
        }
        continue;
      case T_OP:
        if (*it == '(') {
          brace_count++;
          expr_stack.emplace_back(*it);
          state = T_LBRACE;
        } else if (isspace(*it)) {
          state = T_CONTINUE_EXPR;
        } else {
          throw ParseException();
        }
        continue;
      case T_NUM:
        if (isdigit(*it)) {
          curr_num.push_back(*it);
        } else if (isspace(*it)) {
          int num = std::stol(curr_num);
          expr_stack.emplace_back(num);
          curr_num.clear();
          state = T_CONTINUE_EXPR;
        } else if (*it == ')') {
          brace_count--;
          if (brace_count < 0) {
            throw ParseException();
          }
          int num = std::stol(curr_num);
          expr_stack.emplace_back(num);
          curr_num.clear();
          expr_stack = reduce_stack(expr_stack);

          if (expr_stack.size() == 1 and brace_count == 0 and it + 1 == expr.end()) {
            return expr_stack.back().val.value();
          } else {
            state = T_RBRACE_CONTINUE;
          }
        } else {
          throw ParseException();
        }
        continue;
      case T_CONTINUE_EXPR:
        if (isdigit(*it)) {
          curr_num.push_back(*it);
          state = T_NUM;
        } else if (*it == '(') {
          brace_count++;
          expr_stack.emplace_back(*it);
          state = T_LBRACE;
        } else {
          throw ParseException();
        }
        continue;
      case T_RBRACE_CONTINUE:
        if (*it == ')') {
          brace_count--;
          if (brace_count < 0) {
            throw ParseException();
          }
          expr_stack = reduce_stack(expr_stack);
          if (expr_stack.size() == 1 and brace_count == 0 and it + 1 == expr.end()) {
            return expr_stack.back().val.value();
          } else {
            state = T_RBRACE_CONTINUE;
          }
        } else if (isdigit(*it)) {
          curr_num.push_back(*it);
          state = T_NUM;
        } else if (isspace(*it)) {
          state = T_CONTINUE_EXPR;
        } else {
          throw ParseException();
        }
        continue;
    }
  }

  throw ParseException();
}
