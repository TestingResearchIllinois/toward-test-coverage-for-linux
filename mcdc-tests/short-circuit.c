// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

void short_circuit(bool a, bool b) {
  if (a && b) {
    printf("short_circuit decision true\n");
  }
}

int main() {
  short_circuit(false, false);
  short_circuit(true, true);
}
