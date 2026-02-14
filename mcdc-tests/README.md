<!--
SPDX-License-Identifier: MIT
-->

# Tests for Modified Condition/Decision Coverage (MC/DC)

## MC/DC Definition

MC/DC coverage = (Number of conditions evaluated to all possible outcomes affecting the outcome of the decision / Total number of conditions within the decisions) * 100

A condition is shown to independently affect a decision's outcome "by varying just that condition while holding fixed all other possible conditions", from [LLVM Design Doc](https://drive.google.com/file/d/1iHycLilNvGADj9oWe8i_Di6DVO4skVwJ/view?usp=sharing).

Condition is leaf-level Boolean expression (it cannot be broken down into simpler Boolean expressions).

Decision is a Boolean expression composed of conditions and zero or more **Boolean operators**.

We also explored the definition of MC/DC coverage in [VectorCAST](https://www.vectorcast.com/). Please refer to [Analysis.md](Analysis.md) for more details.

## Test Programs

The tests for MC/DC coverage:

- [boolean-operator.c](boolean-operator.c)
    - Definition of Boolean Operator
- [not.c](not.c)
    - `!` Operator
- [short-circuit.c](short-circuit.c)
    - Unevaluatable Condition
- [short-circuit-2.c](short-circuit-2.c)
    - Another test for unevaluatable Condition
- [scope.c](scope.c)
    - Scope of Decision
- [split-condition.c](split-condition.c)
    - Split Condition
    - Independent Affect
- [split-nested.c](split-nested.c)
    - Split-nested Condition

We also collect and modify the tests from LLVM tests for MC/DC coverage. The original tests can be found [here](https://reviews.llvm.org/differential/diff/544931/changesets/) (NOTE: They are not merged into LLVM yet).

- [mcdc-const.c](mcdc-const.c)
- [mcdc-const-folding.c](mcdc-const-folding.c)
- [mcdc-general.c](mcdc-general.c)

## Prerequisites

To run tests under this directory, we must use Clang with the MC/DC patch
(As for other software, there should be no *strict* version requirements).
The following instructions are tested with:

- Architecture: x86_64
- Distro: Ubuntu 22.04
- Kernel: 5.15.0-86-generic

Install dependencies:

```shell
sudo apt install build-essential
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
make --version
```

```text
GNU Make 4.3
Built for x86_64-pc-linux-gnu
Copyright (C) 1988-2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

<!--
  FIXME
  We'll use a fork instead of manually applying patches. And eventually we'll
  use the mainline LLVM. That README will need to be updated.
-->
Build Clang/LLVM with the MC/DC patch following instructions [here](../llvm-mcdc/README.md).

Add the LLVM build directory to `$PATH`:

```shell
export PATH="<llvm-source-root>/build/bin:$PATH"
```

and make sure we will invoke the correct version of `clang`:

```shell
which clang
```

should output:

```text
<llvm-source-root>/build/bin/clang
```

and

```shell
clang --version
```

should output:

```text
clang version 18.0.0 (https://github.com/llvm/llvm-project.git b5dffd4957dfb58c73e168a3d9b6967f03b23a6c)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: <llvm-source-root>/build/bin
```

## Build

```shell
make
```

## Collect Coverage Data

Execute the built program, e.g. `short-circuit`:

```shell
export MCDC_BASENAME="short-circuit"
LLVM_PROFILE_FILE="$MCDC_BASENAME.profraw" ./$MCDC_BASENAME
```

Once finished, a `short-circuit.profraw` file will be produced. Convert it into profile data file:

```shell
llvm-profdata merge $MCDC_BASENAME.profraw -o $MCDC_BASENAME.profdata
```

Read the profile data:

```shell
llvm-cov show --show-mcdc ./$MCDC_BASENAME -instr-profile=$MCDC_BASENAME.profdata
```

Example terminal output, where the source code is displayed along with MC/DC results:

```c
    1|       |#include <stdbool.h>
    2|       |#include <stdio.h>
    3|       |
    4|      2|void short_circuit(bool a, bool b) {
    5|      2|  if (a && b) {
  ------------------
  |---> MC/DC Decision Region (5:7) to (5:13)
  |
  |  Number of Conditions: 2
  |     Condition C1 --> (5:7)
  |     Condition C2 --> (5:12)
  |
  |  Executed MC/DC Test Vectors:
  |
  |     C1, C2    Result
  |  1 { F,  -  = F      }
  |  2 { T,  T  = T      }
  |
  |  C1-Pair: covered: (1,2)
  |  C2-Pair: not covered
  |  MC/DC Coverage for Decision: 50.00%
  |
  ------------------
    6|      1|    printf("short_circuit decision true\n");
    7|      1|  }
    8|      2|}
    9|       |
   10|      1|int main() {
   11|      1|  short_circuit(false, false);
   12|      1|  short_circuit(true, true);
   13|      1|}
```

## More Results

The MC/DC coverage results for all tests can be found in this directory, ending with extension name `.mcdc`.
For example, the result for `boolean-operator.c` is [boolean-operator.mcdc](boolean-operator.mcdc).

## Discussions

### Definition of Boolean Operator

```cpp
void biimplication(bool a, bool b) {
    if (a == b) { } // should == be a logical operator by the MC/DC definition?
}
```

```cpp
void bitwise_op(int a, int b) {
    if (a | b) { } // should bitwise operators be treated as logical by the MC/DC definition?
}
```

In LLVM, only `&&` and `||` are considered as logical operators. `==` is considered as a bitwise operator in its [implementation](https://clang.llvm.org/doxygen/Expr_8h_source.html#l03966).

```cpp
static bool isLogicalOp(Opcode Opc) { return Opc == BO_LAnd || Opc==BO_LOr; }
```

### Unevaluatable Condition

```cpp
struct s { int f; }
void short_circuit(struct s *p) {
    if (p != NULL && p->f > 0) { } // when first condition is false, second can't be even evaluated
}
```

From LLVM-Cov MC/DC Coverage Design Document:

> An addendum to the DO-178C standard definition clarifies, "or varying just that condition while holding fixed all other possible conditions that could affect the outcome", which allows us to ignore unevaluatable conditions for languages (like C/C++) that have short-circuit semantics on logical operations `&&` and `||`.

### Scope of Decision

Do we need to measure MC/DC for each decision/boolean expression?

```cpp
void bool_exp(bool a, bool b) {
    bool c = a && b; // should MC/DC be measured for each boolean expression?
    if (c) { }
}
```

LLVM-Cov measures MC/DC for all boolean expressions.

### Split Condition

How to deal with split condition?

- Decision

    ```cpp
    if ((y && z) || (u && v)) {}
    ```

- Split version

    ```cpp
    x = y && z;
    w = u && v;
    if (x || w) {}
    ```

Though we can trace all boolean expressions in the split version, this two decisions are not equivalent if we strictly follow the rule of holding all other conditions fixed.

Here is one example pair:

```
y = F, z = T, u = F, v = T --> x = F, w = F, x || w = F
y = T, z = T, u = T, v = F --> x = T, w = F, x || w = T
```

This pair obviously satisfy the split version. But for the original version, since we change `u` and `v`, that said, we don't hold all other conditions fixed, so it doesn't satisfy the original version if we follow the rule strictly.

But this is a short-circuit case, so we can ignore the second condition when the first condition is `True`. In this case, the original version is equivalent to the split version. This is how LLVM-Cov handles the split condition. For the original version, it will label `u` and `v` with `-`, which means we don't care about the condition. Thus, the pair could be interpreted as:

```
y = F, z = T, u = -, v = - --> x = F, w = -, x || w = F
y = T, z = T, u = -, v = - --> x = T, w = -, x || w = T
```

But for the following pair

```
y = F, z = T, u = T, v = T --> x = F, w = T, x || w = T
y = T, z = F, u = T, v = F --> x = F, w = F, x || w = F
```

It can satisfy the split version but not the original version. Plus, we cannot leverage the short-circuit semantics here.

### Split-nested Condition

How to deal with split-nested condition?

```cpp
x = (a && b && c && foo(d && f))
```

This comes from LLVM-Cov. It cannot handle the split-nested condition and raises a warning to users. The discussion for LLVM-Cov is [here](https://reviews.llvm.org/D138849?id=485147#inline-1455158).

### Independent Affect

```cpp
void keep_minor_same(bool a, bool b, bool c) {
    if ((a && b) || c) { } // does TTF and F?T satisfy MC/DC for "a"?
}
```
