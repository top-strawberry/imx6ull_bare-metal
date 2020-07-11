#ifndef __USR_INT_H
#define __USR_INT_H

typedef void (*system_irq_handler_t)(IRQn_Type irq_num, void *arg);

typedef struct
{
    system_irq_handler_t irq_handler;  
    void * arg;
} stSYS_IRQ_HANDLER_t;


#endif // !__USR_INT_H





