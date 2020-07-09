#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "usr_led.h"

extern void usr_delay_ms(volatile uint32_t ms);



/* 初始化LED */
void usr_led_init(void)
{
    // IOMUX_SW_MUX->GPIO1_IO03 = 0x5;    /* 复用为GPIO1——IO03*/
    // IOMUX_SW_PAD->GPIO1_IO03 = 0X10B0; /*设置GPIO1_IO03电气属性 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10b0);

    // GPIO初始化 
    GPIO1->GDIR = 0x8;           // 设置为输出 
    GPIO1->DR = 0X8;             // 关闭LED灯 
}

void usr_led_set(uint8_t state)
{
    if(state){
        LED_ON();
    }else{
        LED_OFF();
    }
}

void usr_led_trigger(uint32_t ms)
{
    static uint8_t state = 0;
    state = !state;
    usr_led_set(state);
    usr_delay_ms(ms);
}