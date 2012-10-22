
.SUFFIXES: .cc

CXX = clang++

WARNINGS = -Wall -Wswitch -Wno-virtual-dtor
USE_CXX_11 = -std=c++11 -stdlib=libc++ -isystem /usr/lib/c++/v1 -isystem /usr/include/c++/4.2.1 -isystem /usr/include/c++/4.2.1/ext
CXXFLAGS = -Isrc -g ${USE_CXX_11} ${WARNINGS} ${OPTFLAGS}

SOURCES  = ${wildcard src/*.cc src/**/*.cc}
PROG_SRC = ${wildcard bin/*.cc}
TEST_SRC = ${wildcard test/*.cc}

OBJECTS = ${SOURCES:.cc=.o}

PROGRAMS = ${PROG_SRC:%.cc=%}

TESTS = ${TEST_SRC:%.cc=%}

TARGET = build/libcalc.a

rebuild: clean
rebuild: all

all: ${TARGET} ${PROGRAMS} run

run:
	./bin/main

${PROGRAMS}: CXXFLAGS += ${TARGET}
#	@echo BUILD $@
#	@${CXX} ${CXXFLAGS} ${TARGET} -o $@ $


${TARGET}: CXXFLAGS += -fPIC
${TARGET}: build ${OBJECTS}
	@echo AR $@
	@ar rcs $@ ${OBJECTS}
	@ranlib $@

build:
	@mkdir -p build

.cc.o: %.cc
	@echo CC $<
	@${CXX} -c ${CXXFLAGS} -o $@ $<

clean:
	@echo cleaning
	@rm -rf build ${OBJECTS} ${TESTS} ${PROGRAMS} *.o *.d *~
	@find . -name "*.gc*" -exec rm {} \;
	@rm -rf `find . -name "*.dSYM" -print`


.PHONY: clean all rebuild run build
