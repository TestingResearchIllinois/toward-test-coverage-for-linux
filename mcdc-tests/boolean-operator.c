// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

void biimplication(bool a, bool b) {
  if (a == b) {
    printf("biimplication decision true\n");
  } // should == be a logical operator by the MC/DC definition?
}

void bitwise_op(int a, int b) {
  if (a | b) {
    printf("bitwise_op decision true\n");
  } // should bitwise operators be treated as logical by the MC/DC definition?
}

int main() {
  biimplication(false, true);
  biimplication(true, true);

  bitwise_op(0, 0);
  bitwise_op(1, 0);
}
