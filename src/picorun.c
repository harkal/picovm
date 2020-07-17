
/*
	This is a small testing program to inspect the execution of 
	a picovm program. 
*/

#include <stdio.h>
#include <string.h>

#include "picovm.h"
#include "picodisasm.h"

#define UNUSED __attribute__((unused))

// #define DO_TRACE true

const uint16_t mem_size = 0xffff;

uint8_t vm_memory[mem_size];

void dump_stack(struct picovm_s *vm, uint16_t size)
{	
	printf("\n  IP: 0x%04x\n", vm->ip);
	printf(" SFP: 0x%04x\n", vm->sfp);
	printf("  SP: 0x%04lx\n", (void *)vm->sp - vm->mem);
	printf("   N:%d Z:%d\n", vm->flags&PICOVM_FLAG_N, vm->flags&PICOVM_FLAG_Z);

	printf("Stack memory:\n");
	for(unsigned int i = size - 64 ; i < size ; i++)
	{
		if (i % 4 == 3)printf(" ");
		printf("%02x", vm_memory[i]);
	}
	printf("\n");

	for(unsigned int i = size - 64 ; i <= size ; i++)
	{
		if (i % 4 == 3)printf(" ");
		if (i == (vm->sp - (uint8_t *)vm->mem)) {
			printf("^^-- SP");
		} else {
			printf("  ");
		}
	}
	printf("\n");
	for(unsigned int i = size - 64 ; i <= size ; i++)
	{
		if (i % 4 == 3)printf(" ");
		if (i == vm->sfp) {
			printf("^^-- SFP");
		} else {
			printf("  ");
		}
	}
	printf("\n");
}

int trace_op(struct picovm_s *vm, uint16_t ip, char *str) 
{
	char mnemonic[16];
	mnemonic[0] = 0;
	int size = disassemble(vm->mem, ip, mnemonic);
	sprintf(str, "0x%04x:\t%s [", ip, mnemonic);
	int i = 0;
	if (size <=0) size = 1;
	while(i < size) {
		sprintf(str, "%s%02x", str, *(uint8_t*)(vm->mem + ip + i));
		++i;
	}
	sprintf(str, "%s]", str);
	return size;
}

void trace(struct picovm_s *vm)
{
	char disasm[64];
	disasm[0] = 0;

	printf("\n");

	int s = trace_op(vm, vm->ip, disasm);
	printf("\x1B[93m-> %s\033[0m\n", disasm);
	if (s > 0) {
		trace_op(vm, vm->ip + s, disasm);
		printf("   %s\n", disasm);
	}
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

	int i;
	for(i = 0 ; 1 ; i++) {
#ifdef DO_TRACE
		trace(&vm);
#endif
		if(picovm_exec(&vm))
			break;
#ifdef DO_TRACE
		dump_stack(&vm, mem_size);
		getchar();
#endif
	}

	printf("\nExecuted %d instructions", i);

	return 0;
}
