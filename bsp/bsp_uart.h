#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

#define UART_LOG            PRINTF
#define UART_CLK    		(80000000U)


typedef enum {
    rfdiv_div1 = 5,//101 Divide input clock by 1
    rfdiv_div2 = 4,//100 Divide input clock by 2
    rfdiv_div3 = 3,//011 Divide input clock by 3
    rfdiv_div4 = 2,//010 Divide input clock by 4
    rfdiv_div5 = 1,//001 Divide input clock by 5
    rfdiv_div6 = 0,//000 Divide input clock by 6
    rfdiv_div7 = 6,//110 Divide input clock by 7

} emUART_UCR4_RFDIV_DIV;

typedef enum {
    UARTEN = 0,
    DOZE = 1,
    ATDMAEN = 2,
    TXDMAEN = 3,
    SNDBRK = 4,
    RTSDEN = 5,
    TXMPTYEN = 6,
    IREN = 7,
    RXDMAEN = 8,
    RRDYEN = 9,
    ICD = 10,
    IDEN = 12,
    TRDYEN = 13,
    ADBR = 14,
    ADEN = 15,

} emUART_UCR1_REG_BIT;

typedef enum {
    SRST = 0,
    RXEN = 1,
    TXEN = 2,
    ATEN = 3,
    RTSEN = 4,
    WS = 5,
    STPB = 6,
    PROE = 7,
    PREN = 8,
    RTEC = 9,
    ESCEN = 11,
    CTS = 12,
    CTSC = 13,
    IRTS = 14,
    ESCI = 15,
} emUART_UCR2_REG_BIT;

typedef enum {
    ACIEN = 0,
    INVT = 1,
    RXDMUXSEL = 2,
} emUART_UCR3_REG_BIT;

typedef enum {
    RFDIV = 7,
} emUART_UCR4_REG_BIT;


//extern void putc(unsigned char c);
//extern void puts(char *str);
//extern unsigned char getc(void);


//typedef struct Usr_uart Usr_uart;
//struct Usr_uart
//{
//
//};


extern int8_t bsp_uart_init(UART_Type *uart, uint32_t baud);


#ifdef __cplusplus
}
#endif

#endif




