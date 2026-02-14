# SPDX-License-Identifier: MIT

CC = gcc
COV = gcov
CFLAGS = -Wall
OPTIM = -O0

ifeq ($(COV),gcov)
	COV_FLAGS = -fprofile-arcs -ftest-coverage
else ifeq ($(COV),scc)
	COV_FLAGS = -fcoverage-mapping -fprofile-instr-generate
endif

# TODO: PGOCOV and -fprofile-generate are not documented

all: object-1 object-2 object-3 object-4 object-5

object-1: object-1.c
	$(CC) $(CFLAGS) $(OPTIM) $(COV_FLAGS) -o $@ $<

object-2: object-2.c
	$(CC) $(CFLAGS) $(OPTIM) $(COV_FLAGS) -o $@ $<

object-3: object-3.c
	$(CC) $(CFLAGS) $(OPTIM) $(COV_FLAGS) -o $@ $<

object-4: object-4.c
	$(CC) $(CFLAGS) $(OPTIM) $(COV_FLAGS) -o $@ $<

object-5: object-5.c
	$(CC) $(CFLAGS) $(OPTIM) $(COV_FLAGS) -o $@ $<

clean:
	rm -f *.gcda *.gcno *.gcov *.profraw *.profdata \
	object-1 object-2 object-3 object-4 object-5

.PHONY: clean all

# Force rebuild because we frequently change compiler flags
.PHONY: object-1 object-2 object-3 object-4 object-5
