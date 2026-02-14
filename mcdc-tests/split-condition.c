// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

void bool_exp(bool y, bool z, bool u, bool v) {
  if ((y && z) || (u && v)) {
    printf("bool_exp decision true\n");
  }
}

void bool_exp_splited(bool y, bool z, bool u, bool v) {
  bool x = y && z;
  bool w = u && v;
  if (x || w) {
    printf("bool_exp_splited decision true\n");
  }
}

int main() {
  bool_exp(false, true, true, true);
  bool_exp(true, false, true, false);

  bool_exp_splited(false, true, true, true);
  bool_exp_splited(true, false, true, false);
}
