

/*
    
*/

#include <stdio.h>
#include <string.h>

#include "../../src/picovm.h"

#define UNUSED __attribute__((unused))

uint8_t vm_memory[64];

void call_user(void *ctx UNUSED) 
{

}

int main(int argc UNUSED, char **argv UNUSED)
{
    memset(vm_memory, 0, 64);

    struct picovm_s vm = {
	    0, &vm_memory[0] + 64, 0,
        &vm_memory,
        NULL,
        call_user
    };

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
    for(i = 0 ; i < 50 ; i++) {
        if(picovm_exec(&vm))
            break;
    }

    printf("Executed %d instructions", i);

    return 0;
}
