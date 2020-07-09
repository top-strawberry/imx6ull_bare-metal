#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "usr_key.h"
#include "usr_led.h"

#include "stdio.h"

extern void usr_clk_init(void);

extern void usr_beep_init(void);
extern void usr_beep_set(uint8_t state);
extern void usr_delay_ms(volatile uint32_t ms);
extern uint8_t usr_key_get_value(void);
extern void usr_uart_init(UART_Type * uart, uint32_t baud);

int main(void)
{
    uint8_t beep_state = 0;
    int a, b;
    int c = 100;

    usr_led_init(); 
    usr_beep_init();
    usr_clk_init();

    usr_uart_init(UART1, 115200);

    // while(1) {
    //     if (usr_key_get_value() == KEY_PRESS) {
    //         beep_state = !beep_state;
    //         usr_beep_set(beep_state);
    //     }
    //     usr_led_trigger(1000);
    // }

    printf("%d的十六进制为：%#x\r\n", c, c);
    while(1) {
        printf("请输入两个整数，使用空格隔开:");
        scanf("%d %d", &a, &b);
        printf("\r\n 数据%d+%d=%d\r\n", a, b, a + b);
    }

    return 0;
}
