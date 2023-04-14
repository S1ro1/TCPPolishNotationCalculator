//
// Created by matej on 4/14/2023.
//

#include "Parser.h"
#include <numeric>

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
  if (lbrace == expr_stack.rbegin()) exit_with_error("Didn't find (", -1);

  std::vector<Token> prev_vec(expr_stack.begin(), lbrace.base() - 1);
  std::vector<Token> to_reduce(lbrace.base(), expr_stack.end());
  if (to_reduce.size() < 3) exit_with_error("Too short of a expression", 4);

  Token op = to_reduce.at(0);

  if (!op.type.has_value()) exit_with_error("Invalid first token", 4);

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
          exit_with_error("Invalid token", 1);
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
          exit_with_error("Invalid token", 1);
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
          exit_with_error("Invalid token", 2);
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
            exit_with_error("Invalid braces", 11);
          }
          int num = std::stol(curr_num);
          expr_stack.emplace_back(num);
          curr_num.clear();
          expr_stack = reduce_stack(expr_stack);

          auto it2 = it;
          if (expr_stack.size() == 1 and brace_count == 0 and ++it2 == expr.end()) {
            return expr_stack.back().val.value();
          } else {
            state = T_RBRACE_CONTINUE;
          }
        } else {
          exit_with_error("Invalid token", 3);
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
          exit_with_error("Invalid token", 4);
        }
        continue;
      case T_RBRACE_CONTINUE:
        if (*it == ')') {
          brace_count--;
          if (brace_count < 0) {
            exit_with_error("Invalid braces", 11);
          }
          expr_stack = reduce_stack(expr_stack);
          auto it2 = it;
          if (expr_stack.size() == 1 and brace_count == 0 and ++it2 == expr.end()) {
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
          exit_with_error("Invalid token", 5);
        }
        continue;
    }
  }

  exit_with_error("Couldn't parse lol", 42);
  return 0;
}
