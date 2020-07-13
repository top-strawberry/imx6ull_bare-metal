#include "usr_app.h"

#define UART_CLK    (80000000U)

//通过UART1发送一个字符
void putc(unsigned char c)
{
    while(((UART1->USR2 >> 3 ) & 0X01) == 0);
    UART1->UTXD = c;
}

//通过串口接收数据
unsigned char getc(void)
{
     while(((UART1->USR2) & 0X01) == 0); //等待有数据可以读取
     return UART1->URXD;
}

// 通过串口发送一串字符
void puts(char *str)
{
    char *p = str;

    while(*p){
        putc(*p++);
    }
}


// raise函数，防止编译报错
void raise(int sig_nr)
{

}

void usr_uart_en_set(UART_Type *uart, FunctionalState NewState)
{
    if(NewState == enable){
        uart->UCR1 |= 0x1 << UARTEN;
    }else if(NewState == disable){
        uart->UCR1 &= ~(0x1 << UARTEN);
    }
}

static int8_t usr_uart_baud_set(UART_Type *uart, uint32_t baud)
{
    if(uart == NULL){
        return -1;
    }

    uint32_t numerator = 0u;		//分子
    uint32_t denominator = 0U;		//分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = UART_CLK;
    denominator = baud << 4;
    divisor = 1;

    while (denominator != 0){
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = UART_CLK / divisor;
    denominator = (baud << 4) / divisor;

    //numerator ranges from 1 ~ 7 * 64k
    //denominator ranges from 1 ~ 64k
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK)){
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator){
            numerator = 1;
        }

        if (0 == denominator){
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider){
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = UART_CLK;
    tempDenominator = (numerator << 4);
    divisor = 1;
    //get the approximately maximum divisor
    while (tempDenominator != 0){
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }

    tempNumerator = UART_CLK / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baud) ? (baudDiff - baud) : (baud - baudDiff);

    if (baudDiff < (baud / 100) * 3){
        uart->UFCR &= ~UART_UFCR_RFDIV_MASK;
        uart->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        uart->UBIR = UART_UBIR_INC(denominator - 1); //要先写UBIR寄存器，然后在写UBMR寄存器，3592页
        uart->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }

    return 0;
}

static void usr_uart_gpio_init(UART_Type *uart)
{
    if(uart == UART1) {
        IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
        IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
        IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10b0);
    }
}

int8_t usr_uart_init(UART_Type *uart, uint32_t baud)
{
    if(uart == NULL){
        return -1;
    }

    //1、配置UART时钟，并开启UART外设时钟。
    //2、配置UART引脚
    usr_uart_gpio_init(uart);

    //3、失能UART
    uart->UCR1 = 0;
    usr_uart_en_set(uart, disable);

    //4、配置UART参数(数据校验，数据位，停止位)
    uart->UCR2 = 0;
    uart->UCR2 |= (0x1 << WS) | (0x1 << IRTS);
    uart->UCR3 = 0;
    uart->UCR3 |= 0x1 << RXDMUXSEL;

    //5、设置波特率
    //设置时钟分频
    uart->UFCR &= ~(0x7 << RFDIV);
    uart->UFCR |= rfdiv_div1 << RFDIV; //1分频
    usr_uart_baud_set(uart, baud);
    //6、使能UART接收和发送
    uart->UCR2 |= (0x1 << RXEN) | (0x1 << TXEN);
    //7、使能UART
    usr_uart_en_set(uart, enable);

    UART_LOG("hello i.max6ull\r\n");

    return 0;
}






