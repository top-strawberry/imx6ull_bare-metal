#include "usr_app.h"



static void gpio1_io18_irq_handler(IRQn_Type irq_num, void *arg)
{
	bsp_epit_en(EPIT2, enable, EPIT_CLK / 100);
    bsp_gpio_clear_int_flag (GPIO1, kGPIO1_IO18_KEY_PIN);
}


void usr_exti_init(TOP_INFO * topInfo)
{
	stPIN_CFG key_cfg;
    Bsp_int * bsp_int = &topInfo->bsp_int;

	IOMUXC_SetPinMux (IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
	IOMUXC_SetPinConfig (IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);
	key_cfg.dir = gpio_digital_input;
	key_cfg.int_mode = gpio_fall_int;
    bsp_gpio_init (GPIO1, kGPIO1_IO18_KEY_PIN, &key_cfg);
	GIC_EnableIRQ (GPIO1_Combined_16_31_IRQn);
    bsp_int->bsp_int_system_regiser_irq_handler (GPIO1_Combined_16_31_IRQn, gpio1_io18_irq_handler, NULL);
    bsp_gpio_int_enable (GPIO1, kGPIO1_IO18_KEY_PIN);
}






