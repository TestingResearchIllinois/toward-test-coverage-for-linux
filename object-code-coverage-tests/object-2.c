// SPDX-License-Identifier: MIT

#include <stdio.h>

void checkAndSet(int *arr, int n, int threshold) {
  for (int i = 0; i < n; ++i) {
    if (arr[i] < threshold) {
      arr[i] = threshold;
    }
  }
}

int main() {
  int data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  checkAndSet(data, 10, 5);

  for (int i = 0; i < 10; ++i) {
    printf("%d ", data[i]);
  }
  printf("\n");
  return 0;
}
