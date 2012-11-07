SRC = ir_test.c

all: ir_test.llvm ir_test.s ir_test.cpp

%.llvm: %.c
	clang -cc1 -emit-llvm -O3 -o $@ $<

%.s: %.c
	clang -fomit-frame-pointer -O3 -S -o $@ $<

%.cpp: %.llvm
	llc -march=cpp -O3 -o $@ $<


clean:
	rm ir_test.llvm
	rm ir_test.cpp
	rm ir_test.s

.PHONY: clean all rebuild runall opt noopt
