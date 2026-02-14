# SPDX-License-Identifier: MIT

set -e

name="${1%.*}"
clang $1 -fcoverage-mcdc -fcoverage-mapping -fprofile-instr-generate -o $name

LLVM_PROFILE_FILE="$name.profraw" ./$name
llvm-profdata merge -sparse $name.profraw -o $name.profdata
llvm-cov show --show-branches=count --show-mcdc $name -instr-profile $name.profdata $name.c | tee $name.mcdc

# Clean the intermediate files
rm $name $name.profraw $name.profdata
