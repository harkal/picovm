CC = gcc
CFLAGS += -Wall -Wextra -Os

all: picohello

picovm.o: ../../src/picovm.c
	$(CC) $(CFLAGS) -c -o picovm.o ../../src/picovm.c

picohello: picohello.o picovm.o
	$(CC) picohello.o picovm.o -o picohello

.PHONY: clean
clean:
	rm -f picohello *.d *.o core

