#include "usr_app.h"

static stSYS_IRQ_HANDLER irq_table[NUMBER_OF_INT_VECTORS];

void bsp_int_system_regiser_irq_handler(IRQn_Type irq_num, system_irq_handler_t irq_handler, void *arg)
{
    if(irq_handler == NULL){
        INT_LOG("bsp_int_system_regiser_irq_handler irq_handler is null\r\n");
        return;
    }

    irq_table[irq_num].irq_handler = irq_handler;
    irq_table[irq_num].arg = arg;
}

static void bsp_int_default_irq_handler(IRQn_Type irq_num, void *arg)
{
    INT_LOG("bsp_int_default_irq_handler irq_num = %d\r\n", irq_num);
    while(1);
}

static void bsp_int_ststem_irq_table_init(void)
{
    uint8_t i = 0;

    for(i = 0; i < NUMBER_OF_INT_VECTORS; i ++){
        irq_table[i].irq_handler = bsp_int_default_irq_handler;
        irq_table[i].arg = NULL;
    }
}




void system_irq_handler(uint32_t gicc_iar)
{
    IRQn_Type irq_num = gicc_iar & 0x3ff;

    INT_LOG("gicc_iar = %d irq_num = %d\r\n",gicc_iar, irq_num);
    irq_table[irq_num].irq_handler(irq_num, irq_table[irq_num].arg);
}


void bsp_int_init(TOP_INFO * topInfo)
{
    Bsp_int * self = &topInfo->bsp_int;

    self->bsp_int_system_regiser_irq_handler = bsp_int_system_regiser_irq_handler;
    GIC_Init();
    bsp_int_ststem_irq_table_init();
    __set_VBAR(0x87800000);//设置中断向量偏移地址
}


