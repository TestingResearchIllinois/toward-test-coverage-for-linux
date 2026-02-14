#!/bin/bash

# SPDX-License-Identifier: MIT

export PATH="$(realpath ../llvm-mcdc/llvm/build/bin):$PATH"
which clang
clang --version
echo
read -n 1 -s -r -p "Press any key to continue..."
echo
echo

linux_build_dir=$(realpath .)
build_time=$(date +%s)
suffix="llvm-19-mcdc-kunit"
cp .config linux
cd linux

make LLVM=1 olddefconfig
make LLVM=1 LOCALVERSION=-${suffix} -j$(nproc) \
    |& tee ${linux_build_dir}/build-linux_${suffix}_${build_time}.log
version=$(cat .version)
mv ${linux_build_dir}/build-linux_${suffix}_${build_time}.log ${linux_build_dir}/build-linux_${suffix}_${version}_${build_time}.log
echo $suffix > ${linux_build_dir}/.suffix
