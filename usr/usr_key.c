#include "usr_app.h"

void usr_key_init()
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    GPIO1->GDIR &= ~(1 << 18); //
}

uint8_t usr_key_read()
{
    return ( (GPIO1->DR >> 18) & 0x1);
}

uint8_t usr_key_get_value(void)
{
    uint8_t ret = KEY_RELEASE;
    static uint8_t release = 1;
    if(release == 1){
        if(usr_key_read() == KEY_PRESS){
            usr_delay_ms(30);
            if(usr_key_read() == KEY_PRESS){
                release = 0;
                ret = KEY_PRESS;
            }
        }
    }else if(usr_key_read() == KEY_RELEASE){
        release = 1;
        ret = KEY_RELEASE;
    }

    return ret;
}







