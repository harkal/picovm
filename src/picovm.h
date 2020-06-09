
#ifndef PICOVM_H
#define PICOVM_H

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct picovm_s
{
	uint16_t ip, sp;
	void *ctx;

    uint8_t  (*mem_read8)(uint16_t addr, void *ctx);
    uint16_t (*mem_read16)(uint16_t addr, void *ctx);
	uint32_t (*mem_read32)(uint16_t addr, void *ctx);
    void    (*mem_write8)(uint16_t addr, uint8_t value, void *ctx);
    void    (*mem_write16)(uint16_t addr, uint16_t value, void *ctx);
	void    (*mem_write32)(uint16_t addr, uint32_t value, void *ctx);
};

extern int8_t picovm_exec(struct picovm_s *vm);

#ifdef __cplusplus
}
#endif

#endif
