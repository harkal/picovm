
/*
	This is a small testing program to inspect the execution of 
	a picovm program. 
*/

#include <stdio.h>
#include <string.h>

#include "picovm.h"

#define UNUSED __attribute__((unused))

const uint16_t mem_size = 64;

uint8_t vm_memory[mem_size];

void trace(struct picovm_s *vm, uint16_t size)
{
	for(uint16_t i = 0 ; i < size ; i++)
	{
		printf("%02x", vm_memory[i]);
	}
	printf("\n");

	for(uint16_t i = 0 ; i < size ; i++)
	{
		if (i == vm->ip) {
			printf("^^-- IP ");
			i += 3;
		} else if (i == (vm->sp - (uint8_t *)vm->mem)) {
			printf("^^-- SP");
		} else {
			printf("  ");
		}
	}

	printf("\n N:%d Z:%d\n", vm->flags&PICOVM_FLAG_N, vm->flags&PICOVM_FLAG_Z);

}

void call_user(void *ctx UNUSED);

struct picovm_s vm = {
	0, mem_size, &vm_memory[0] + mem_size, 0,
	&vm_memory,
	NULL,
	call_user
};

void call_user(void *ctx UNUSED) 
{
	putc(*(char *)vm.sp, stdout);
}


int main(int argc UNUSED, char **argv UNUSED)
{
	memset(vm_memory, 0, mem_size);

	if (argc != 2) {
exit_with_helpmsg:
		fprintf(stderr, "Usage: %s [-v] binfile\n", argv[0]);
		return 1;
	}

	int ch;
	FILE *f = fopen(argv[1], "rb");
	if (!f)
		goto exit_with_helpmsg;

	for (int i = 0; (ch = fgetc(f)) != -1; i++)
		vm_memory[i] = ch;
	fclose(f);

	trace(&vm, mem_size);
	int i;
	for(i = 0 ; i < 500 ; i++) {
		if(picovm_exec(&vm))
			break;
		trace(&vm, mem_size);
	}

	printf("\nExecuted %d instructions", i);

	return 0;
}
