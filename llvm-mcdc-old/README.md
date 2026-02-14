<!--
SPDX-License-Identifier: MIT
-->

# LLVM MC/DC Cov

## Build LLVM with MC/DC support

You can following the instructions below:

```bash
LLVM_PATH=$(pwd)/llvm-project
git clone https://github.com/llvm/llvm-project $LLVM_PATH

cd $LLVM_PATH
git checkout b5dffd4957dfb58c73e168a3d9b6967f03b23a6c
git apply /path/to/mcdc-20231006.patch # the patch was obtained from Alan Phipps, the developer of MC/DC for LLVM-Cov

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -GNinja -DLLVM_ENABLE_PROJECTS='clang' -DLLVM_TARGETS_TO_BUILD='X86' \
      -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind;compiler-rt" \
      -DLLVM_USE_LINKER=gold -DLLVM_PARALLEL_LINK_JOBS=20  -DLLVM_USE_SPLIT_DWARF=ON  -DCMAKE_EXE_LINKER_FLAGS_DEBUG='-Wl,--gdb-index' \
      -DSPHINX_OUTPUT_HTML=OFF -DSPHINX_OUTPUT_MAN=OFF -DCMAKE_CXX_FLAGS='-ggnu-pubnames' -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_BUILD_TESTS=ON ../llvm
ninja -j 20
```

Or run `bash build-llvm-mcdc.sh` in this directory, which will build LLVM with MC/DC support and install it to `llvm-project/build/bin` under this directory.

## Run LLVM MC/DC Cov

```bash
export PATH=$LLVM_PATH/build/bin:$PATH
bash llvm-mcdc.sh path/to/source-file.c
```

The MC/DC coverage result will be printed to stdout and saved to `path/to/source-file.mcdc`.
