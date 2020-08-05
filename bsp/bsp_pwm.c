//
// Created by Administrator on 2020/7/12 0012.
//
#include "usr_app.h"

stPWM_DEV pwm_dev;

static int8_t bsp_pwm_set_period(PWM_Type *pwm, uint32_t value)
{
	uint32_t reg_value = 0;

	if(value < 2){
		value = 2;
	}else{
		reg_value = value - 2;
	}

	pwm->PWMPR = (reg_value & 0xffff);

	return 0;
}

int8_t bsp_pwm_set_duty(PWM_Type *pwm, uint16_t duty)
{
	uint16_t period = 0;
	uint16_t sample = 0;

	period = pwm->PWMPR + 2;
	sample = (uint16_t)(period * duty / 100.0f);
	pwm->PWMSAR = (sample & 0xffff);

	return 0;
}

static void bsp_pwm1_irq_handler(IRQn_Type irq_num, void *arg)
{
	if((PWM1->PWMSR >> emPWMSR_FE) & 0x1){
		bsp_pwm_set_duty(PWM1, pwm_dev.pwm_duty);
		PWM1->PWMSR |= 1 << emPWMSR_FE;// 清除中断标志
	}
}

int8_t bsp_pwm_init(PWM_Type *pwm)
{
	uint8_t i = 0;

	pwm->PWMCR = 0;
	pwm->PWMCR = (1 << emPWMCR_CLKSRC) | (65 << emPWMCR_PRESCALER) | (1 << emPWMCR_FWM);
	bsp_pwm_set_period(pwm, 1000);
	pwm_dev.pwm_duty = 50;
	for(i = 0; i < 4; i ++){
		bsp_pwm_set_duty(pwm, pwm_dev.pwm_duty);
	}

	pwm->PWMIR = 1 << emPWMIR_FIE;
	if(pwm == PWM1){
		bsp_int_system_regiser_irq_handler (PWM1_IRQn, bsp_pwm1_irq_handler, NULL);
		GIC_EnableIRQ (PWM1_IRQn);
	}

	pwm->PWMSR = 0xff;
	pwm->PWMCR |= 1 << emPWMCR_EN;//开启pwm

	return 0;
}