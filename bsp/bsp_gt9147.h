#ifndef __BSP_GT9147_H
#define __BSP_GT9147_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

#define GT9147_LOG                  PRINTF
#define kGT9147_ADDR			    0X14	//GT9147 设备地址
#define kGT9147_ID                  "9147"
#define kGPIO1_IO09_GT9147_INT_PIN  (9)
#define kGPIO5_IO09_GT9147_RST_PIN  (9)

//GT9147 部分寄存器定义 
#define kGT9147_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define kGT9147_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define kGT9147_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define kGT9147_PID_REG 	0X8140   	//GT9147产品ID寄存器

#define kGT9147_GSTID_REG   0X814E   	//GT9147当前检测到的触摸情况
#define kGT9147_TP1_REG 	0X8150  	//第一个触摸点数据地址
#define kGT9147_TP2_REG 	0X8158		//第二个触摸点数据地址
#define kGT9147_TP3_REG 	0X8160		//第三个触摸点数据地址
#define kGT9147_TP4_REG 	0X8168		//第四个触摸点数据地址
#define kGT9147_TP5_REG 	0X8170		//第五个触摸点数据地址 

#define kGT9147_XYCOORDREG_NUM		30		//触摸点坐标寄存器数量


typedef enum
{
	emGT9147_INIT_NOTFINISHED 	= 0,		//触摸屏初始化未完成
	emGT9147_INIT_FINISHED		= 1,		//触摸屏初始化完成	
}emGT9147_INIT_STA;

typedef enum
{
	emGT9147_TOUCH_EVENT_DOWN		=		0x00,	//按下
	emGT9147_TOUCH_EVENT_UP			=		0x01,	//释放
	emGT9147_TOUCH_EVENT_ON			=		0x02,	//接触
	emGT9147_TOUCH_EVENT_RESERVED	=		0x03,	//没有事件	
}emGT9147_TOUCH_EVENT;


//触摸信息结构体
typedef struct {
	uint8_t cfg_v;
	uint8_t id[5];
	uint8_t sta;
	uint8_t init_flag;	//触摸屏初始化状态
	uint8_t int_flag;	//标记中断有没有发生
	uint8_t point_num;	//触摸点数量
	uint16_t x[5];		//5点X轴坐标
	uint16_t y[5];		//5点Y轴坐标	
} stGT9147_DEV;

extern stGT9147_DEV gt9147_dev;
extern int8_t bsp_gt9147_init(void);

#ifdef __cplusplus
}
#endif

#endif




