# SPDX-License-Identifier: MIT

set -e

source=$1
basename=$( basename $source .c )
executable=$basename
CFLAGS='-O3'

make clean > /dev/null
printf "\n### Front-end instrumentation ###\n\n"

# NOTE(wt): I don't see the point of -fprofile-generate=<dir>. There would only
#           be one .profraw file per .c file. Even repeated run would only
#           update the same .profraw file instead of creating a new one.
#           On the other hand at the same level we can reuse the `clean` recipe
#           in Makefile.
clang $CFLAGS -fprofile-instr-generate $source -o $executable

LLVM_PROFILE_FILE="$basename.profraw" ./$executable > /dev/null

llvm-profdata merge -output=$basename.profdata $basename.profraw
llvm-profdata show $basename.profdata                                          \
    --all-functions `# show details of each function`                          \
    --counts        `# show counter *values* in addition to num of counters`

make clean > /dev/null
printf "\n### Middle-end instrumentation ###\n\n"

clang $CFLAGS -fprofile-generate $source -o $executable

LLVM_PROFILE_FILE="$basename.profraw" ./$executable > /dev/null

llvm-profdata merge -output=$basename.profdata $basename.profraw
llvm-profdata show $basename.profdata                                          \
    --all-functions `# show details of each function`                          \
    --counts        `# show counter *values* in addition to num of counters`
