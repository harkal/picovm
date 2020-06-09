
#ifndef PICOVM_H
#define PICOVM_H

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct picovm_s
{
	uint16_t ip, sp;
	void *mem;
};

extern int8_t picovm_exec(struct picovm_s *vm);

#ifdef __cplusplus
}
#endif

#endif
