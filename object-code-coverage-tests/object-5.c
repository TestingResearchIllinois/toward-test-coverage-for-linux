// SPDX-License-Identifier: MIT

#include <stdio.h>

int main() {
  int array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int sum = 0;

  for (int i = 0; i < 10; i++) {
    if (array[i] % 2 == 0) {
      sum += array[i];
    } else {
      sum += 2 * array[i];
    }
  }

  printf("Sum is: %d\n", sum);
  return 0;
}
