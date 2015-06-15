

SRC = $(wildcard *.c)
TARGET = $(patsubst %.c,${DIR_TEST_BIN}/%)

CC=g++
CFLAGS= -Wall -g -I /home/zhuoweibin/test/sail/

all: ../bin/test-dict


../bin/test-dict: ../obj/test-dict.o ../obj/dict.o
	$(CC) $(CFLAGS) -o $@ $^

../obj/test-dict.o: test-dict.cc ../base/dict.h
	$(CC) $(CFLAGS) -c $< -o $@


