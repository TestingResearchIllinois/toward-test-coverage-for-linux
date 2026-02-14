// SPDX-License-Identifier: MIT

#include "ss1.h"
#include "ss2.h"

int global3;

int set_global2_in_ss3() {
  global2 = 1;
  return 0;
}

int set_global3() {
  global3 = 1;
  return 0;
}

int consume_global1_in_ss3() {
  int local = 0;
  if (global1 & 1) {
    local = 1;
  } else if (global1 & 2) {
    local = 2;
  } else if (global1 & 4) {
    local = 3;
  }
  int temp = global1;
  return local + temp;
}

int doSomething3() { return 1; }

int call_global3() {
  global1++;
  doSomething1();
  return doSomething2();
}