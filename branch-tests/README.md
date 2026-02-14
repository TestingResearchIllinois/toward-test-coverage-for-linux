<!--
SPDX-License-Identifier: MIT
-->

# Tests for Branch Coverage

This directory contains tests for branch coverage.
They include all `gcov-*.c` files (33 in total) collected from
[the unit tests for GCC](https://github.com/gcc-mirror/gcc/tree/releases/gcc-11.4.0/gcc/testsuite/gcc.misc-tests)
(release 11.4.0), and one (`gcov-mytest.c`) crafted by ourselves.

## Prerequisites

There should be no *strict* software version requirements for these tests.
One can produce similar results as long as they're using a moderately updated toolchain.
The following instructions are tested with:

- Architecture: x86_64
- Distro: Ubuntu 22.04
- Kernel: 5.15.0-86-generic

Install dependencies:

```shell
sudo apt install build-essential clang llvm lld
```

Verify installations:

```shell
gcc --version
```

```text
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

```shell
clang --version
```

```text
Ubuntu clang version 14.0.0-1ubuntu1.1
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

## Build

Build with any of the three {compiler} - {coverage mechanism} combinations:

### 1. GCC-GCOV

```shell
make CC=gcc   COV=gcov
```

which internally invokes `gcc -fprofile-arcs -ftest-coverage` to compile each program.

### 2. Clang-GCOV

```shell
make CC=clang COV=gcov
```

which internally invokes `clang -fprofile-arcs -ftest-coverage` to compile each program.

### 3. Clang-SCC

```shell
make CC=clang COV=scc
```

which internally invokes `clang -fcoverage-mapping -fprofile-instr-generate` to compile each program.

## Collect Coverage Data

Execute the built program, e.g. `gcov-1`:

```shell
LLVM_PROFILE_FILE="gcov-1.profraw" ./gcov-1
```

where the environment variable `LLVM_PROFILE_FILE` is only used by source-based code coverage (SCC) and would be ignored by GCOV.

Next, we'll parse the generated `.gcda` files or `.profraw` files, depending on the coverage mechanism being used.

### 1. GCC-GCOV

For this {compiler} - {coverage mechanism} combination, `.gcno` files will be generated at compile-time and `.gcda` files will be generated at run-time. Run the following to parse the results:

```shell
gcov gcov-1.c
```

Example terminal output:

```text
File 'gcov-1.c'
Lines executed:100.00% of 6
Creating 'gcov-1.c.gcov'

Lines executed:100.00% of 6
```

At the same time, a `.gcov` will be generated, where the source code is displayed along with the execution counters. Here's an example of `gcov-1.c.gcov`:

```c
        -:    0:Source:gcov-1.c
        -:    0:Graph:gcov-1.gcno
        -:    0:Data:gcov-1.gcda
        -:    0:Runs:1
        -:    1:/* Test Gcov basics.  */
        -:    2:
        -:    3:/* { dg-options "-fprofile-arcs -ftest-coverage" } */
        -:    4:/* { dg-do run { target native } } */
        -:    5:
       10:    6:void noop ()
        -:    7:{
       10:    8:}
        -:    9:
        1:   10:int main ()
        -:   11:{
        -:   12:  int i;
        -:   13:
       11:   14:  for (i = 0; i < 10; i++)      /* count(11) */
       10:   15:    noop ();                    /* count(10) */
        -:   16:
        1:   17:  return 0;                     /* count(1) */
        -:   18:}
        -:   19:
        -:   20:/* { dg-final { run-gcov gcov-1.c } } */
```

### 2. Clang-GCOV

For this {compiler} - {coverage mechanism} combination, `.gcno` files will be generated at compile-time and `.gcda` files will be generated at run-time. The only difference to the previous case is we'll use `llvm-cov gcov` to to parse the results:

```shell
llvm-cov gcov gcov-1.c
```

Example terminal output:

```text
File 'gcov-1.c'
Lines executed:100.00% of 6
Creating 'gcov-1.c.gcov'
```

At the same time, a `.gcov` will be generated, where the source code is displayed along with the execution counters. Here's an example of `gcov-1.c.gcov`:

```c
        -:    0:Source:gcov-1.c
        -:    0:Graph:gcov-1.gcno
        -:    0:Data:gcov-1.gcda
        -:    0:Runs:1
        -:    0:Programs:1
        -:    1:/* Test Gcov basics.  */
        -:    2:
        -:    3:/* { dg-options "-fprofile-arcs -ftest-coverage" } */
        -:    4:/* { dg-do run { target native } } */
        -:    5:
       10:    6:void noop ()
        -:    7:{
       10:    8:}
        -:    9:
        1:   10:int main ()
        -:   11:{
        -:   12:  int i;
        -:   13:
       11:   14:  for (i = 0; i < 10; i++)      /* count(11) */
       10:   15:    noop ();                    /* count(10) */
        -:   16:
        1:   17:  return 0;                     /* count(1) */
        -:   18:}
        -:   19:
        -:   20:/* { dg-final { run-gcov gcov-1.c } } */
```

### 3. Clang-SCC

For this {compiler} - {coverage mechanism} combination, the steps are drastically different than the previous two cases: `.profraw` files will be generated at run-time. Then we'll convert them into `.profdata` files:

```shell
llvm-profdata merge gcov-1.profraw -o gcov-1.profdata
```

Read the profile data:

```shell
llvm-cov show ./gcov-1 -instr-profile=gcov-1.profdata
```

Example terminal output, where the source code is displayed along with the execution counters:

```c
    1|       |/* Test Gcov basics.  */
    2|       |
    3|       |/* { dg-options "-fprofile-arcs -ftest-coverage" } */
    4|       |/* { dg-do run { target native } } */
    5|       |
    6|       |void noop ()
    7|     10|{
    8|     10|}
    9|       |
   10|       |int main ()
   11|      1|{
   12|      1|  int i;
   13|       |
   14|     11|  for (i = 0; i < 10; i++)        /* count(11) */
   15|     10|    noop ();                      /* count(10) */
   16|       |
   17|      1|  return 0;                       /* count(1) */
   18|      1|}
   19|       |
   20|       |/* { dg-final { run-gcov gcov-1.c } } */
```

## Check Coverage Consistency

For each test, the expected branch and line coverage are labelled in the source code. Please see [Format of Unit Test](#format-of-unit-test) for more details.

> [!NOTE]
> Later sections only apply to GCC-GCOV

### Unit Test Framework in GCC

The unit tests for GCOV are put under [`gcc/testsuite/gcc.misc-tests`](https://github.com/gcc-mirror/gcc/tree/releases/gcc-11.4.0/gcc/testsuite/gcc.misc-tests) of GCC source tree.

The testing framework of GCOV can only check for combinations of line counts, branch percentages, and call return percentages.

Here are the instructions to run these unit tests (tested with the same setup in [this section](#prerequisites)):

<!-- FIXME: below there're duplicate dependencies. Verify the minimum on a fresh machine. -->

```bash
# Install extra dependencies.
sudo apt-get install build-essential gcc g++ make bison flex libgmp-dev libmpfr-dev libmpc-dev dejagnu autogen

# Clone the GCC source code.
git clone https://github.com/gcc-mirror/gcc --branch releases/gcc-11.4.0 --depth 1
cd gcc

# Create the build dir for GCC.
mkdir gcc-build
cd gcc-build

# Configure the GCC makefile.
../configure --enable-languages=c,c++ --disable-multilib

# Build GCC.
make -j$(nproc)

# Execute the GCOV tests.
make check-gcc RUNTESTFLAGS="gcov.exp"
```

The results would be saved in `<gcc-source-root>/gcc-build/gcc/testsuite/gcc/gcc.sum` and `<gcc-source-root>/gcc-build/gcc/testsuite/g++/g++.sum`.

Example `gcc.sum` file:

```text
...
PASS: gcc.misc-tests/gcov-pr90574-2.c gcov
PASS: gcc.misc-tests/gcov-pr94029.c (test for excess errors)
PASS: gcc.misc-tests/gcov-pr94029.c gcov

                === gcc Summary ===

# of expected passes            233
# of expected failures          2
<gcc-source-root>/gcc-build/gcc/xgcc  version 11.4.0 (GCC)
```

It's OK to see `PASS` records (expected success) and `XFAIL` records (expected failure).
Or according to the summary in the end, we can tell every test case ended up as expected.

### Format of Unit Tests

GCC unit tests are written in a format where the expected line or branch counts are specified within comments alongside the source code. Here is an example:

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
  if (i < 0)                            /* count(5) */
                                        /* branch(end) */
                                        /* returns(0) */
    exit (0);                           /* count(1) */
                                        /* returns(end) */
  val += i;                             /* count(4) */
}

int
main()
{
  int i;

                                        /* returns(100) */
  foo (100);                            /* count(1) */
                                        /* returns(end) */
  for (i = 2; i > -10; i--)
                                        /* returns(75) */
    foo (i);                            /* count(4) */
                                        /* returns(end) */
}

/* { dg-final { run-gcov branches calls { -b gcov-6.c } } } */
```

Documentation for this format can be found [here](https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gccint/Final-Actions.html#Checks-for-gcov-tests).
