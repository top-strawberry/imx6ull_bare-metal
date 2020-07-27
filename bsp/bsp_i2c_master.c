#include "usr_app.h"

emI2C_STATUS bsp_i2c_master_start(I2C_Type *i2c, uint8_t addr, emI2C_TRANSFER_DIRE dire)
{
	if ((i2c->I2SR >> emI2C_IBB) & 0x1) {
		I2C_LOG("%s[%d] error:bsp_i2c_master_start i2c is busy!\r\n",__FILE__, __LINE__);
		return em_i2c_busy;
	}

	i2c->I2CR |= (1 << emI2C_MTX) | (1 << emI2C_MSTA); //主机发送模式
	i2c->I2DR = ((uint32_t)addr << 1) | ((dire == em_i2c_write) ? em_i2c_write : em_i2c_read);

	return em_i2c_ok;
}

emI2C_STATUS bsp_i2c_master_stop(I2C_Type *i2c)
{
	uint16_t timeout = 0x00ff;

	i2c->I2CR &= ~(7 << emI2C_TXAK); //
	while ((i2c->I2SR >> emI2C_IBB) & 0x1) {
		timeout--;
		if (timeout == 0) {
			I2C_LOG("%s[%d] error:bsp_i2c_master_stop i2c is timeout!\r\n",__FILE__, __LINE__);
			return em_i2c_timeout;
		}
	}

	return em_i2c_ok;
}

emI2C_STATUS bsp_i2c_master_repeat_start(I2C_Type *i2c, uint8_t addr, emI2C_TRANSFER_DIRE dire)
{
	if (((i2c->I2SR >> emI2C_IBB) & 0x1) && 
	   (((i2c->I2CR >> emI2C_MSTA) & 0x1) == 0)) {
		I2C_LOG("%s[%d] error:bsp_i2c_master_repeat_start i2c is busy!\r\n",__FILE__, __LINE__);
		return em_i2c_busy;
	}

	i2c->I2CR |= (1 << emI2C_SRW) | (1 << emI2C_IAL); //
	i2c->I2DR = ((uint32_t)addr << 1) | ((dire == em_i2c_write) ? em_i2c_write : em_i2c_read);

	return em_i2c_ok;
}

emI2C_STATUS bsp_i2c_master_check_and_clear_err(I2C_Type *i2c, uint32_t status)
{
	emI2C_STATUS i2c_status = em_i2c_ok;

	if ((status >> emI2C_IAL) & 0x1) {
		i2c->I2SR &= ~(1 << emI2C_IAL);
		i2c->I2CR &= ~(1 << emI2C_IEN);
		i2c->I2CR |= (1 << emI2C_IEN);
		i2c_status = em_i2c_arbitrationost;
		I2C_LOG("%s[%d] error:bsp_i2c_master_check_and_clear_err i2c is arbitrationost!\r\n",__FILE__, __LINE__);
	} else if ((status >> emI2C_RXAK) & 0x1) {
		i2c_status = em_i2c_nack;
		I2C_LOG("%s[%d] error:bsp_i2c_master_check_and_clear_err i2c is nack!\r\n",__FILE__, __LINE__);
	}

	return i2c_status;
}

emI2C_STATUS bsp_i2c_master_write(I2C_Type *i2c, const uint8_t *buf_t, uint32_t size)
{
	uint16_t timeout = 0x00ff;
	emI2C_STATUS i2c_status = em_i2c_ok;

	while (((i2c->I2SR >> emI2C_ICF) & 0x1) == 0) { //等待数据就绪
		// timeout--;
		// if (timeout == 0) {
		// 	I2C_LOG("%s[%d] error:bsp_i2c_master_write i2c is timeout!\r\n",__FILE__, __LINE__);
		// 	return em_i2c_timeout;
		// }
	}

	i2c->I2SR &= ~(1 << emI2C_IIF);
	i2c->I2CR |= (1 << emI2C_IEN);

	while (size--) {
		i2c->I2DR = *buf_t++;
		while (((i2c->I2SR >> emI2C_IIF) & 0x1) == 0) {
			// timeout--;
			// if (timeout == 0) {
			// 	I2C_LOG("%s[%d] error:bsp_i2c_master_write i2c is timeout!\r\n",__FILE__, __LINE__);
			// 	return em_i2c_timeout;
			// }
		}

		i2c->I2SR &= ~(1 << emI2C_IIF); //清除标志
		i2c_status = bsp_i2c_master_check_and_clear_err(i2c, i2c->I2SR);
		if (i2c_status != em_i2c_ok) {
			I2C_LOG("%s[%d] error:bsp_i2c_master_write i2c is %d!\r\n",__FILE__, __LINE__, (int8_t)i2c_status);
			return i2c_status;
		}
	}

	i2c->I2SR &= ~(1 << emI2C_IIF); //清除标志
	i2c_status = bsp_i2c_master_stop(i2c);

	return i2c_status;
}

emI2C_STATUS bsp_i2c_master_read(I2C_Type *i2c, uint8_t *buf_r, uint32_t size)
{
	uint16_t timeout = 0x00ff;
	volatile uint8_t dummy = 0;
	emI2C_STATUS i2c_status = em_i2c_ok;	

	dummy++;
	while (((i2c->I2SR >> emI2C_ICF) & 0x1) == 0) { //等待数据就绪
		// timeout--;
		// if (timeout == 0) {
		// 	I2C_LOG("%s[%d] error:bsp_i2c_master_read i2c is timeout!\r\n",__FILE__, __LINE__);
		// 	return em_i2c_timeout;
		// }
	}

	i2c->I2SR &= ~(1 << emI2C_IIF); //清除标志
	i2c->I2CR &= ~(3 << emI2C_TXAK);

	if (size == 1) {
		i2c->I2CR |= 1 << emI2C_TXAK;
	}

	dummy = i2c->I2DR;//假读 
	while (size--) {
		while (((i2c->I2SR >> emI2C_IIF) & 0x1) == 0) {
			// timeout--;
			// if (timeout == 0) {
			// 	I2C_LOG("%s[%d] error:bsp_i2c_master_read i2c is timeout!\r\n",__FILE__, __LINE__);
			// 	return em_i2c_timeout;
			// }
		}

		i2c->I2SR &= ~(1 << emI2C_IIF); //清除标志
		if (size == 1) {
			i2c->I2CR |= 1 << emI2C_TXAK; // nack
		} else if (size == 0) {
			i2c_status = bsp_i2c_master_stop(i2c);
		}
		*buf_r++ = i2c->I2DR;
	}

	return i2c_status;
}

/*
 * @description	: I2C数据传输，包括读和写
 * @param - i2c	: 要使用的IIC
 * @param - xfer: 传输结构体
 * @return 		: 传输结果,0 成功，其他值 失败;
 */
emI2C_STATUS bsp_i2c_master_transfer(I2C_Type * i2c, stI2C_TRANSFER *xfer)
{
	emI2C_STATUS i2c_status = em_i2c_ok;
	emI2C_TRANSFER_DIRE direction = xfer->direction;

	i2c->I2SR &= ~((1 << emI2C_IIF) | (1 << emI2C_IAL)); //清除标志位 
	while (((i2c->I2SR >> emI2C_ICF) & 0X1) == 0){//等待传输完成
		//I2C_LOG("%s[%d] debug:bsp_i2c_master_transfer i2c waitting!\r\n",__FILE__, __LINE__);
	}

	// 如果是读的话，要先发送寄存器地址，所以要先将方向改为写 
	if ((xfer->subaddressSize > 0) && (xfer->direction == em_i2c_read)) {
		direction = em_i2c_write;
	}

	i2c_status = bsp_i2c_master_start(i2c, xfer->slaveAddress, direction); //发送开始信号 
	if (i2c_status) {
		I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is busy!\r\n",__FILE__, __LINE__);
		return i2c_status;
	}

	while ((i2c->I2SR & (1 << emI2C_IIF)) == 0){ //等待传输完成 
		//I2C_LOG("%s[%d] debug:bsp_i2c_master_transfer i2c waitting!\r\n",__FILE__, __LINE__);
	}

	i2c_status = bsp_i2c_master_check_and_clear_err(i2c, i2c->I2SR); //检查是否出现传输错误 
	if (i2c_status) {
		I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is %d!!\r\n",__FILE__, __LINE__, (uint8_t)i2c_status);
		goto ERR;
	}

	//发送寄存器地址
	if (xfer->subaddressSize) {
		do {
			i2c->I2SR &= ~(1 << emI2C_IIF); //清除标志位 
			xfer->subaddressSize--;	 //地址长度减一

			i2c->I2DR = ((xfer->subaddress) >> (8 * xfer->subaddressSize)); //向I2DR寄存器写入子地址

			while ((i2c->I2SR & (1 << emI2C_IIF)) == 0){ //等待传输完成 
				//I2C_LOG("%s[%d] debug:bsp_i2c_master_transfer i2c waitting!\r\n",__FILE__, __LINE__);
			}

			//检查是否有错误发生
			i2c_status = bsp_i2c_master_check_and_clear_err(i2c, i2c->I2SR);
			if (i2c_status) {
				I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is %d!!\r\n",__FILE__, __LINE__, (uint8_t)i2c_status);
				goto ERR;
			}

		} while ((xfer->subaddressSize > 0) && (i2c_status == em_i2c_ok));

		if (xfer->direction == em_i2c_read){ //读取数据 		
			i2c->I2SR &= ~(1 << emI2C_IIF);
			i2c_status = bsp_i2c_master_repeat_start(i2c, xfer->slaveAddress, em_i2c_read); //发送重复开始信号和从机地址 
			if (i2c_status) {
				I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is %d!!\r\n",__FILE__, __LINE__, (uint8_t)i2c_status);
				goto ERR;
			}

			while ((i2c->I2SR & (1 << emI2C_IIF)) == 0) {//等待传输完成 
				//I2C_LOG("%s[%d] debug:bsp_i2c_master_transfer i2c waitting!\r\n",__FILE__, __LINE__);
			}

			// 检查是否有错误发生 
			i2c_status = bsp_i2c_master_check_and_clear_err(i2c, i2c->I2SR);
			if (i2c_status) {
				I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is %d!!\r\n",__FILE__, __LINE__, (uint8_t)i2c_status);
				goto ERR;
			}
		}
	}

	//发送数据 
	if ((xfer->direction == em_i2c_write) && (xfer->dataSize > 0)) {
		i2c_status = bsp_i2c_master_write(i2c, xfer->data, xfer->dataSize);
		if (i2c_status) {
			I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is %d!!\r\n",__FILE__, __LINE__, (uint8_t)i2c_status);
			goto ERR;
		}
	}

	//读取数据 
	if ((xfer->direction == em_i2c_read) && (xfer->dataSize > 0)) {
		i2c_status = bsp_i2c_master_read(i2c, xfer->data, xfer->dataSize);
		if (i2c_status) {
			I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is %d!!\r\n",__FILE__, __LINE__, (uint8_t)i2c_status);
			goto ERR;
		}
	}

	return i2c_status;

ERR:
	i2c_status = bsp_i2c_master_stop(i2c); //发送出错，发送停止信号
	I2C_LOG("%s[%d] error:bsp_i2c_master_transfer i2c is %d!!\r\n",__FILE__, __LINE__, (uint8_t)i2c_status);
	
	return i2c_status;
}

int8_t bsp_i2c_master_init(I2C_Type *i2c)
{
	i2c->I2CR &= ~(1 << emI2C_IEN); //关闭I2C
	i2c->IFDR = 0x15;//640 分频  66000000 / 640 = 103.125KHz
	i2c->I2CR |= (1 << emI2C_IEN);	//使能I2C

	return 0;
}
