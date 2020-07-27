#include "usr_app.h"



void usr_led_set(uint8_t state)
{
	if(state){
		kLED_ON();
	}else{
		kLED_OFF();
	}
}

void usr_led_trigger(void)
{
	static uint8_t state = 0;
	state = !state;
	usr_led_set(state);
}

void usr_led_init(void)
{
	stPIN_CFG led_cfg;
	// IOMUX_SW_MUX->GPIO1_IO03 = 0x5;    /* 复用为GPIO1——IO03*/
	// IOMUX_SW_PAD->GPIO1_IO03 = 0X10B0; /*设置GPIO1_IO03电气属性 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);

	/* 配置I2C2 IO属性        
		*bit 16:0 HYS关闭
		*bit [15:14]: 1 默认47K上拉
		*bit [13]: 1 pull功能
		*bit [12]: 1 pull/keeper使能 
		*bit [11]: 0 关闭开路输出
		*bit [7:6]: 10 速度100Mhz
		*bit [5:3]: 110 驱动能力为R0/6
		*bit [0]: 1 高转换率
	*/
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10b0);

	led_cfg.dire = gpio_digital_output;
	led_cfg.output_logic = gpio_set;
	led_cfg.int_mode = gpio_no_int;
	bsp_gpio_init(GPIO1, kGPIO1_IO03_LED_PIN, &led_cfg);
}




