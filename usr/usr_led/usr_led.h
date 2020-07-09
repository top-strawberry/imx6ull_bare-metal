#ifndef __USR_LED_H
#define __USR_LED_H

#define LED_ON()    GPIO1->DR &= ~(1 << 3)    // bit3清零 
#define LED_OFF()   GPIO1->DR |= (1 << 3)     // bit3置1

extern void usr_led_init(void);
extern void usr_led_set(uint8_t state);
extern void usr_led_trigger(uint32_t ms);

#endif