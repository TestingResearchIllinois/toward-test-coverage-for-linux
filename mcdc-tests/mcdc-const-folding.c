// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdlib.h>

bool case0(bool a) { return 0 && a; /* mcdc(0) */ }
bool case1(bool a) { return a && 0; /* mcdc(0) */ }
bool case2(bool a) { return 1 && a; /* mcdc(100) */ }
bool case3(bool a) { return a && 1; /* mcdc(100) */ }
bool case4(bool a) { return 1 || a; /* mcdc(0) */ }
bool case5(bool a) { return a || 1; /* mcdc(0) */ }
bool case6(bool a) { return 0 || a; /* mcdc(100) */ }
bool case7(bool a) { return a || 0; /* mcdc(100) */ }

bool case8(bool a, bool b) { return 0 && a && b; /* mcdc(0) */ }
bool case9(bool a, bool b) { return a && 0 && b; /* mcdc(0) */ }
bool casea(bool a, bool b) { return 1 && a && b; /* mcdc(100) */ }
bool caseb(bool a, bool b) { return a && 1 && b; /* mcdc(100) */ }
bool casec(bool a, bool b) { return 1 || a || b; /* mcdc(0) */ }
bool cased(bool a, bool b) { return a || 1 || b; /* mcdc(0) */ }
bool casee(bool a, bool b) { return 0 || a || b; /* mcdc(100) */ }
bool casef(bool a, bool b) { return a || 0 || b; /* mcdc(100) */ }

bool caseg(bool a, bool b) { return b && a && 0; /* mcdc(0) */ }
bool caseh(bool a, bool b) { return b && 0 && a; /* mcdc(0) */ }
bool casei(bool a, bool b) { return b && a && 1; /* mcdc(100) */ }
bool casej(bool a, bool b) { return b && 1 && a; /* mcdc(100) */ }
bool casek(bool a, bool b) { return b || a || 1; /* mcdc(0) */ }
bool casel(bool a, bool b) { return b || 1 || a; /* mcdc(0) */ }
bool casem(bool a, bool b) { return b || a || 0; /* mcdc(100) */ }
bool casen(bool a, bool b) { return b || 0 || a; /* mcdc(100) */ }

int main(int argc, char *argv[]) {
  bool a = atoi(argv[1]);
  bool b = atoi(argv[2]);
  volatile bool c;

  c = case0(a);
  c = case1(a);
  c = case2(a);
  c = case3(a);
  c = case4(a);
  c = case5(a);
  c = case6(a);
  c = case7(a);

  c = case8(a, b);
  c = case9(a, b);
  c = casea(a, b);
  c = caseb(a, b);
  c = casec(a, b);
  c = cased(a, b);
  c = casee(a, b);
  c = casef(a, b);

  c = caseg(a, b);
  c = caseh(a, b);
  c = casei(a, b);
  c = casej(a, b);
  c = casek(a, b);
  c = casel(a, b);
  c = casem(a, b);
  c = casen(a, b);
  return 0;
}
