//
// Created by Administrator on 2020/7/12 0012.
//
#include "usr_app.h"




void bsp_gpio_pin_write(GPIO_Type *gpio, uint32_t pin, uint8_t level)
{
    if(level == 0){
        gpio->DR &= ~(1 << pin);
    }else if(level == 1){
        gpio->DR |= (1 << pin);
    } else{
        GPIO_LOG("ERROR:usr_gpio_pin_write level != 0/1 \r\n");
    }
}

uint8_t bsp_gpio_pin_read(GPIO_Type *gpio, uint32_t pin)
{
    return ((gpio->DR >> pin) & 0x01);
}

void bsp_gpio_int_enable(GPIO_Type *gpio, uint32_t pin)
{
    gpio->IMR |= 1 << pin;
}
void bsp_gpio_int_disable(GPIO_Type *gpio, uint32_t pin)
{
    gpio->IMR &= ~(1 << pin);
}

void bsp_gpio_clear_int_flag(GPIO_Type *gpio, uint32_t pin)
{
    gpio->ISR |= 1 << pin;
}

int8_t bsp_gpio_int_cfg(GPIO_Type *gpio, uint32_t pin, emGPIO_INTERRUPT_MODE int_mode)
{
    volatile uint32_t *icr = NULL;
    uint32_t  icr_shift = pin;

    gpio->EDGE_SEL &= ~(1 << pin);
    if(pin < 16){
        icr = &gpio->ICR1;
    } else if(pin < 32){
        icr = &gpio->ICR2;
        icr_shift -= 16;
    } else{
        GPIO_LOG("ERROR:usr_gpio_pin_write pin >= 32 \r\n");
        return -1;
    }

    switch (int_mode) {
        case gpio_low_int:
            *icr &= ~(3 << (2 * icr_shift));
            break;
        case gpio_high_int:
            *icr &= ~(3 << (2 * icr_shift));
            *icr |=  (1 << (2 * icr_shift));
            break;
        case gpio_rise_int:
            *icr &= ~(3 << (2 * icr_shift));
            *icr |=  (2 << (2 * icr_shift));
            break;
        case gpio_fall_int:
            *icr &= ~(3 << (2 * icr_shift));
            *icr |=  (3 << (2 * icr_shift));
            break;
        case gpio_rise_fall_int:
            gpio->EDGE_SEL |= 1 << pin;
            break;
        default:
            break;
    }

    return 0;
}




int8_t bsp_gpio_init(GPIO_Type *gpio, uint32_t pin, stPIN_CFG *cfg)
{
    if(cfg->dir == gpio_digital_input){
        gpio->GDIR &= ~(1 << pin);
    }else if(cfg->dir == gpio_digital_output){
        gpio->GDIR |= (1 << pin);
        bsp_gpio_pin_write(gpio, pin, cfg->output_logic);
    }

    if(cfg->int_mode){
        if(bsp_gpio_int_cfg(gpio, pin, cfg->int_mode) != 0){
            GPIO_LOG("ERROR:usr_gpio_int_cfg return != 0 \r\n");
            return -1;
        }
    }

    return 0;
}





