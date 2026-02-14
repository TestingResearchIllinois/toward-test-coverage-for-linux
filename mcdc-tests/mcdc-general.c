// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

extern int foo();

void test(bool a, bool b, bool c, bool d) {

  if ((a && b) || (c && d)) /* mcdc(100) */
    printf("test1 decision true\n");

  if (b && c)   /* mcdc(100) */
    if (a && d) /* mcdc(50) */
      printf("test2 decision true\n");

  if ((c && d) && (a && b)) /* mcdc(75) */
    printf("test3 decision true\n");
}

int main() {
  test(false, false, false, false);
  test(true, false, true, false);
  test(true, false, true, true);
  test(true, true, false, false);

  test(true, false, false, false);
  test(true, true, true, true);
  test(false, true, true, false);

  return 0;
}
