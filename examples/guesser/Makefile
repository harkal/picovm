CC = gcc
CFLAGS += -Wall -Wextra -Os

run: all
	./guesser

all: guesser guesser.hex

guesser: guesser.o picovm.o
	$(CC) guesser.o picovm.o -o guesser

picovm.o: ../../src/picovm.c
	$(CC) $(CFLAGS) -c -o picovm.o ../../src/picovm.c

guesser.hex: guesser.asm
	node ../../picoasm/picoasm.js guesser.asm guesser.hex

.PHONY: clean
clean:
	rm -f guesser *.d *.o core

