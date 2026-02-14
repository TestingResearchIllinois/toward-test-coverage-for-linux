// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

void a_and_not_a(bool a, bool b) {
  bool c = a || b;
  if ((a || b) && (c || !a)) {
    printf("A and not A decision true\n");
  }
}

int main() {
  // a_and_not_a(false, true);
  a_and_not_a(true, true);
  a_and_not_a(true, false);
  a_and_not_a(false, false);
}
