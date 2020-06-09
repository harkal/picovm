
#include <stdio.h>
#include <string.h>

#include "picovm.h"

#define UNUSED __attribute__((unused))

static uint8_t mem_read8(uint16_t addr, void *ctx)
{
    uint8_t *memory = ctx;
	return *(uint8_t *)(memory + addr);
}

static uint16_t mem_read16(uint16_t addr, void *ctx)
{
    uint8_t *memory = ctx;
	return *(uint16_t *)(memory + addr);
}

static uint32_t mem_read32(uint16_t addr, void *ctx)
{
    uint8_t *memory = ctx;
	return *(uint32_t *)(memory + addr);
}

static void mem_write8(uint16_t addr, uint8_t value, void *ctx)
{
    uint8_t *memory = ctx;
	*(uint8_t *)(memory + addr) = value;
}

static void mem_write16(uint16_t addr, uint16_t value, void *ctx)
{
    uint8_t *memory = ctx;
	*(uint16_t *)(memory + addr) = value;
}

static void mem_write32(uint16_t addr, uint32_t value, void *ctx)
{
    uint8_t *memory = ctx;
	*(uint32_t *)(memory + addr) = value;
}

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
        &vm_memory,
	    &mem_read8,
        &mem_read16,
        &mem_read32,
        &mem_write8,
        &mem_write16,
        &mem_write32
    };

    if (argc != 2) {
exit_with_helpmsg:
		fprintf(stderr, "Usage: %s [-v] {binfile} {hex-start-addr}\n", argv[0]);
		return 1;
	}

    int ch, addr;
    FILE *f = fopen(argv[1], "rb");
	if (!f)
		goto exit_with_helpmsg;

	for (int i = 0; (ch = fgetc(f)) != -1; i++)
		vm_memory[i] = ch;
	fclose(f);

    coredump(&vm, 64);

    for(int i = 0 ; i < 300 ; i++) {
        if(picovm_exec(&vm))
            break;
        coredump(&vm, 64);
    }

    return 0;
}
