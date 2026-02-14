# SPDX-License-Identifier: MIT

# Variables
LLVM_INCLUDE := $(shell llvm-config --includedir)
LLVM_FLAGS := $(shell llvm-config --cxxflags --ldflags --libs core)
CLANG_LIBS := -lclangTooling -lclangFrontend -lclangDriver -lclangSerialization \
             -lclangParse -lclangSema -lclangAnalysis -lclangAST -lclangBasic \
             -lclangEdit -lclangLex
CXXFLAGS := --std=c++17
LOG_FILE := analyze-compile.log

# Default target
all: analyze

analyze: analyze.cpp
	@clang++ $< -o $@ -I$(LLVM_INCLUDE) $(LLVM_FLAGS) $(CLANG_LIBS) $(CXXFLAGS) 2>&1 | tee $(LOG_FILE)

clean:
	@rm -f analyze $(LOG_FILE)

.PHONY: all clean
