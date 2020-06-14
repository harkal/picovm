

all: examples pico

pico:
	make -C ./src

examples:
	make -C ./examples/helloworld picohello
	make -C ./examples/guesser guesser
clean:
	make -C ./src clean
	make -C ./examples/helloworld clean
	make -C ./examples/guesser clean

.PHONY: clean examples