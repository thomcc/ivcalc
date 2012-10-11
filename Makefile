SOURCES = ${wildcard src/*.cc src/**/*.cc}
OBJECTS = ${SOURCES:.cc=.o}
CC = clang++
WARNINGS = -Wall -Wswitch -Wno-virtual-dtor
CFLAGS = -std=c++11 -g ${WARNINGS} ${OPTFLAGS}
PROGRAM = test

all: ${PROGRAM}

rebuild: clean
rebuild: all

${PROGRAM}: ${OBJECTS}
  ${CC} ${CFLAGS} -o $@ ${OBJECTS}
  ./${PROGRAM}
.cc.o:
  ${CC} ${CFLAGS} -c $<

clean:
  rm -f ${PROGRAM} ${OBJECTS} *.o *.d *~
  find . -name "*.gc*" -exec rm {} \;
  rm -rf `find . -name "*.dSYM" -print`

.PHONY: clean all rebuild
