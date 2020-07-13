#ifndef __USR_LED_H
#define __USR_LED_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "usr_common.h"

#ifdef __cplusplus
extern "C"{
#endif

#define kGPIO1_IO03_LED_PIN (3)
#define kLED_ON()           GPIO1->DR &= ~(1 << 3)    // bit3清零
#define kLED_OFF()          GPIO1->DR |= (1 << 3)     // bit3置1

extern void usr_led_init(void);
extern void usr_led_set(uint8_t state);
extern void usr_led_trigger(void);

#ifdef __cplusplus
}
#endif

#endif



