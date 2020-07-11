
#ifndef PICODISASM_H
#define PICODISASM_H

#include <stdint.h>

int disassemble(void *mem, uint16_t ip, char *str);

#endif