#include "picovm.h"

#define READ8(x)  (*(uint8_t *)(vm->mem + (x)))
#define READ16(x) (*(uint16_t *)(vm->mem + (x)))
#define READ32(x) (*(uint32_t *)(vm->mem + (x)))

#define WRITE8(x, v)  (*(uint8_t *)(vm->mem + (x)) = (v))
#define WRITE16(x, v) (*(uint16_t *)(vm->mem + (x)) = (v))
#define WRITE32(x, v) (*(uint32_t *)(vm->mem + (x)) = (v))

static inline int16_t signext(uint16_t val, uint16_t mask)
{
	val = val & mask;
	if ((val & ~(mask >> 1)) != 0)
		val |= ~mask;
	return val;
}

static inline read_mem(struct picovm_s *vm, uint16_t addr, uint8_t size, void *value)
{
	memcpy(value, vm->mem + addr, size);
}

static inline write_mem(struct picovm_s *vm, uint16_t addr, void *value, uint8_t size)
{
	memcpy(vm->mem + addr, value, size);
}

static inline void push_stack(struct picovm_s *vm, void *value, uint8_t size)
{
    vm->sp -= size;
	memcpy(vm->mem + vm->sp, value, size);
}

static inline void pop_stack(struct picovm_s *vm, uint8_t size, void *value)
{
	memcpy(value, vm->mem + vm->sp, size);
	vm->sp += size;
}

int8_t picovm_exec(struct picovm_s *vm)
{
    uint8_t opcode = READ8(vm->ip);
    uint32_t a, b, addr;
    switch (opcode)
	{
        case 0x00 ... 0x00 + 3: // LOAD addr
        {
            uint16_t addr = READ16(vm->ip + 1);
			uint8_t size = 1 << (opcode & 0x07);
			uint32_t value;
			read_mem(vm, addr, size, &value);
			push_stack(vm, &value, size);            
            vm->ip += 3;
            break;
        }
		
        case 0x08 ... 0x08 + 3: // STORE addr
        {
			uint16_t addr = READ16(vm->ip + 1);
			uint8_t size = 1 << (opcode & 0x07);
			uint32_t value;
			pop_stack(vm, size, &value);
			write_mem(vm, addr, &value, size);
            vm->ip += 3;
            break;
        }

		case 0x80+0 ... 0x80+14:
			pop_stack(vm, 4, &b);
			pop_stack(vm, 4, &a);
			switch (opcode)
			{
				case 0x80 +  0: a += b; break;
				case 0x80 +  1: a -= b; break;
				case 0x80 +  2: a *= b; break;
				case 0x80 +  3: a /= b; break;
				// case 0x80 +  4: push_stack_32(vm, a % b);  break;
				// case 0x80 +  5: push_stack_32(vm, a << b); break;
				// case 0x80 +  6: push_stack_32(vm, a >> b); break;
				// case 0x80 +  7: push_stack_32(vm, a & b);  break;
				// case 0x80 +  8: push_stack_32(vm, a | b);  break;
				// case 0x80 +  9: push_stack_32(vm, a ^ b);  break;
				// case 0x80 + 10: push_stack_32(vm, a && b); break;
				// case 0x80 + 11: push_stack_32(vm, a || b); break;
				// case 0x80 + 12: push_stack_32(vm, ~a);     break;
				// case 0x80 + 13: push_stack_32(vm, -a);     break;
				// case 0x80 + 14: push_stack_32(vm, !a);     break;
			}
			push_stack(vm, &a, 4);
			vm->ip++;
			break;
		// JMP addr
		case 0xa0 ... 0xa0+16:
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

			int32_t top;
			pop_stack(vm, 4, &top);

			printf("%d, t: %d\n", top, jump_type);

			switch (jump_type)
			{
			case 1:
				if (top == 0)
					vm->ip = addr;
				break;
			case 2:
				if (top != 0)
					vm->ip = addr;
				break;
			case 3:
				if (top < 0)
					vm->ip = addr;
				break;
			case 4:
				if (top > 0)
					vm->ip = addr;
				break;
			case 5:
				if (top <= 0)
					vm->ip = addr;
				break;
			case 6:
				if (top >= 0)
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
