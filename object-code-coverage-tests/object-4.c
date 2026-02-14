// SPDX-License-Identifier: MIT

#include <stdio.h>

int compute_sum(int n) {
  int sum = 0;
  for (int i = 1; i <= n; ++i) {
    sum += i;
  }
  return sum;
}

int main() {
  int result = compute_sum(4);
  printf("Result: %d\n", result);
  return 0;
}
