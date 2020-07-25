#include "usr_app.h"

TOP_INFO usr_app;

static int8_t usr_app_run(TOP_INFO *self)
{
	char buf[32] = {0};
	stRTC_DATETIME rtc_date;

	usr_led_trigger();
	usr_delay_ms(500);

	memset(&rtc_date, 0, sizeof(stRTC_DATETIME));
	bsp_rtc_get_datetime(&rtc_date);
	sprintf(buf,"%04d/%02d/%02d %02d:%02d:%02d",rtc_date.year, rtc_date.month, rtc_date.day, rtc_date.hour, rtc_date.minute, rtc_date.second);
	usr_lcd_fill(50,150, 300,130, lcd_dev.backcolor);
	usr_lcd_show_string(50, 150, 250, 16, 16,(char*)buf);  /* 显示字符串 */
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
	usr_delay_init ();
//	usr_exti_init (topInfo);
//	bsp_epit_init (EPIT1, 0, EPIT_CLK /2);
//	bsp_epit_init (EPIT2, 0, EPIT_CLK /100);
	bsp_gpt_init(GPT1);
	bsp_gpt_init(GPT2);
	bsp_lcd_init();
	bsp_rtc_init();

    return 0;
}







