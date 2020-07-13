#ifndef __USR_APP_H
#define __USR_APP_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "core_ca7.h"

#include "usr_common.h"
#include "main.h"

#include "stdio.h"

#include "bsp_int.h"
#include "bsp_gpio.h"
#include "usr_uart.h"
#include "usr_led.h"
#include "usr_beep.h"
#include "usr_key.h"
#include "usr_delay.h"

struct Usr_app
{

    Bsp_int bsp_int;

    int8_t (*usr_app_run)(Usr_app *self);
};


#endif




