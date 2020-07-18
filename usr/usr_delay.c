#include "usr_app.h"

#if 0
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

#else
void usr_delay_us(uint32_t us)
{
	uint32_t old_cnt = 0, new_cnt = 0;
	uint32_t t_cnt = 0;

	old_cnt = EPIT2->CNR;
	while(1){
		new_cnt = EPIT2->CNR;
		if(new_cnt < old_cnt){
			t_cnt = old_cnt -new_cnt;
		}else{
			t_cnt = 0xffffffff - new_cnt + old_cnt;
		}

		if(t_cnt >= us){
			break;
		}
	}
}

void usr_delay_ms(uint32_t ms)
{
	uint32_t i = 0;
	for(i = 0; i < ms; i++){
		usr_delay_us(1000);
	}
}


int8_t usr_delay_init(void)
{
	EPIT2->CR = 0;
	EPIT2->CR = (1 << EPIT_ENMOD) | (1 << EPIT_OCIEN) | (1 << EPIT_RLD) | (132 << EPIT_PRESCALAR) |(1 << EPIT_CLKSRC);
	EPIT2->LR = 0xffffffff;//加载寄存器，倒计数值
	EPIT2->CMPR = 0;
	EPIT2->CR |= 1 << EPIT_EN;

	return 0;
}

#endif



