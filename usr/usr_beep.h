#ifndef __USR_BEEP_H
#define __USR_BEEP_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "usr_common.h"

#ifdef __cplusplus
extern "C"{
#endif

#define kGPIO5_IO01_LED_PIN (1)
#define kBEEP_ON()          GPIO5->DR &= ~(1 << 1)     // bit1清零
#define kBEEP_OFF()         GPIO5->DR |=  (1 << 1)     // bit1置1

extern void usr_beep_init(void);
extern void usr_beep_set(uint8_t state);
extern void usr_beep_trigger(void);

#ifdef __cplusplus
}
#endif

#endif
