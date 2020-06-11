
#ifndef PICOVM_H
#define PICOVM_H

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define PICOVM_FLAG_N 1
#define PICOVM_FLAG_Z 2

struct picovm_s
{
	uint16_t ip;
    uint8_t *sp;
    uint8_t flags;

	void *mem;
    void *ctx;

    void (*call_user)(void *ctx);
};

extern int8_t picovm_exec(struct picovm_s *vm);

#ifdef __cplusplus
}
#endif

#endif
