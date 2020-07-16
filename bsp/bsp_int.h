#ifndef __BSP_INT_H
#define __BSP_INT_H

#include "usr_common.h"
#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif


#define INT_LOG            PRINTF

typedef void (*system_irq_handler_t)(IRQn_Type irq_num, void *arg);

typedef struct {
    system_irq_handler_t irq_handler;
    void *arg;
} stSYS_IRQ_HANDLER;

typedef struct Bsp_int Bsp_int;
struct Bsp_int
{
    void (*bsp_int_system_regiser_irq_handler)(IRQn_Type irq_num, system_irq_handler_t irq_handler, void *arg);
};

extern void bsp_int_init(TOP_INFO * topInfo);
extern void bsp_int_system_regiser_irq_handler(IRQn_Type irq_num, system_irq_handler_t irq_handler, void *arg);

#ifdef __cplusplus
}
#endif

#endif // !__USR_INT_H





