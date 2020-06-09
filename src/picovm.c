#include <memory.h>
#include "picovm.h"

// #include <stddef.h>
// void *memcpy(void *__restrict, const void *__restrict, size_t);

#define READ8(x)  (*(uint8_t *)(vm->mem + (x)))
#define READ16(x) (*(uint16_t *)(vm->mem + (x)))
#define READ32(x) (*(uint32_t *)(vm->mem + (x)))

static void push_stack(struct picovm_s *vm, void *value, uint8_t size)
{
    vm->sp -= size;
	memcpy(vm->mem + vm->sp, value, size);
}

static void pop_stack(struct picovm_s *vm, uint8_t size, void *value)
{
	memcpy(value, vm->mem + vm->sp, size);
	vm->sp += size;
}

int8_t picovm_exec(struct picovm_s *vm)
{
    uint8_t opcode = READ8(vm->ip);
    uint32_t a, b, addr;

	struct { float b, a; } fops;

    switch (opcode)
	{
        case 0x00 ... 0x00 + 3: // LOAD addr
        {
            uint16_t addr = READ16(vm->ip + 1);
			uint8_t size = 1 << (opcode & 0x02);
			vm->sp -= size;
			memcpy(vm->mem + vm->sp, vm->mem + addr, size);
            vm->ip += 3;
            break;
        }
		
        case 0x08 ... 0x08 + 3: // STORE addr
        {
			uint16_t addr = READ16(vm->ip + 1);
			uint8_t size = 1 << (opcode & 0x02);
			memcpy(vm->mem + addr, vm->mem + vm->sp, size);
			vm->sp += size;
            vm->ip += 3;
            break;
        }

		case 0x10 ... 0x10 + 3: // POP
        {
			uint8_t size = 1 << (opcode & 0x02);
			vm->sp += size;
            vm->ip += 1;
            break;
        }

		case 0x14 ... 0x14 + 9: // DUP
        {
			uint8_t k = (opcode & 0xc) >> 2;
			uint8_t size = 1 << (opcode & 0x02);

			vm->sp -= size;
			memcpy(vm->mem + vm->sp, vm->mem + vm->sp + size, k * size);
			memcpy(vm->mem + vm->sp + k * size, vm->mem + vm->sp, size);

            vm->ip += 1;
            break;
        }

		case 0x80+0 ... 0x80+43:
		{
			uint8_t cmd = (opcode & 0x3c) >> 2;
			uint8_t size = 1 << (opcode & 0x02);
			if (cmd != 7) {
				pop_stack(vm, size, &b);
			}
			pop_stack(vm, size, &a);
			switch (cmd)
			{
				case 0: a += b; break;
				case 1: a -= b; break;
				case 2: a *= b; break;
				case 3: a /= b; break;
				case 4: a %= b; break;

				case 5: a &= b; break;
				case 6: a |= b; break;
				case 7: a ^= b; break;
				case 8: a = !a; break;
			}
			uint8_t sign_bit = size * 8 - 1;
			uint32_t mask = (1 << size*8) - 1;
			
			vm->flags = (a & mask) == 0 ? PICOVM_FLAG_Z : 0;
			if (a & (1 << sign_bit)) vm->flags |= PICOVM_FLAG_N;

			push_stack(vm, &a, size);
			vm->ip++;
			break;
		}
		case 0xac ... 0xac + 4:
		{
			uint8_t cmd = (opcode & 0x3c) >> 2;
			
			memcpy(&fops, vm->mem + vm->sp, sizeof(float) * 2);
			vm->sp += sizeof(float) * 2;
			switch (cmd)
			{
				case 11: fops.a += fops.b; break;
				case 12: fops.a -= fops.b; break;
				case 13: fops.a *= fops.b; break;
				case 14: if (fops.b != 0.0f)fops.a /= fops.b; else fops.a = 0.0f; break;
			}

			push_stack(vm, &fops.a, sizeof(float));
			vm->ip++;
			break;
		}
		// JMP addr
		case 0xc0 ... 0xc0+16:
		{
			// Get jump address
			if ((opcode & 1) == 0) {
				addr = vm->ip + (int8_t)READ8(vm->ip+1);
				vm->ip += 2;
			} else {
				addr = vm->ip + (int16_t)READ16(vm->ip+1);
				vm->ip += 3;
			}

			uint16_t jump_type = (opcode & 0xE) >> 1;
			if (jump_type == 0) {
				vm->ip = addr;
				break;
			}

			uint8_t flags = vm->flags;
			switch (jump_type)
			{
			case 1:
				if (flags & PICOVM_FLAG_Z)
					vm->ip = addr;
				break;
			case 2:
				if (~flags & PICOVM_FLAG_Z)
					vm->ip = addr;
				break;
			case 3:
				if (flags & PICOVM_FLAG_N)
					vm->ip = addr;
				break;
			case 4:
				if (~flags & PICOVM_FLAG_N)
					vm->ip = addr;
				break;
			case 5:
				if ( (flags & PICOVM_FLAG_N) && (flags & PICOVM_FLAG_Z) )
					vm->ip = addr;
				break;
			case 6:
				if ( ~(flags & PICOVM_FLAG_N) && (flags & PICOVM_FLAG_Z) )
					vm->ip = addr;
				break;
			}
			break;
			
		}
		// HLT
		case 0xff:
			return -1;
		// YIELD
		case 0xfe:
			vm->ip++;
			return -2;
	}
	
	return 0;
}
