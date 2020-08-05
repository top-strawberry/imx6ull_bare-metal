//
// Created by Administrator on 2020/7/12 0012.
//

#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

#define PWM_LOG            PRINTF

typedef enum
{
	emPWMSR_FIFOAV = 0,
	emPWMSR_FE = 3,
	emPWMSR_ROV = 4,
	emPWMSR_CMP = 5,
	emPWMSR_FWE = 6,
} emPWMSR_REG_BIT;

typedef enum
{
	emPWMCR_EN = 0,
	emPWMCR_REPEAT = 1,
	emPWMCR_SWR = 3,
	emPWMCR_PRESCALER = 4,
	emPWMCR_CLKSRC = 16,
	emPWMCR_POUTC = 18,
	emPWMCR_HCTR = 20,
	emPWMCR_BCTR = 21,
	emPWMCR_DBGEN = 22,
	emPWMCR_WAITEN = 23,
	emPWMCR_DOZEN = 24,
	emPWMCR_STOPEN = 25,
	emPWMCR_FWM = 26,
} emPWMCR_REG_BIT;

typedef enum
{
	emPWMIR_FIE = 0,
	emPWMIR_RIE = 1,
	emPWMIR_CIE = 2,	
} emPWMIR_REG_BIT;


typedef struct 
{
	uint8_t pwm_duty;

} stPWM_DEV;


extern stPWM_DEV pwm_dev;
extern int8_t bsp_pwm_init(PWM_Type *pwm);

#ifdef __cplusplus
}
#endif

#endif //






