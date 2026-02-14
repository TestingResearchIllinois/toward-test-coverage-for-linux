// SPDX-License-Identifier: MIT

#include <stdio.h>

int addIf(int x, int y);

int main() {
  int x = 4;
  int y = 10;
  int result = addIf(x, y);
  printf("the result is %d\n", result);
  return 0;
}

int addIf(int x, int y) {
  if (x > 10) {
    return x + y;
  } else {
    return x;
  }
}
