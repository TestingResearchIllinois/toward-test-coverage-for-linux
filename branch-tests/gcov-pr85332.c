// SPDX-License-Identifier: MIT

/* { dg-options "-fprofile-arcs -ftest-coverage" } */
/* { dg-do run { target native } } */

int doit(int sel, int n, void *p)
{
  int * const p0 = p;

  switch (sel)
  {
    case 0: /* count(3) */
      do {*p0 += *p0;} while (--n); /* count(3) */
      return *p0 == 0; /* count(1) */

    default:
      __builtin_abort ();
  }
}

int main()
{
  int v0;
  v0 = 1; doit(0, 3, &v0);
#if defined(__GNUC__) && !defined(__clang__)
  __builtin_exit (0);
#else
  return 0;
#endif
}

/* { dg-final { run-gcov gcov-pr85332.c } } */
