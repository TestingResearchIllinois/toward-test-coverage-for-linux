// SPDX-License-Identifier: MIT

#include "ss1.h"
#include "ss2.h"
#include <stdio.h>

int main() {
  set_global1_next();
  consume_global1();
  call_global1();
  consume_global1_next();

  set_global2();
  consume_global2();
  call_global2();
}