
#include <stdio.h>
#include <memory.h>
#include "picodisasm.h"


#define READ8(x)  (*(uint8_t *)(mem + (x)))
#define READ16(x) (*(uint16_t *)(mem + (x)))
#define READ32(x) (*(uint32_t *)(mem + (x)))

static uint8_t get_k(void *mem, uint16_t ip)
{
	uint8_t k = (READ8(ip) & 0xc) >> 2;
	if (k == 3) {
		k = (uint8_t)READ8(ip+1);
	}
	return k;
}

int disassemble(void *mem, uint16_t ip, char *str)
{
	uint16_t addr = 0;
	uint32_t value = 0;
	uint8_t opcode = READ8(ip);

	str[0]=0;

	uint8_t size = 1 << (opcode & 0x03);
	char *size_mod;
	switch (size)
	{
	case 2:
		size_mod = "16";
		break;
	case 4:
		size_mod = "32";
		break;
	default:
		size_mod = "";
		break;
	}

	switch (opcode)
	{
		case 0x00 ... 0x00 + 15: // LOAD addr
		{
			uint8_t cmd = (opcode & 0xc) >> 2;

			switch (cmd)
			{
				case 0:
					addr = READ16(ip + 1);
					sprintf(str, "LOAD%s [0x%x]", size_mod, addr);
					return 4;
				// case 1:
				// 	uint8_t offset = READ8(ip + 1);
				// 	return 2;
				// case 2:
				// {
				// 	struct {
				// 		int16_t offset;
				// 		uint16_t addr;
				// 	} data;

				// 	#pragma GCC diagnostic push
				// 	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
				// 	MEMCPY_4(&data, vm->sp);
				// 	#pragma GCC diagnostic pop
				// 	vm->sp += sizeof(uint16_t) * 2;
				// 	vm->sp -= size;
				// 	MEMCPY_SIZE(vm->sp, vm->mem + data.addr + data.offset);
				// 	vm->ip += 1;
				// 	break;
				// }
				case 3:
					value = 0;
					memcpy(&value, mem + ip + 1, size);
					sprintf(str, "LOAD%s 0x%x", size_mod, value);
					return 1 + size;
			}
			return -1;
		}
		
		case 0x10 ... 0x10 + 9: // STORE addr
		{
			uint8_t cmd = (opcode & 0xc) >> 2;

			switch (cmd)
			{
				case 0:
					addr = READ16(ip + 1);
					sprintf(str, "STORE%s [0x%x]", size_mod, addr);
					return 4;
			}
			
			return -1;
		}
		case 0x1c ... 0x1c + 3: // POP
		{
			sprintf(str, "POP%s", size_mod);
			return 1;
		}
		case 0x20 ... 0x20 + 15: // DUP
		{
			uint8_t k = get_k(mem, ip);
			sprintf(str, "DUP%s %d", size_mod, k);
			return 1;
		}
		case 0x30 ... 0x30 + 15: // DIG
		{
			uint8_t k = get_k(mem, ip);
			sprintf(str, "DIG%s %d", size_mod, k);
			return 1;
		}
		case 0x40: // CALL
			addr = (int16_t)READ16(ip+1);
			sprintf(str, "CALL 0x%x", addr);
			return 3;
		case 0x41: // CALL [ref]
			sprintf(str, "CALL [ref]");
			return 3;
		case 0x42: // RET
		{
			sprintf(str, "RET");
			return 1;
		}
		case 0x43: // CALLUSER
		{
			sprintf(str, "CALLUSER");
			return 1;
		}
		case 0x5c ... 0x5c+3:
		{
			int8_t offset = (int8_t)READ8(ip+1);
			if (offset > 0)
				sprintf(str, "LOAD%s [SFP + %d]", size_mod, offset);
			else 
				sprintf(str, "LOAD%s [SFP - %d]", size_mod, -offset);
			return 2;
		}

		case 0x6c ... 0x6c+3:
		{
			int8_t offset = (int8_t)READ8(ip+1);
			sprintf(str, "STORE%s [SFP + %d]", size_mod, offset);
			return 2;
		}

		case 0x80 ... 0xab: // ARITHMETIC OPERATIONS
		{
			uint8_t cmd = (opcode & 0x3c) >> 2;
			switch (cmd)
			{
				case 0: sprintf(str, "ADD%s", size_mod); break;
				case 1: sprintf(str, "SUB%s", size_mod); break;
				case 2: sprintf(str, "MUL%s", size_mod); break;
				case 3: sprintf(str, "DIV%s", size_mod); break;
				case 4: sprintf(str, "MOD%s", size_mod); break;
				case 5: sprintf(str, "AND%s", size_mod); break;
				case 6: sprintf(str, "OR%s", size_mod); break;
				case 7: sprintf(str, "XOR%s", size_mod); break;
				case 8: sprintf(str, "NOT%s", size_mod); break;
			}
			return 1;
		}
		case 0xac ... 0xac + 4: // FLOATING POINT OPERATIONS
		{
			uint8_t cmd = (opcode & 0x3c) >> 2;
			
			switch (cmd)
			{
				case 11: sprintf(str, "ADDF"); break;
				case 12: sprintf(str, "SUBF"); break;
				case 13: sprintf(str, "MULF"); break;
				case 14: sprintf(str, "DIVF"); break;
			}

			return 1;
		}
		case 0xbc: // CONVI
		{
			printf(str, "CONVI"); break;
			return 1;
		}
		case 0xbc + 1: // CONVF
		{
			printf(str, "CONVF"); break;
			return 1;
		}
		// JMP addr
		case 0xc0 ... 0xc0+16: // Branching
		{			
			uint32_t addr;
			uint8_t size;

			// Get jump address
			if ((opcode & 1) == 0) {
				addr = ip + (int8_t)READ8(ip+1);
				size = 2;
			} else {
				addr = ip + (int16_t)READ16(ip+1);
				size = 3;
			}

			uint16_t jump_type = (opcode & 0xE) >> 1;

			switch (jump_type)
			{
			case 0:
				sprintf(str, "JMP 0x%x", addr); break;
			case 1:
				sprintf(str, "JEQ 0x%x", addr); break;
			case 2:
				sprintf(str, "JNE 0x%x", addr); break;
			case 3:
				sprintf(str, "JLE 0x%x", addr); break;
			case 4:
				sprintf(str, "JGE 0x%x", addr); break;
			case 5:
				sprintf(str, "JLT 0x%x", addr); break;
			case 6:
				sprintf(str, "JGT 0x%x", addr); break;
			}

			return size;
			
		}
		// HLT
		case 0xff:
			sprintf(str, "HLT"); break;
			return 1;
		// YIELD
		case 0xfe:
			sprintf(str, "YIELD"); break;
			return 1;
	}

	return -1;
}