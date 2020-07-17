#ifndef __BSP_EPIT_H
#define __BSP_EPIT_H

#ifdef __cplusplus
extern "C"{
#endif

#define EPIT_CLK    		(66000000U)

typedef enum {
    EPIT_EN = 0,
	EPIT_ENMOD = 1,
	EPIT_OCIEN = 2,
	EPIT_RLD = 3,
	EPIT_PRESCALAR = 4,
	EPIT_SWR = 16,
	EPIT_IOVW = 17,
	EPIT_DBGEN = 18,
	EPIT_WAITEN = 19,
	EPIT_STOPEN = 21,
	EPIT_OM = 22,
	EPIT_CLKSRC = 24,
} emEPIT_CR_REG_BIT;

typedef enum {
    EPIT_OCIF = 0,

} emEPIT_SR_REG_BIT;

extern int8_t bsp_epit_init(EPIT_Type *epit, uint32_t frac, uint32_t value);
extern void bsp_epit_en(EPIT_Type *epit, FunctionalState NewState, uint32_t value);

#ifdef __cplusplus
}
#endif



#endif