<!--
SPDX-License-Identifier: MIT
-->

# Tests for Object Code Coverage

We create 5 example programs for measuring object code coverage.
In all examples, the machine code would have different branches than the source code if compiled with certain optimization levels.

<!-- TODO a script to demonstrate that? -->

<!-- What's the point of the old "How to Run section" and Makefile?
if they don't measure object code coverage? -->

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

## Use LLVM to Collect Object Code Coverage

From [LLVM Discourse](https://discourse.llvm.org/t/how-to-collect-the-coverage-of-branches-after-optimization-in-llvm/75379):

> "Ultimately, the goal of Branch Coverage, as a specific part of Source-based Code Coverage, is to show you the coverage of source code. As such, it is always tied back to source code with an accurate report of a condition’s evaluation, regardless of how well the generated code is optimized (though it will tell you if a condition is constant-folded and removed)"

By default, LLVM will not collect object code coverage.

Fortunately, we can use another type of profiling to collect the object code coverage based on the [discussion](https://discourse.llvm.org/t/how-to-collect-the-coverage-of-branches-after-optimization-in-llvm/75379/9), [tutorial video](https://www.youtube.com/watch?v=vFWwJrOiVMM) and [doc](https://releases.llvm.org/14.0.0/tools/clang/docs/UsersManual.html#cmdoption-fprofile-generate)

Run the wrapper script against, e.g. `object-2.c`:

```bash
./clang-obj-cover.sh object-2.c
```

In general, the script compiles the source file with optimization level `-O3`, runs the executable and displays the profile data.
The script does so twice for comparison:

1. For the first time, we instrument *at the front-end* with compiler flag `-fprofile-instr-generate`.
2. For the second time, we instrument *at the middle-end* with compiler flag `-fprofile-generate`.

The output would look like:

```text
### Front-end instrumentation ###

Counters:
  main:
    Hash: 0x000000000011b458
    Counters: 2
    Function count: 1
    Block counts: [10]
  checkAndSet:
    Hash: 0x00000046ca6d2451
    Counters: 3
    Function count: 1
    Block counts: [10, 5]
Instrumentation level: Front-end
Functions shown: 2
Total functions: 2
Maximum function count: 1
Maximum internal block count: 10

### Middle-end instrumentation ###

Counters:
  main:
    Hash: 0x00860b4d6c19fcfe
    Counters: 4
    Block counts: [10, 10, 1, 4]
  checkAndSet:
    Hash: 0x0f9bab18231589a7
    Counters: 3
    Block counts: [0, 0, 0]
Instrumentation level: IR  entry_first = 0
Functions shown: 2
Total functions: 2
Maximum function count: 10
Maximum internal block count: 10
```

Note the "Instrumentation level" field is `Front-end` and `IR` respectively. And for IR/middle-end instrumentation, the result is closer to the actual object-level execution: the counts for `checkAndSet()` are all zero.
