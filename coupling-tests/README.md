<!--
SPDX-License-Identifier: MIT
-->

# Tests for Data/Control Coupling Coverage

These examples for data and control coupling are adapted from [this demo](https://www.vector.com/us/en/events/global-de-en/webinar-recordings/2022/vectorcast-automating-data-and-control-coupling/) by VectorCAST.

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

## Build and Run

To run the test, enter either subdirectory and

```shell
make
./myprogram
```

Expected results (TODO):

So far the tests under this directory are just example programs
that help us understand the definition of data and control coupling.
But there seems to be no open-source solutions to it.
