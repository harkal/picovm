

/*
    
*/

#include <stdio.h>
#include <string.h>

#include "../../src/picovm.h"

#define UNUSED __attribute__((unused))

void call_user(void *ctx);

#define VM_SIZE 1024

uint8_t vm_memory[VM_SIZE];
struct picovm_s vm = {
    0, &vm_memory[0] + VM_SIZE, 0,
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

void trace(struct picovm_s *vm, uint16_t size)
{
    int16_t start = vm->ip - 16;
    if(start < 0)start=0;
    uint16_t end = start + 32;

    for(uint16_t i = start ; i < end ; i++)
    {
        printf("%02x", vm_memory[i]);
    }
    for(uint16_t i = size-32; i < size ; i++)
    {
        printf("%02x", vm_memory[i]);
    }
    printf("\n");
    
    for(uint16_t i = start ; i < start+29 ; i++)
    {
        if (i == vm->ip) {
            printf("^^-- IP ");
            //i += 3;
        } else {
            printf("  ");
        }
    }

    for(uint16_t i = size - 32 ; i < size ; i++)
    {
        if (i == (vm->sp - (uint8_t *)vm->mem)) {
            printf("^^-- SP");
        } else {
            printf("  ");
        }
    }
    printf("\n");
    // printf("N:%d Z:%d\n", vm->flags&PICOVM_FLAG_N, vm->flags&PICOVM_FLAG_Z);

}

int main(int argc UNUSED, char **argv UNUSED)
{
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
        trace(&vm, 1024);
        if(i == 50)break;
    }

    printf("\n\nExecuted %d instructions", i);

    return 0;
}
