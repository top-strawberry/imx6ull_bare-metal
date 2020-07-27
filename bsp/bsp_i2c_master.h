#ifndef __BSP_I2C_MASTER_H
#define __BSP_I2C_MASTER_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

#define I2C_LOG            PRINTF

typedef enum 
{
	em_i2c_ok = 0,
	em_i2c_busy,
	em_i2c_idle,
	em_i2c_nack,
	em_i2c_arbitrationost, //仲裁丢失错误
	em_i2c_timeout,
	em_i2c_addr_nack,
} emI2C_STATUS;

typedef enum 
{
	em_i2c_write = 0,   //主机向从机发数据
	em_i2c_read = 1,    //主机从从机读数据
} emI2C_TRANSFER_DIRE;

typedef struct 
{
	uint8_t slaveAddress;			//7位从机地址
	emI2C_TRANSFER_DIRE direction;	//传输方向
	uint32_t subaddress;			//寄存器地址
	uint8_t subaddressSize;			//寄存器地址长度
	uint8_t *volatile data;			//数据缓冲区
	volatile uint32_t dataSize;		//数据缓冲区长度
} stI2C_TRANSFER;


typedef enum
{
	emI2C_RXAK = 0,
	emI2C_IIF = 1,
	emI2C_SRW = 2,
	emI2C_IAL = 4,
	emI2C_IBB = 5,
	emI2C_IAAS = 6,
	emI2C_ICF = 7,
} emI2SR_REG_BIT;

typedef enum
{
	emI2C_RSTA = 2,
	emI2C_TXAK = 3,
	emI2C_MTX = 4,
	emI2C_MSTA = 5,
	emI2C_IIEN = 6,
	emI2C_IEN = 7,
} emI2CR_REG_BIT;



extern int8_t bsp_i2c_master_init(I2C_Type *i2c);
extern emI2C_STATUS bsp_i2c_master_transfer(I2C_Type * i2c, stI2C_TRANSFER *xfer);
#ifdef __cplusplus
}
#endif

#endif




