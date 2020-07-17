#include "usr_app.h"

void bsp_epit_en(EPIT_Type *epit, FunctionalState NewState, uint32_t value)
{
	if(NewState == enable){
		epit->CR &= ~(1 << EPIT_EN);
		if(value != 0) {
			epit->LR = value;
		}
		epit->CR |= 1 << EPIT_EN;
	}else if(NewState == disable){
		epit->CR &= ~(1 << EPIT_EN);
	}

}

static void bsp_epit_1_irq_handler(IRQn_Type irq_num, void *arg)
{
	static uint8_t state = 0;
	state = !state;

	if((EPIT1->SR >> EPIT_OCIF) & 0x01){
		usr_led_set (state);
	}

	EPIT1->SR |= 1 << EPIT_OCIF;
}

static void bsp_epit_2_irq_handler(IRQn_Type irq_num, void *arg)
{
	if((EPIT2->SR >> EPIT_OCIF) & 0x01){
		bsp_epit_en(EPIT2, disable, 0);
		if(bsp_gpio_pin_read (GPIO1, kGPIO1_IO18_KEY_PIN) == 0){
			usr_beep_trigger ();
		}
	}

	EPIT2->SR |= 1 << EPIT_OCIF;
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
	if(epit == EPIT1){
		bsp_int_system_regiser_irq_handler (EPIT1_IRQn, bsp_epit_1_irq_handler, NULL);
		GIC_EnableIRQ (EPIT1_IRQn);
		epit->CR |= 1 << EPIT_EN;
	}else if((epit == EPIT2)){
		bsp_int_system_regiser_irq_handler (EPIT2_IRQn, bsp_epit_2_irq_handler, NULL);
		GIC_EnableIRQ (EPIT2_IRQn);
		epit->CR &= ~(1 << EPIT_EN);
	}

	return 0;
}








