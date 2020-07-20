#include "usr_app.h"

extern stLCD_DEV_INFO lcd_dev;

inline void usr_lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
	*(uint32_t *)(lcd_dev.frambuffer + lcd_dev.pix_size * (lcd_dev.width * y + x)) = color;
}

inline uint32_t usr_lcd_read_point(uint16_t x, uint16_t y)
{
	return *(uint32_t *)(lcd_dev.frambuffer + lcd_dev.pix_size * (lcd_dev.width * y + x));
}

void usr_lcd_clear(uint32_t color)
{
	uint32_t num;
	uint32_t i = 0;
	uint32_t *start_addr = (uint32_t *)lcd_dev.frambuffer;
	num = (uint32_t)(lcd_dev.width * lcd_dev.height);
	for(i = 0; i < num; i ++){
		start_addr[ i ] = color;
	}
}

/*
 * @description		: 以指定的颜色填充一块矩形
 * @param - x0		: 矩形起始点坐标X轴
 * @param - y0		: 矩形起始点坐标Y轴
 * @param - x1		: 矩形终止点坐标X轴
 * @param - y1		: 矩形终止点坐标Y轴
 * @param - color	: 要填充的颜色
 * @return 			:
 */
void usr_lcd_fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
{
    uint16_t x, y;

	if(x0 < 0) x0 = 0;
	if(y0 < 0) y0 = 0;
	if(x1 >= lcd_dev.width) x1 = lcd_dev.width - 1;
	if(y1 >= lcd_dev.height) y1 = lcd_dev.height - 1;

    for(y = y0; y <= y1; y++){
        for(x = x0; x <= x1; x++){
			usr_lcd_draw_point(x, y, color);
		}
    }
}


#include "font.h"

/*
 * @description		: 画线函数
 * @param - x1		: 线起始点坐标X轴
 * @param - y1		: 线起始点坐标Y轴
 * @param - x2		: 线终止点坐标X轴
 * @param - y2		: 线终止点坐标Y轴
 * @return 			: 无
 */
void usr_lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t;
	int32_t xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int32_t incx, incy, uRow, uCol;
	delta_x = x2 - x1; 					/* 计算坐标增量 */
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if(delta_x > 0) 					/* 设置单步方向 */
		incx = 1;
	else if(delta_x==0)					/* 垂直线 */
		incx = 0;
	else{
		incx = -1;
		delta_x = -delta_x;
	}
	if(delta_y>0)
		incy=1;
	else if(delta_y == 0)				/* 水平线 */
		incy=0;
	else{
		incy = -1;
		delta_y = -delta_y;
	}
	if( delta_x > delta_y)				/*选取基本增量坐标轴  */
		distance = delta_x;
	else
		distance = delta_y;
	for(t = 0; t <= distance+1; t++ ){	/* 画线输出 */
		usr_lcd_draw_point(uRow, uCol, lcd_dev.forecolor);/* 画点 */
		xerr += delta_x ;
		yerr += delta_y ;
		if(xerr > distance){
			xerr -= distance;
			uRow += incx;
		}
		if(yerr > distance){
			yerr -= distance;
			uCol += incy;
		}
	}
}

/*
 * @description	: 画矩形函数
 * @param - x1	: 矩形坐上角坐标X轴
 * @param - y1	: 矩形坐上角坐标Y轴
 * @param - x2	: 矩形右下角坐标X轴
 * @param - y2	: 矩形右下角坐标Y轴
 * @return 		: 无
 */
void usr_lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	usr_lcd_draw_line(x1, y1, x2, y1);
	usr_lcd_draw_line(x1, y1, x1, y2);
	usr_lcd_draw_line(x1, y2, x2, y2);
	usr_lcd_draw_line(x2, y1, x2, y2);
}

/*
 * @description	: 在指定位置画一个指定大小的圆
 * @param - x0	: 圆心坐标X轴
 * @param - y0	: 圆心坐标Y轴
 * @param - y2	: 圆形半径
 * @return 		: 无
 */
void usr_lcd_draw_circle(uint16_t x0, uint16_t y0, uint16_t r)
{
    int32_t mx = x0, my = y0;
    int32_t x = 0, y = r;

    int32_t d = 1 - r;
    while(y > x) {   	// y>x即第一象限的第1区八分圆
        usr_lcd_draw_point(x + mx, y + my, lcd_dev.forecolor);
        usr_lcd_draw_point(y + mx, x + my, lcd_dev.forecolor);
        usr_lcd_draw_point(-x + mx, y + my, lcd_dev.forecolor);
        usr_lcd_draw_point(-y + mx, x + my, lcd_dev.forecolor);

        usr_lcd_draw_point(-x + mx, -y + my, lcd_dev.forecolor);
        usr_lcd_draw_point(-y + mx, -x + my, lcd_dev.forecolor);
        usr_lcd_draw_point(x + mx, -y + my, lcd_dev.forecolor);
        usr_lcd_draw_point(y + mx, -x + my, lcd_dev.forecolor);
        if( d < 0){
            d = d + 2 * x + 3;
        }else{
            d= d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

/*
 * @description	: 在指定位置显示一个字符
 * @param - x	: 起始坐标X轴
 * @param - y	: 起始坐标Y轴
 * @param - num	: 显示字符
 * @param - size: 字体大小, 可选12/16/24/32
 * @param - mode: 叠加方式(1)还是非叠加方式(0)
 * @return 		: 无
 */
 void usr_lcd_show_char(uint16_t x, uint16_t y, uint16_t num, uint16_t size, uint16_t mode)
{
    uint8_t temp, t1, t;
	uint16_t y0 = y;
	uint8_t csize = (size / 8+ ((size % 8) ? 1 : 0)) * (size / 2);//得到字体一个字符对应点阵集所占的字节数
 	num = num - ' ';  //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t = 0; t < csize; t++){
		if(size == 12) temp = asc2_1206[num][t]; 		//调用1206字体
		else if(size == 16)temp = asc2_1608[num][t];	//调用1608字体
		else if(size == 24)temp = asc2_2412[num][t];	//调用2412字体
		else if(size == 32)temp = asc2_3216[num][t];	//调用3216字体
		else return;									//没有的字库
		for(t1 = 0; t1 < 8; t1++){
			if(temp & 0x80) usr_lcd_draw_point(x, y, lcd_dev.forecolor);
			else if(mode==0) usr_lcd_draw_point(x, y, lcd_dev.backcolor);
			temp <<= 1;
			y++;
			if(y >= lcd_dev.height) return;//超区域了
			if((y - y0) == size){
				y = y0;
				x++;
				if(x >= lcd_dev.width) return;//超区域了
				break;
			}
		}
	}
}

/*
 * @description	: 计算m的n次方
 * @param - m	: 要计算的值
 * @param - n	: n次方
 * @return 		: m^n次方.
 */
uint32_t usr_lcd_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while(n--) result *= m;
	return result;
}

/*
 * @description	: 显示指定的数字，高位为0的话不显示
 * @param - x	: 起始坐标点X轴。
 * @param - y	: 起始坐标点Y轴。
 * @param - num	: 数值(0~999999999)。
 * @param - len : 数字位数。
 * @param - size: 字体大小
 * @return 		: 无
 */
void use_lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for(t = 0; t < len; t++){
		temp = (num / usr_lcd_pow(10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1)){
			if(temp == 0){
				usr_lcd_show_char(x + (size / 2) * t, y, ' ', size, 0);
				continue;
			}else enshow = 1;
		}
	 	usr_lcd_show_char(x + (size / 2) * t, y, temp + '0', size, 0);
	}
}

/*
 * @description		: 显示指定的数字，高位为0,还是显示
 * @param - x	 	: 起始坐标点X轴。
 * @param - y	 	: 起始坐标点Y轴。
 * @param - num		: 数值(0~999999999)。
 * @param - len 	: 数字位数。
 * @param - size	: 字体大小
 * @param - mode	: [7]:0,不填充;1,填充0.
 * 					  [6:1]:保留
 *					  [0]:0,非叠加显示;1,叠加显示.
 * @return 	 		: 无
 */
void usr_lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for(t = 0; t < len; t++){
		temp = (num / usr_lcd_pow(10, len - t- 1)) % 10;
		if(enshow == 0 && t < (len - 1)){
			if(temp == 0){
				if(mode & 0X80) usr_lcd_show_char(x + (size / 2) * t, y, '0', size, mode & 0X01);
				else  usr_lcd_show_char(x + (size / 2) * t, y , ' ', size, mode & 0X01);
 				continue;
			}else enshow=1;

		}
	 	usr_lcd_show_char( x + (size / 2) * t, y, temp + '0' , size , mode & 0X01);
	}
}

/*
 * @description		: 显示一串字符串
 * @param - x		: 起始坐标点X轴。
 * @param - y		: 起始坐标点Y轴。
 * @param - width 	: 字符串显示区域长度
 * @param - height	: 字符串显示区域高度
 * @param - size	: 字体大小
 * @param - p		: 要显示的字符串首地址
 * @return 			: 无
 */
void usr_lcd_show_string(uint16_t x, uint16_t y, uint16_t width,uint16_t height, uint8_t size,char *p)
{
	uint8_t x0 = x;
	width += x;
	height += y;
    while((*p <= '~') &&(*p >= ' ')){//判断是不是非法字符!

        if(x >= width) {x = x0; y += size;}
        if(y >= height) break;//退出
        usr_lcd_show_char(x, y, *p , size, 0);
        x += size / 2;
        p++;
    }
}












