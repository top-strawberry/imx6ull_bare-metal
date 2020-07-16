#include "usr_app.h"

void usr_beep_set(uint8_t state)
{
    if(state){
        kBEEP_ON();
    }else{
        kBEEP_OFF();
    }
}

void usr_beep_trigger(void)
{
    static uint8_t state = 0;
    state = !state;
    usr_beep_set(state);
}

void usr_beep_init(void)
{
    stPIN_CFG beep_cfg;
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10b0);

    beep_cfg.dir = gpio_digital_output;
    beep_cfg.output_logic = gpio_set;
    beep_cfg.int_mode = gpio_no_int;
    bsp_gpio_init(GPIO5, kGPIO5_IO01_BEEP_PIN, &beep_cfg);
}




