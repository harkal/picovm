

/*
    
*/

#include <stdio.h>
#include <string.h>

#include "../../src/picovm.h"

#define UNUSED __attribute__((unused))

void call_user(void *ctx);

uint8_t vm_memory[64];
struct picovm_s vm = {
    0, &vm_memory[0] + 64, 0,
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
    memset(vm_memory, 0, 64);

    int ch;
    FILE *f = fopen("helloworld.hex", "rb");
	if (!f)
		return 1;

	for (int i = 0; (ch = fgetc(f)) != -1; i++)
		vm_memory[i] = ch;
	fclose(f);

    int i;
    for(i = 0 ; i < 50000 ; i++) {
        if(picovm_exec(&vm))
            break;
    }

    printf("\n\nExecuted %d instructions", i);

    return 0;
}
