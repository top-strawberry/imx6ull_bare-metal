#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "usr_beep.h"


void usr_beep_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10b0);
    GPIO5->GDIR = 0x2;           // 设置为输出 
    BEEP_OFF();
}                       

void usr_beep_set(uint8_t state)
{
    if(state){
        BEEP_ON();
    }else{
        BEEP_OFF();
    }
}