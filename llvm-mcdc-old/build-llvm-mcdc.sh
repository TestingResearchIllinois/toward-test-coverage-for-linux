# SPDX-License-Identifier: MIT

set -e

PATCH_PATH=$(pwd)/mcdc-20231006.patch
LLVM_PATH=$(pwd)/llvm-project
git clone https://github.com/llvm/llvm-project $LLVM_PATH

cd $LLVM_PATH
git checkout b5dffd4957dfb58c73e168a3d9b6967f03b23a6c
git apply $PATCH_PATH # the patch was obtained from Alan Phipps, the developer of MC/DC for LLVM-Cov

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -GNinja -DLLVM_ENABLE_PROJECTS='clang' -DLLVM_TARGETS_TO_BUILD='X86' \
      -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind;compiler-rt" \
      -DLLVM_USE_LINKER=gold -DLLVM_PARALLEL_LINK_JOBS=20  -DLLVM_USE_SPLIT_DWARF=ON  -DCMAKE_EXE_LINKER_FLAGS_DEBUG='-Wl,--gdb-index' \
      -DSPHINX_OUTPUT_HTML=OFF -DSPHINX_OUTPUT_MAN=OFF -DCMAKE_CXX_FLAGS='-ggnu-pubnames' -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_BUILD_TESTS=ON ../llvm
ninja -j 20
