<!--
SPDX-License-Identifier: MIT
-->

# Install nightly LLVM packages on a Debian/Ubuntu

LLVM maintains nightly packages so that we can try very recent commits without
having to build it from source.

As of March 2024, the "development branch"
([`main`](https://github.com/llvm/llvm-project/tree/main))
is 19 and the "qualification branch"
([the largest `release/N.x`](https://github.com/llvm/llvm-project/tree/release/18.x))
is 18. Most of MC/DC features and fixes (theirs and ours) are already available
on these two branches. Visit https://apt.llvm.org/ for more information.

Get the installation script:

```shell
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
```

Install LLVM 18:

```shell
sudo ./llvm.sh 18
```

Install LLVM 19:

```shell
sudo ./llvm.sh 19
```

Now we can invoke executables by names like `clang-19`, `clang++-18`,
`llvm-profdata-19`, `llvm-cov-18` etc

To avoid typing the suffix each time, install these meta packages which will
point executables like `clang`, `clang++`, `llvm-profdata`, `llvm-cov` etc to
the current "development branch" (that is, 19 as of March 2024).

```shell
sudo apt install clang lld llvm
```
