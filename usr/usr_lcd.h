#ifndef __USR_LCD_H
#define __USR_LCD_H

#include "main.h"


#ifdef __cplusplus
extern "C"{
#endif

/* 颜色 */
#define LCD_BLUE		  	0x000000FF
#define LCD_GREEN		  	0x0000FF00
#define LCD_RED 		  	0x00FF0000
#define LCD_CYAN		  	0x0000FFFF
#define LCD_MAGENTA 	  	0x00FF00FF
#define LCD_YELLOW		  	0x00FFFF00
#define LCD_LIGHTBLUE	  	0x008080FF
#define LCD_LIGHTGREEN	  	0x0080FF80
#define LCD_LIGHTRED	  	0x00FF8080
#define LCD_LIGHTCYAN	  	0x0080FFFF
#define LCD_LIGHTMAGENTA  	0x00FF80FF
#define LCD_LIGHTYELLOW   	0x00FFFF80
#define LCD_DARKBLUE	  	0x00000080
#define LCD_DARKGREEN	  	0x00008000
#define LCD_DARKRED 	  	0x00800000
#define LCD_DARKCYAN	  	0x00008080
#define LCD_DARKMAGENTA   	0x00800080
#define LCD_DARKYELLOW	  	0x00808000
#define LCD_WHITE		  	0x00FFFFFF
#define LCD_LIGHTGRAY	  	0x00D3D3D3
#define LCD_GRAY		  	0x00808080
#define LCD_DARKGRAY	  	0x00404040
#define LCD_BLACK		  	0x00000000
#define LCD_BROWN		  	0x00A52A2A
#define LCD_ORANGE		  	0x00FFA500
#define LCD_TRANSPARENT   	0x00000000



extern inline void usr_lcd_draw_point(uint16_t x, uint16_t y, uint32_t color);
extern inline uint32_t usr_lcd_read_point(uint16_t x, uint16_t y);
extern void usr_lcd_clear(uint32_t color);
extern void usr_lcd_fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
extern void usr_lcd_draw_circle(uint16_t x0, uint16_t y0, uint16_t r);
extern void usr_lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern void usr_lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern void usr_lcd_show_char(uint16_t x, uint16_t y, uint16_t num, uint16_t size, uint16_t mode);
extern void use_lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);
extern void usr_lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);
extern void usr_lcd_show_string(uint16_t x, uint16_t y, uint16_t width,uint16_t height, uint8_t size,char *p);





#ifdef __cplusplus
}
#endif

#endif



