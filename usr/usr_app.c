#include "usr_app.h"


Usr_app usr_app;

extern void bsp_int_init(Usr_app * topInfo);
extern void usr_clk_init(void);
extern int8_t usr_uart_init(UART_Type *uart, uint32_t baud);

extern void usr_exti_init(Usr_app * topInfo);


static int8_t usr_app_run(Usr_app *self)
{
    return 0;
}

int8_t usr_app_init(Usr_app *self)
{
    Usr_app * topInfo = self;

    memset(self, 0, sizeof(Usr_app));
    self->usr_app_run = usr_app_run;
    usr_clk_init();
    usr_uart_init(UART1, 115200);
	bsp_int_init(topInfo);
    usr_led_init();
    usr_exti_init(topInfo);

    return 0;
}







