// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

struct s {
  int f;
};

void short_circuit(struct s *p) {
  if (p != NULL && p->f > 0) {
    printf("short_circuit decision true\n");
  } // when first condition is false, second can't be even evaluated
}

int main() { short_circuit(NULL); }
