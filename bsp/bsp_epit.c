#include "usr_app.h"

void bsp_epit_1_irq_handler(IRQn_Type irq_num, void *arg)
{
	static uint8_t state = 0;
	state = !state;

	if((EPIT1->SR >> EPIT_OCIF) & 0x01){
		usr_led_set (state);
	}
	EPIT1->SR |= 1 << EPIT_OCIF;
}



int8_t bsp_epit_init(EPIT_Type *epit, uint32_t frac, uint32_t value)
{
	if(frac > 4095){
		frac = 4095;
	}

	epit->CR = 0;
	epit->CR = (1 << EPIT_ENMOD) | (1 << EPIT_OCIEN) | (1 << EPIT_RLD) | (frac << EPIT_PRESCALAR) | (1 << EPIT_CLKSRC);
	epit->LR = value;//加载寄存器，倒计数值
	epit->CMPR = 0;

	//初始化中断
	bsp_int_system_regiser_irq_handler (EPIT1_IRQn, bsp_epit_1_irq_handler, NULL);
	GIC_EnableIRQ (EPIT1_IRQn);

	epit->CR |= 1 << EPIT_EN;

	return 0;
}








