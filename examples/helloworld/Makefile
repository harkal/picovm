CC = gcc
CFLAGS += -Wall -Wextra -Os

run: all
	./picohello

all: picohello helloworld.hex

picohello: picohello.o picovm.o
	$(CC) picohello.o picovm.o -o picohello

picovm.o: ../../src/picovm.c
	$(CC) $(CFLAGS) -c -o picovm.o ../../src/picovm.c

helloworld.hex: helloworld.asm
	node ../../picoasm/picoasm.js helloworld.asm helloworld.hex

.PHONY: clean
clean:
	rm -f picohello *.d *.o core

