#!/bin/bash

# SPDX-License-Identifier: MIT

export PATH="$(realpath ../llvm-mcdc/llvm/build/bin):$PATH"
which clang
clang --version
cat << EOF

Make sure it's the same version of LLVM you used to compile the kernel.

EOF
read -n 1 -s -r -p "Press any key to continue..."
echo
echo

profraw_filename=$( ls -tr vmlinux_*.profraw | tail -n 1                 )
profile_basename=$( basename $profraw_filename .profraw                  )
profdata_filename=$profile_basename.profdata
suffix=$(           echo $profile_basename | tr -d '/' | cut -d '_' -f 2 )
version=$(          echo $profile_basename | tr -d '/' | cut -d '_' -f 3 )
boot_time=$(        echo $profile_basename | tr -d '/' | cut -d '_' -f 4 )

llvm-profdata merge $profraw_filename -o $profdata_filename

echo > llvm-cov_${suffix}_${version}_${boot_time}.log

llvm-cov show `#--num-threads=1`                                               \
              --show-mcdc                                                      \
              --show-mcdc-summary                                              \
              --show-region-summary=false                                      \
              --show-branch-summary=false                                      \
              --format=text                                                    \
              --use-color=false                                                \
              -output-dir=text_clang-scc-kunit_${version}_${boot_time}         \
              -instr-profile $profdata_filename                                \
              linux/vmlinux                                                  |&\
              tee -a llvm-cov_${suffix}_${version}_${boot_time}.log

llvm-cov show `#--num-threads=1`                                               \
               --show-mcdc                                                     \
              --show-mcdc-summary                                              \
              --show-region-summary=false                                      \
              --show-branch-summary=false                                      \
              --format=html                                                    \
              -output-dir=html_clang-scc-kunit_${version}_${boot_time}         \
              -instr-profile $profdata_filename                                \
              linux/vmlinux                                                  |&\
              tee -a llvm-cov_${suffix}_${version}_${boot_time}.log
