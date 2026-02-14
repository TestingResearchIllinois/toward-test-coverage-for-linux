// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

void bool_exp(bool a, bool b) {
  bool c = a && b; // should MC/DC be measured for each boolean expression?
  if (c) {
    printf("bool_exp decision true\n");
  }
}

int main() {
  bool_exp(false, true);
  bool_exp(true, true);
}
