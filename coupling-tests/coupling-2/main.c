// SPDX-License-Identifier: MIT

#include "ss1.h"
#include "ss2.h"
#include "ss3.h"
#include <stdio.h>

int main() {
  set_global1_next();
  consume_global1();
  call_global1();
  consume_global1_next();
  read_global1();
  consume_global1_in_ss3();

  set_global2();
  consume_global2();
  set_global2_in_ss3();
  consume_global2_next();
  call_global2();

  set_global3();
  consume_global1_and_global3();
}