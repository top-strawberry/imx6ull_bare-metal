#ifndef __BSP_GPT_H
#define __BSP_GPT_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

#define GPT_LOG            	PRINTF
#define GPT_CLK    			(80000000U)


//typedef enum {
//    rfdiv_div1 = 5,//101 Divide input clock by 1


//} emUART_UCR4_RFDIV_DIV;

typedef enum {
    GPT_EN = 0,
	GPT_ENMOD = 1,
	GPT_DBGEN = 2,
	GPT_WAITEN = 3,
	GPT_DOZEEN = 4,
	GPT_STOPEN = 5,
	GPT_CLKSRC = 6,
	GPT_FRR = 9,
	GPT_EN_24M = 10,
	GPT_SWR = 15,
	GPT_IM1 = 16,
	GPT_IM2 = 18,
	GPT_OM1 = 20,
	GPT_OM2 = 23,
	GPT_OM3 = 26,
	GPT_FO1 = 29,
	GPT_FO2 = 30,
	GPT_FO3 = 31,
} emGPT_CR_REG_BIT;

typedef enum {
    GPT_OF1IE = 0,
	GPT_OF2IE = 1,
	GPT_OF3IE = 2,
	GPT_IF1IE = 3,
	GPT_IF2IE = 4,
	GPT_ROVIE = 5,
} emGPT_IR_REG_BIT;




extern int8_t bsp_gpt_init(GPT_Type *gpt);


#ifdef __cplusplus
}
#endif

#endif




