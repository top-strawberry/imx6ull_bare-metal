#include "usr_app.h"

TOP_INFO usr_app;

static int8_t usr_app_run(TOP_INFO *self)
{
	//usr_led_trigger();
	//usr_delay_ms (100);
    return 0;
}

int8_t usr_app_init(TOP_INFO *self)
{
    TOP_INFO * topInfo = self;

    memset(self, 0, sizeof(TOP_INFO));
    self->usr_app_run = usr_app_run;
	bsp_int_init (topInfo);
	bsp_clk_init ();
	bsp_uart_init (UART1, 115200);
	usr_led_init();
	usr_beep_init ();
//	usr_delay_init ();
	usr_exti_init (topInfo);
	bsp_epit_init (EPIT1, 0, EPIT_CLK /2);
	bsp_epit_init (EPIT2, 0, EPIT_CLK /100);

	bsp_gpt_init(GPT1);
	bsp_gpt_init(GPT2);


    return 0;
}







