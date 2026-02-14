# SPDX-License-Identifier: MIT

CC = gcc
COV = gcov
# CFLAGS = -Wall

ifeq ($(COV),gcov)
	COV_FLAGS = -fprofile-arcs -ftest-coverage
else ifeq ($(COV),scc)
	COV_FLAGS = -fcoverage-mapping -fprofile-instr-generate
endif

# NOTE(wt)
#
# 1. Why gcov-4b wasn't here...?
# 2. As for the original version in GCC, gcovpart-13b.c doesn't have a main(),
#    thus skipped here.

TARGETS = gcov-10b gcov-10 gcov-11 gcov-12 gcov-13 gcov-14 gcov-15 gcov-16     \
          gcov-17 gcov-18 gcov-1a gcov-1 gcov-2 gcov-3 gcov-4b gcov-4 gcov-5b  \
          gcov-6 gcov-7 gcov-8 gcov-9 gcov-mytest gcov-pr83813                 \
          gcov-pr84758 gcov-pr85217 gcov-pr85332 gcov-pr85338 gcov-pr85350     \
          gcov-pr85372 gcov-pr86536 gcov-pr90574-1 gcov-pr90574-2 gcov-pr94029

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $(COV_FLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
	rm -f *.gcda *.gcno *.gcov *.profraw *.profdata

.PHONY: all clean

# Force rebuild because we frequently change compiler flags
.PHONY: $(TARGETS)
