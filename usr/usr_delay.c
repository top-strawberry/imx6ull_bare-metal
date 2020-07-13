#include "usr_delay.h"

// 短延时
static void usr_delay_short(volatile uint32_t n)
{
    while(n--){}
}

/* 延时，一次循环大概是1ms，在主频396MHz
 * n ：延时ms数
 */
void usr_delay_ms(volatile uint32_t ms)
{
    while(ms--) {
        usr_delay_short(0x7ff);
    }
}


