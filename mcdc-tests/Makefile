# SPDX-License-Identifier: MIT

CC = clang
# CFLAGS = -Wall
COV_FLAGS = -fcoverage-mapping -fprofile-instr-generate -fcoverage-mcdc

TARGETS = boolean-operator mcdc-const mcdc-const-folding mcdc-general not \
          scope short-circuit-2 short-circuit split-condition split-nested

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $(COV_FLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
	rm -f *.gcda *.gcno *.gcov *.profraw *.profdata

.PHONY: clean all
