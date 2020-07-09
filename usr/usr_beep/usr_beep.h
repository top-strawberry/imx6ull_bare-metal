#ifndef __USR_BEEP_H
#define __USR_BEEP_H

#define BEEP_ON()    GPIO5->DR &= ~(1 << 1)    // bit1清零 
#define BEEP_OFF()   GPIO5->DR |= (1 << 1)     // bit1置1

#endif