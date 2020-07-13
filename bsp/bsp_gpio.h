//
// Created by Administrator on 2020/7/12 0012.
//

#ifndef __USR_GPIO_H
#define __USR_GPIO_H

#include "usr_common.h"
#include "main.h"

#ifdef __cplusplus
#define "C"{
#endif

#define GPIO_LOG            PRINTF

typedef enum {
    gpio_no_int = 0U,
    gpio_low_int = 1U,
    gpio_high_int = 2U,
    gpio_rise_int = 3U,
    gpio_fall_int = 4U,
    gpio_rise_fall_int = 5U,
} emGPIO_INTERRUPT_MODE;

typedef enum {
    gpio_digital_input = 0U,
    gpio_digital_output = 1U,
} emGPIO_PIN_DIRECTION;

typedef enum {
    gpio_clr = 0U,
    gpio_set = 1U,
} emGPIO_OUTPUT;

typedef struct {
    emGPIO_PIN_DIRECTION dir;
    uint8_t output_logic;
    emGPIO_INTERRUPT_MODE int_mode;
} stPIN_CFG;


extern int8_t bsp_gpio_init(GPIO_Type *gpio, uint32_t pin, stPIN_CFG *cfg);
extern void bsp_gpio_int_enable(GPIO_Type *gpio, uint32_t pin);



#ifdef __cplusplus
}
#endif

#endif //__USR_GPIO_H






