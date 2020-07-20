#ifndef __USR_DELAY_H
#define __USR_DELAY_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

extern void usr_delay_ms(volatile uint32_t ms);
extern int8_t usr_delay_init(void);


#ifdef __cplusplus
}
#endif

#endif


