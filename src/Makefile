CC = llvm-gcc
# CC = arm-none-eabi-gcc
# CFLAGS +=  -Wall -Wextra -Og -glldb
CFLAGS += -MD -Wall -Wextra -Os

all: picorun

picorun: picorun.o picovm.o

install: picovm
	install -TD evmdemo /usr/local/bin/evmdemo
	install -TD -m0644 picovm.c /usr/local/share/picovm/picovm.c
	install -TD -m0644 picovm.h /usr/local/share/picovm/picovm.h

clean:
	rm -f picovm *.d *.o core

-include *.d
