#ifndef __USR_APP_H
#define __USR_APP_H

#include "main.h"

#include "stdio.h"

#include "bsp_int.h"
#include "bsp_clk.h"
#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "bsp_epit.h"
#include "bsp_gpt.h"
#include "bsp_lcd.h"
#include "bsp_rtc.h"



#include "usr_exti.h"
#include "usr_led.h"
#include "usr_beep.h"
#include "usr_key.h"
#include "usr_delay.h"
#include "usr_lcd.h"


struct Usr_app
{

    Bsp_int bsp_int;

    int8_t (*usr_app_run)(TOP_INFO *self);
};


#endif




