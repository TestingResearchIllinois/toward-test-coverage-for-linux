// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

bool foo(bool x) { return !x; }

void split_nested(bool a, bool b, bool c, bool d) {
  if (a && b && foo(c || d)) {
    printf("split_nested decision true\n");
  }
}

int main() {
  split_nested(true, true, true, false);
  split_nested(true, true, false, false);
}
