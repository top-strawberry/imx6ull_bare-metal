#ifndef __BSP_CLK_H
#define __BSP_CLK_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif


typedef enum
{
    cacrr_div1 = 0,
    cacrr_div2,
    cacrr_div3,
    cacrr_div4,
    cacrr_div5,
    cacrr_div6,
    cacrr_div7,
    cacrr_div8,
} emCCM_CACRR_DIV;

typedef enum
{
    ahb_podf_div1 = 0,
    ahb_podf_div2,
    ahb_podf_div3,
    ahb_podf_div4,
    ahb_podf_div5,
    ahb_podf_div6,
    ahb_podf_div7,
    ahb_podf_div8,
} emCCM_CBCDR_AHB_PODF_DIV;

typedef enum
{
    ipg_podf_div1 = 0,
    ipg_podf_div2,
    ipg_podf_div3,
    ipg_podf_div4,
} emCCM_CBCDR_IPG_PODF_DIV;

typedef enum
{
    perclk_podf_div1 = 0,
    perclk_podf_div2,
    perclk_podf_div3,
    perclk_podf_div4,
    perclk_podf_div5,
    perclk_podf_div6,
    perclk_podf_div7,
    perclk_podf_div8 = 0x3f

} emCCM_CSCMR1_PERCLK_PODF_DIV;

typedef enum
{
    clk_podf_div1 = 0,       //000000 divide by 1
    clk_podf_div2 = 0x3f,    //111111 divide by 2^6
} emCCM_UART_CLK_PODF_DIV;

typedef enum
{
    ccm_lcdif1_pred_div1 = 0,
    ccm_lcdif1_pred_div2 = 1,
    ccm_lcdif1_pred_div3 = 2,
    ccm_lcdif1_pred_div4 = 3,
    ccm_lcdif1_pred_div5 = 4,
    ccm_lcdif1_pred_div6 = 5,
    ccm_lcdif1_pred_div7 = 6,
    ccm_lcdif1_pred_div8 = 7,
} emCCM_LCDIF1_PRED_DIV;


typedef enum
{
    ARM_PODF = 0,
} emCCM_CACRR_REG_BIT;

typedef enum
{
    PLL3_SW_CLK_SEL     = 0,
    PLL1_SW_CLK_SEL     = 2,
    SECONDARY_CLK_SEL   = 3,
    STEP_SEL            = 8,
} emCCM_CCSR_REG_BIT;

typedef enum
{
    PERIPH2_CLK2_PODF   = 0,
    FABRIC_MMDC_PODF    = 3,
    AXI_SEL             = 6,
    AXI_ALT_SEL         = 7,
    IPG_PODF            = 8,
    AHB_PODF            = 10,
    AXI_PODF            = 16,
    PERIPH2_CLK_SEL     = 26,
    PERIPH_CLK2_PODF    = 27
} emCCM_CBCDR_REG_BIT;

typedef enum
{
    CCM_PERIPH_CLK2_SEL     = 12,
	CCM_PRE_PERIPH_CLK_SEL = 18,
	CCM_PERIPH2_CLK2_SEL = 20,
	CCM_PRE_PERIPH2_CLK_SEL =21,
	CCM_LCDIF1_PODF = 23,
} emCCM_CSCMR_REG_BIT;


typedef enum
{
    PERCLK_PODF     = 0,
    PERCLK_CLK_SEL  = 6,
    QSPI1_CLK_SEL   = 7,
    SAI1_CLK_SEL    = 10,
    SAI2_CLK_SEL    = 12,
    SAI3_CLK_SEL    = 14,
    USDHC1_CLK_SEL  = 16,
    USDHC2_CLK_SEL  = 17,
    BCH_CLK_SEL     = 18,
    GPMI_CLK_SEL    = 19,
    ACLK_EIM_SLOW_PODF  = 23,
    QSPI1_PODF      = 26,
    ACLK_EIM_SLOW_SEL   = 29,
} emCCM_CSCMR1_REG_BIT;

typedef enum
{
    AXI_PODF_BUSY   = 0,
    AHB_PODF_BUSY   = 1,

} emCCM_CDHIPR_REG_BIT;


typedef enum
{
    DIV_SELECT      = 0,
    POWERDOWN       = 12,
    ENABLE          = 13,
    BYPASS_CLK_SRC  = 14,
    BYPASS          = 16,
    LOCKLOCK        = 31,
} emCCM_ANALOG_PLL_ARM_REG_BIT;


typedef enum
{
    PFD0_FRAC       = 0,
    PFD0_STABLE     = 6,
    PFD0_CLKGATE    = 7,
    PFD1_FRAC       = 8,
    PFD1_STABLE     = 14,
    PFD1_CLKGATE    = 15,
    PFD2_FRAC       = 16,
    PFD2_STABLE     = 22,
    PFD2_CLKGATE    = 23,
    PFD3_FRAC       = 24,
    PFD3_STABLE     = 30,
    PFD3_CLKGATE    = 31,
} emCCM_ANALOG_PFD_REG_BIT;


typedef enum
{
    UART_CLK_PODF   = 0,
    UART_CLK_SEL    = 6,    //0:derive clock from pll3_80m 1:derive clock from osc_clk
    USDHC1_PODF     = 11,
    USDHC2_PODF     = 16,
    BCH_PODF        = 19,
    GPMI_PODF       = 22,
} emCCM_CSCDR1_REG_BIT;


typedef enum
{
    CCM_LCDIF1_CLK_SEL   = 9,
	CCM_LCDIF1_PRED = 12,
	CCM_LCDIF1_PRE_CLK_SEL = 15,
	CCM_ECSPI_CLK_SEL = 18,
	CCM_ECSPI_CLK_PODF = 19,
} emCCM_CSCDR2_REG_BIT;



typedef enum
{
    CCM_DIV_SELECT    = 0,
	CCM_POWERDOWN = 12,
	CCM_ENABLE = 13,
	CCM_BYPASS_CLK_SRC = 14,
	CCM_BYPASS = 16,
	CCM_PFD_OFFSET_EN = 18,
	CCM_POST_DIV_SELECT = 19,
	CCM_LOCK = 31,
} emCCM_ANALOG_PLL_VIDEO_REG_BIT;


typedef enum
{
    CCM_VIDEO_DIV    = 30,

} emCCM_ANALOG_MISC2_REG_BIT;




extern void bsp_clk_init(void);


#ifdef __cplusplus
}
#endif

#endif




