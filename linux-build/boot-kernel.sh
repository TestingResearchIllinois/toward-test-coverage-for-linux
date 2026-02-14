#!/bin/bash

# SPDX-License-Identifier: MIT

linux_build_dir=$(realpath .)
cd linux

boot_time=$(date +%s)
version=$(cat .version)
suffix=$(cat $linux_build_dir/.suffix)

cmd1="cp /sys/kernel/debug/pgo/profraw ./vmlinux_${suffix}_${version}_${boot_time}.profraw"
cmd2="dmesg > dmesg_${suffix}_${version}_${boot_time}.log"

$linux_build_dir/q -s "$cmd1; $cmd2"

mv vmlinux_${suffix}_${version}_${boot_time}.profraw dmesg_${suffix}_${version}_${boot_time}.log $linux_build_dir
