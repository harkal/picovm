#include "picovm.h"

#define READ8(x) vm->mem_read8((x), vm->ctx)
#define READ16(x) vm->mem_read16((x), vm->ctx)
#define READ32(x) vm->mem_read32((x), vm->ctx)

#define WRITE8(x, v) vm->mem_write8((x), (v), vm->ctx);
#define WRITE16(x, v) vm->mem_write16((x), (v), vm->ctx);
#define WRITE32(x, v) vm->mem_write32((x), (v), vm->ctx);

static inline int16_t signext(uint16_t val, uint16_t mask)
{
	val = val & mask;
	if ((val & ~(mask >> 1)) != 0)
		val |= ~mask;
	return val;
}

static inline void push_stack_8(struct picovm_s *vm, uint8_t value)
{
    vm->sp -= 2;
    vm->mem_write8(vm->sp, value, vm->ctx);
}

static inline void push_stack_16(struct picovm_s *vm, uint16_t value)
{
    vm->sp -= 2;
	
    // vm->mem_write16(vm->sp, value, vm->ctx);
}

static inline void push_stack_32(struct picovm_s *vm, uint32_t value)
{
    vm->sp -= 4;
    vm->mem_write32(vm->sp, value, vm->ctx);
}

static inline uint8_t pop_stack_8(struct picovm_s *vm)
{
    uint8_t value = vm->mem_read32(vm->sp, vm->ctx);
    vm->sp += 1;
    return value;
}

static inline uint16_t pop_stack_16(struct picovm_s *vm)
{
    uint16_t value = vm->mem_read32(vm->sp, vm->ctx);
    vm->sp += 2;
    return value;
}

static inline uint32_t pop_stack_32(struct picovm_s *vm)
{
    uint32_t value = vm->mem_read32(vm->sp, vm->ctx);
    vm->sp += 4;
    return value;
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
			uint8_t size = opcode & 0x07;
			switch (size)
			{
			case 0:
				push_stack_8(vm, READ8(addr));	
				break;
			case 1:
				push_stack_16(vm, READ16(addr));	
				break;
			case 2:
				push_stack_32(vm, READ32(addr));
				break;
			}
            
            vm->ip += 3;
            break;
        }
		
        case 0x08 ... 0x08 + 3: // STORE addr
        {
			uint16_t addr = READ16(vm->ip + 1);
			uint8_t size = opcode & 0x07;
			switch (size)
			{
			case 0:
				WRITE8(addr, pop_stack_8(vm));	
				break;
			case 1:
				WRITE16(addr, pop_stack_16(vm));	
				break;
			case 2:
				WRITE32(addr, pop_stack_32(vm));
				break;
			}            
            vm->ip += 3;
            break;
        }

		case 0x80+0 ... 0x80+14:
			b = pop_stack_32(vm);
			a = pop_stack_32(vm);
			switch (opcode)
			{
				case 0x80 +  0: push_stack_32(vm, a + b);  break;
				case 0x80 +  1: push_stack_32(vm, a - b);  break;
				case 0x80 +  2: push_stack_32(vm, a * b);  break;
				case 0x80 +  3: push_stack_32(vm, a / b);  break;
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

			int32_t top = pop_stack_32(vm);

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
