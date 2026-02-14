// SPDX-License-Identifier: MIT

#include "ss2.h"
#include "ss1.h"

int global2;

int set_global2() {
  global2 = 1;
  return 0;
}

int consume_global1() {
  int local = 0;
  if (global1 > 0) {
    local = 1;
  }
  return local;
}

int consume_global1_next() {
  int local = 0;
  if (global1 < 0) {
    local = 1;
  }
  return local;
}

int doSomething2() { return 1; }

int call_global1() {
  global1++;
  doSomething1();
  return doSomething2();
}