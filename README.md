<!--
SPDX-License-Identifier: MIT
-->

This repository releases some of our older work toward evaluating test
coverage tools on the Linux kernel. This project was a collaboration
between the University of Illinois and The Boeing Company.

More recent results can be found in the following papers:

1. Wentao Zhang, Andrew Oppelt, Ikjun Jeon, Minji Park, Steven H. VanderLeest, and Chuck Wolber

   An Open-Source Structural Coverage Tool for DO-178C Compliance

   44th AIAA DATC/IEEE Digital Avionics Systems Conference
   
   (DASC 2025), Montreal, Canada, September 2025

   [paper](https://ieeexplore.ieee.org/document/11257174)

2. Wentao Zhang, Jinghao Jia, Erkai Yu, Darko Marinov, and Tianyin Xu

   DebCovDiff: Differential Testing of Coverage Measurement Tools on Real-World Projects

   40th IEEE/ACM Conference on Automated Software Engineering

   (ASE 2025), pages 1082-1093, Seoul, South Korea, November 2025

   [paper](https://tianyin.github.io/pub/covdiff.pdf)

## Current GCOV Unit Tests

### Overview

The unit tests for GCov are in the [gcc/testsuite/gcc.misc-tests](https://github.com/gcc-mirror/gcc/tree/master/gcc/testsuite/gcc.misc-tests).

The testing framework of GCov can only check for combinations of line counts, branch percentages, and call return percentages.

Here are the command lines to run execute it:

```bash
# Install the dependencies.
sudo apt-get install  build-essential gcc g++ make bison flex libgmp-dev libmpfr-dev libmpc-dev dejagnu autogen

# Clone the GCC source code.
git clone https://github.com/gcc-mirror/gcc
cd gcc

# Create the build dir for GCC.
mkdir gcc-build
cd gcc-build

# Configure the GCC makefile.
../configure  --enable-languages=c,c++ --disable-multilib

# Build GCC.
make -j 20

# Execute the GCOV tests.
make check RUNTESTFLAGS="gcov.exp" 
```

### Format of Unit Test

The unit tests are written in a format that the expected line or branch count is specified in the comments of the source code. Here is an example:

```c
/* Check that call return percentages are reported correctly by gcov,
   along with line counts and branch percentages.  This test case is
   meant to be simple, as it was added at the same time that checking
   for call return percentages was added.  */

/* { dg-options "-fprofile-arcs -ftest-coverage" } */
/* { dg-do run { target native } } */

extern void exit (int);

int val;

void
foo (int i)
{
  					/* branch(80) */
  if (i < 0)				/* count(5) */
    					/* branch(end) */
    					/* returns(0) */
    exit (0);				/* count(1) */
  					/* returns(end) */
  val += i;				/* count(4) */
}

int
main()
{
  int i;

  					/* returns(100) */
  foo (100);				/* count(1) */
  					/* returns(end) */
  for (i = 2; i > -10; i--)
    					/* returns(75) */
    foo (i);				/* count(4) */
  					/* returns(end) */
}

/* { dg-final { run-gcov branches calls { -b gcov-6.c } } } */
```

Document for the format of the unit test can be found [here](https://github.com/gcc-mirror/gcc/blob/0cfc9c953d0221ec3971a25e6509ebe1041f142e/gcc/doc/sourcebuild.texi#L3460).

### MC/DC Coverage

Some of unit tests for GCov have complex conditions and decisions, which could be used to test the MC/DC coverage.

For example, [pr55980.c](https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.misc-tests/gcov-pr84758.c) has a complex condition:

```c
/* { dg-options "-fprofile-arcs -ftest-coverage" } */
/* { dg-do run { target native } } */

int x, y;

static void
foo (int a, int b)
{
  {
    if (a == 1 || a == 2)  /* count(1) */
      {
	x = 4;  /* count(1) */
	if (b == 3)  /* count(1) */
	  x = 6;  /* count(1) */
      }
    else
      x = 15;  /* count(#####) */
  }
}

int
main (void)
{
  foo (2, 3);
  return 0;
}

/* { dg-final { run-gcov gcov-pr84758.c } } */
```

Besides, [gcov-7.c](https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.misc-tests/gcov-7.c), [gcov-4b.c](https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.misc-tests/gcov-4b.c) and [gcov-4.c](https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.misc-tests/gcov-4.c) have a complex decision.
