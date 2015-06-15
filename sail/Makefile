# Top level makefile

CC = g++
CFLAGS = -Wall
DEBUG = y

ifeq ($(DEBUG), y)
CFLAGS += -g
else
CFLAGS += -O2
endif

DIR_SRC = ./
DIR_OBJ = ./obj
DIR_BIN = ./bin
DIR_TEST_BIN = ./bin/test


.PHONY: test clean

test:
	cd base && $(MAKE)
	#cd net  && $(MAKE)
	cd test && $(MAKE)

clean:
	find ${DIR_OBJ} -name *.o -exec rm -rf {} \;
	rm -fr ${DIR_BIN}/*


