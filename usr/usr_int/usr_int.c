#include "fsl_common.h"
#include "MCIMX6Y2.h"
#include "core_ca7.h"
#include "usr_int.h"

static stSYS_IRQ_HANDLER_t irq_table[NUMBER_OF_INT_VECTORS];

void usr_int_system_regiser_irq_handler(IRQn_Type irq_num, system_irq_handler_t irq_handler, void *arg)
{
    irq_table[irq_num].irq_handler = irq_handler;
    irq_table[irq_num].arg = arg;
}

void default_irq_handler(IRQn_Type irq_num, void *arg)
{
    while(1){
        
    }
}

void usr_int_ststem_irq_table_init(void)
{
    uint16_t i = 0;
    for(i = 0; i < NUMBER_OF_INT_VECTORS; i ++){
        irq_table[i].irq_handler = default_irq_handler;
        irq_table[i].arg = NULL;
    }
}

void usr_int_init(void)
{
    GIC_Init();
    usr_int_ststem_irq_table_init();
    __set_VBAR(0x87800000);//设置中断向量偏移地址
}


void system_irq_handler(uint32_t gicc_iar)
{
    IRQn_Type irq_num = gicc_iar & 0x3ff;

    printf("gicc_iar = %d irq_num = %d",gicc_iar, irq_num);
    irq_table[irq_num].irq_handler(irq_num, irq_table[irq_num].arg); 
}



