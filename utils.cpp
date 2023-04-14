//
// Created by matej on 4/14/2023.
//

#include "utils.h"
#include <iostream>

void exit_with_error(const char *msg, int error_code) {
  std::cerr << msg << std::endl;
  exit(error_code);
}
