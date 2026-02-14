// SPDX-License-Identifier: MIT

#include "ss1.h"
#include "ss2.h"

int global1;

int set_global1() {
  global1 = 1;
  return 0;
}

int set_global1_next() {
  global1 = -1;
  return 0;
}

int read_global1() {
  int local = 0;
  if (global1 > 0) {
    local = 1;
  }
  return local;
}

int consume_global2() {
  int local = 0;
  if (global2 > 0) {
    local = 1;
  }
  return local;
}

int consume_global2_next() {
  int local = 0;
  if (global2 > 0) {
    local = 1;
  }
  return local;
}

int doSomething1() { return 1; }

int call_global2() {
  global2++;
  doSomething2();
  return doSomething1();
}

int call_global2_next() {
  global2 = -1;
  doSomething2();
  return doSomething1();
}