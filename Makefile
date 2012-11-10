
.SUFFIXES: .cc

CXX = clang++

WARNINGS = -Wall -Wswitch -Wno-virtual-dtor -Woverloaded-virtual
USE_CXX_11 = -std=c++11 -stdlib=libc++ -isystem /usr/lib/c++/v1 -isystem /usr/include/c++/4.2.1 -isystem /usr/include/c++/4.2.1/ext
CXXFLAGS = -Isrc -g ${USE_CXX_11} ${WARNINGS} ${OPTFLAGS}
LIBS = `llvm-config --cppflags --ldflags --libs core`
PRODFLAGS = -O3 -DNDEBUG

SOURCES = ${wildcard src/*.cc src/**/*.cc}
PROG_SRC = ${wildcard bin/*.cc}
TEST_SRC = ${wildcard test/*.cc}

OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
PROGRAMS = ${PROG_SRC:%.cc=%}
PROGDEPENDS = ${PROG_SRC:.cc=.d}

TESTS = ${TEST_SRC:.cc=.o}
TESTDEPENDS = ${TESTS:.o=.d}
TESTMAIN = test/test_main

TARGET = build/libcalc.a

all: ${TARGET} tests ${PROGRAMS} run_tests

prod: CXXFLAGS += ${PRODFLAGS}
prod: clean
prod: all

rebuild: clean
rebuild: all

tests: ${TESTS}
	@echo LINK ${TESTMAIN}
	@${CXX} ${CXXFLAGS} ${LIBS} ${TARGET} ${TESTS} -o ${TESTMAIN}

run_tests:
	@echo running tests...
	@./${TESTMAIN}

${PROGRAMS}: CXXFLAGS += ${TARGET}
${PROGRAMS}: ${addsuffix .cc, $@}
	@echo CC ${addsuffix .cc, $@}
	@${CXX} ${LIBS} ${CXXFLAGS} -g -o $@ ${addsuffix .cc, $@} -MD

${TARGET}: CXXFLAGS += -fPIC
${TARGET}: build ${OBJECTS}
	@echo AR $@
	@ar rcs $@ ${OBJECTS}
	@ranlib $@

build:
	@mkdir -p build

.cc.o: %.cc
	@echo CC $<
	@${CXX} -c ${LIBS} ${CXXFLAGS} -o $@ $< -MD

clean:
	@echo cleaning
	@rm -rf build ${OBJECTS} ${DEPENDS} ${PROGDEPENDS} ${TESTDEPENDS} ${TESTS} ${PROGRAMS} ${TESTMAIN} *.o *.d *~
	@find . -name "*.gc*" -exec rm {} \;
	@rm -rf `find . -name "*.dSYM" -print`


.PHONY: clean all rebuild run build tests run_tests


sinclude ${DEPENDS} ${TESTDEPENDS} ${PROGDEPENDS}
