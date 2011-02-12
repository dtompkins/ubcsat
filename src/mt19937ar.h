#ifndef MT19937AR_H
#define MT19937AR_H

#include <stdint.h>

#ifdef __cplusplus 
namespace mersenne {
#endif

extern uint32_t genrand_int32();
extern void init_genrand(uint32_t s);

#ifdef __cplusplus 
}
#endif
#endif
