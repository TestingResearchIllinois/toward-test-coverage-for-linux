<!--
SPDX-License-Identifier: MIT
-->

# Measure Linux Kernel's MC/DC

## 0. Prerequisites

- The following instructions are tested with:
    - Architecture: x86_64
    - Distro: Ubuntu 22.04
    - Kernel: 5.15.0-86-generic

    Other settings can possibly work, but that's not guaranteed.

- Please reserve at least 150G of disk space (not including dependencies installed via the distro's package manager) before proceeding

## 1. Install dependencies

```shell
# For building LLVM
sudo apt-get install cmake ninja-build mold
# For building the kernel
sudo apt-get install git bc libncurses-dev wget busybox libssl-dev libelf-dev dwarves flex bison build-essential
# For booting the kernel
sudo apt-get install qemu qemu-system-x86
```

Set up KVM

```shell
sudo chmod 666 /dev/kvm
```

## 2. Pull the source code and apply patches

```shell
cd path/to/your/workdir
export MCDC_HOME=$(realpath .)

git clone git@github.com:TestingResearchIllinois/enhanced-gcov.git
git clone https://github.com/llvm/llvm-project/ --branch main
git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git --branch v5.15.127 --depth 5

ln -s $MCDC_HOME/linux        $MCDC_HOME/enhanced-gcov/linux-build/linux
ln -s $MCDC_HOME/llvm-project $MCDC_HOME/enhanced-gcov/llvm-mcdc/llvm

cd $MCDC_HOME/enhanced-gcov/linux-build/linux
git apply $MCDC_HOME/enhanced-gcov/linux-build/kernel-atop-v5.15.127.patch

cd $MCDC_HOME/enhanced-gcov/llvm-mcdc/llvm
git checkout 16d0592dda89495afdbefa5c57eb006559a59465
git apply $MCDC_HOME/enhanced-gcov/llvm-mcdc/llvm-atop-16d0592dda89495afdbefa5c57eb006559a59465.patch
```

## 3. Get LLVM

You can either

* [Build LLVM from source](#build-from-source), or
* If you are on a Debian/Ubuntu machine and don't plan to change LLVM source code, [install nightly packages](./llvm-apt.md)

### Build from source

```shell
cd $MCDC_HOME/enhanced-gcov/llvm-mcdc
./build-llvm.sh
```

## 4. Build the kernel

```shell
cd $MCDC_HOME/enhanced-gcov/linux-build
./build-kernel.sh
```

> [!NOTE]
>
> At this stage you will see many warnings and the process will slow down near
> the end of building (mostly linking)
>
> This is expected. The warnings are due to
> [two major design flaws](https://releases.llvm.org/18.1.0/tools/clang/docs/SourceBasedCodeCoverage.html#mc-dc-instrumentation)
> of the current MC/DC implementation in Clang.
> Extra overhead is brought by code instrumentation (counters, bitmaps, MOV and
> ADD instructions to increment the counters), and
> [coverage mapping](https://releases.llvm.org/18.1.0/docs/CoverageMappingFormat.html)
> in order to associate such information with the actual source code locations.
> Together they lead to larger binary size and longer linking time.

## 5. Boot the kernel and collect coverage

```shell
cd $MCDC_HOME/enhanced-gcov/linux-build
./boot-kernel.sh
./llvm-cov-wrapper.sh
```

The results would be put under
`$MCDC_HOME/enhanced-gcov/linux-build/text_clang-scc-kunit_X_YYYYYYYYYY/` and
`$MCDC_HOME/enhanced-gcov/linux-build/html_clang-scc-kunit_X_YYYYYYYYYY/`.
