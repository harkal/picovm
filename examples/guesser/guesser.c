

/*
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../../src/picovm.h"

#define UNUSED __attribute__((unused))

void call_user(void *ctx);

#define VM_SIZE 1024

uint8_t vm_memory[VM_SIZE];
struct picovm_s vm = {
	0, VM_SIZE, &vm_memory[0] + VM_SIZE, 0,
	&vm_memory,
	NULL,
	call_user
};

void call_user(void *ctx UNUSED) 
{
	uint8_t op = *(char *)vm.sp;
	switch (op)
	{
	case 0:
		putc(*(char *)(vm.sp + 1), stdout);
		break;
	case 1:
		*vm.sp = getc(stdin);
		break;
	case 2:
		*vm.sp = rand() & 0xff;
		break;
	default:
		break;
	}
}

int main(int argc UNUSED, char **argv UNUSED)
{
	time_t t;
	srand((unsigned) time(&t));
	memset(vm_memory, 0, VM_SIZE);

	int ch;
	FILE *f = fopen("guesser.hex", "rb");
	if (!f)
		return 1;

	for (int i = 0; (ch = fgetc(f)) != -1; i++)
		vm_memory[i] = ch;
	fclose(f);

	int i;
	for(i = 0 ; ; i++) {
		if(picovm_exec(&vm))
			break;
	}

	printf("\n\nExecuted %d instructions", i);

	return 0;
}
