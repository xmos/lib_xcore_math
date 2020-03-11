

#ifndef TST_COMMON_H_
#define TST_COMMON_H_

#include <stdint.h>

#ifdef __xcore__
#define WORD_ALIGNED __attribute__ ((aligned (4)))
#else
#define WORD_ALIGNED
#endif

#ifndef DEBUG_ON
#define DEBUG_ON    (0)
#endif

#define PRINTF(...)     do{if (DEBUG_ON) {printf(__VA_ARGS__);}} while(0)

int16_t  pseudo_rand_int16(unsigned *r);
uint16_t pseudo_rand_uint16(unsigned *r);
int32_t  pseudo_rand_int32(unsigned *r);
uint32_t pseudo_rand_uint32(unsigned *r);
int64_t  pseudo_rand_int64(unsigned *r);
uint64_t pseudo_rand_uint64(unsigned *r);


void pseudo_rand_bytes(unsigned *r, char* buffer, unsigned size);

void print_warns(
    int start_case, 
    unsigned test_c, 
    unsigned test_asm);

#endif //TST_COMMON_H_