
#include <stdio.h>
#include <string.h>

#include "picovm.h"

#define UNUSED __attribute__((unused))

uint8_t vm_memory[64];

void coredump(struct picovm_s *vm, uint16_t size)
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
        } else if (i == vm->sp) {
            printf("^^-- SP");
        } else {
            printf("  ");
        }
    }

    printf("\n");

}

int main(int argc UNUSED, char **argv UNUSED)
{
    memset(vm_memory, 0, 64);

    struct picovm_s vm = {
	    0, 64,
        &vm_memory
    };

    if (argc != 2) {
exit_with_helpmsg:
		fprintf(stderr, "Usage: %s [-v] {binfile} {hex-start-addr}\n", argv[0]);
		return 1;
	}

    int ch;
    FILE *f = fopen(argv[1], "rb");
	if (!f)
		goto exit_with_helpmsg;

	for (int i = 0; (ch = fgetc(f)) != -1; i++)
		vm_memory[i] = ch;
	fclose(f);

    coredump(&vm, 64);

    for(int i = 0 ; i < 150 ; i++) {
        if(picovm_exec(&vm))
            break;
        coredump(&vm, 64);
    }

    return 0;
}
