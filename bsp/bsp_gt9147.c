#include "usr_app.h"

stGT9147_DEV gt9147_dev;

static void bsp_gt9147_up_init(void)
{
	bsp_gpio_pin_write(GPIO5, kGPIO5_IO09_GT9147_RST_PIN, 0); //复位
	usr_delay_ms(15);
	bsp_gpio_pin_write(GPIO1, kGPIO1_IO09_GT9147_INT_PIN, 1);
	usr_delay_ms(200);
	bsp_gpio_pin_write(GPIO5, kGPIO5_IO09_GT9147_RST_PIN, 1); //停止复位 	
	usr_delay_ms(10);
	bsp_gpio_pin_write(GPIO1, kGPIO1_IO09_GT9147_INT_PIN, 0);
	usr_delay_ms(60);
}


uint8_t bsp_gt9147_read_byte(uint8_t addr, uint32_t reg)
{
    uint8_t val  = 0;    
    stI2C_TRANSFER masterXfer;

	memset(&masterXfer, 0, sizeof(stI2C_TRANSFER));
    masterXfer.slaveAddress = addr;
    masterXfer.direction = em_i2c_read;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 2;
    masterXfer.data = &val;
    masterXfer.dataSize = 1;
    if(bsp_i2c_master_transfer(I2C2, &masterXfer) != em_i2c_ok){
		GT9147_LOG("error:bsp_gt9147_read_byte\r\n");
		return -1;
	}

    return val;
}

void bsp_gt9147_read_bytes(uint8_t addr, uint32_t reg, uint8_t * buf, uint8_t len)
{
    stI2C_TRANSFER masterXfer;

	memset(&masterXfer, 0, sizeof(stI2C_TRANSFER));
    masterXfer.slaveAddress = addr;
    masterXfer.direction = em_i2c_read;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 2;
    masterXfer.data = buf;
    masterXfer.dataSize = len;
    if(bsp_i2c_master_transfer(I2C2, &masterXfer) != em_i2c_ok){
		GT9147_LOG("error:bsp_gt9147_read_bytes\r\n");
	}
}

int8_t bsp_gt9147_write(uint8_t addr, uint32_t reg, const uint8_t *data, uint8_t data_len)
{
    uint8_t status = 0;
    stI2C_TRANSFER masterXfer;

	if(data == NULL){
		GT9147_LOG("error: bsp_gt9147_write data is null\r\n");
		return -1;
	}

    masterXfer.slaveAddress = addr;
    masterXfer.direction = em_i2c_write;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 2;
    masterXfer.data = data;
    masterXfer.dataSize = data_len;
	if(bsp_i2c_master_transfer(I2C2, &masterXfer) != em_i2c_ok){
		GT9147_LOG("error:bsp_gt9147_write\r\n");
		status = -1;
	}

    return status;
}


const uint16_t GT9147_TPX_TBL[5]={kGT9147_TP1_REG, kGT9147_TP2_REG, kGT9147_TP3_REG,kGT9147_TP4_REG,kGT9147_TP5_REG};
static void bsp_gt9147_gpio01_io09_irq(IRQn_Type irq_num, void *arg)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t mode = 0;
	uint8_t buf[4] = {0};	

	if(gt9147_dev.init_flag == emGT9147_INIT_FINISHED) {
		//GT9147_LOG("bsp_gt9147_gpio01_io09_irq \r\n");
		bsp_gt9147_read_bytes(kGT9147_ADDR, kGT9147_GSTID_REG, &mode, 1);	//读取触摸点的状态
		bsp_gt9147_write(kGT9147_ADDR, kGT9147_GSTID_REG, &temp, 1);//清标志	
		gt9147_dev.point_num = mode & 0x0f;//触摸点个数	
		if(gt9147_dev.point_num){			
			for(i = 0; i < gt9147_dev.point_num; i ++){
				bsp_gt9147_read_bytes(kGT9147_ADDR, GT9147_TPX_TBL[i], buf, 4);//读取对应按下触摸点的坐标
				gt9147_dev.x[i] = (((uint16_t)buf[1] << 8) + buf[0]);
				gt9147_dev.y[i] = (((uint16_t)buf[3] << 8) + buf[2]);			
				if((mode == 0x81)||(mode == 0x82)||(mode == 0x83)||(mode == 0x84)||(mode == 0x85)){
					/*if((tp_dev.tmp_x == tp_dev.x_val)&&(tp_dev.tmp_y == tp_dev.y_val)){//重复按下某个坐标
					}else{
						tp_dev.tmp_x = tp_dev.x_val;
						tp_dev.tmp_y = tp_dev.y_val;
						printk("Down..\n");
					}*/			
				}
			}
		}
	}

    //清除中断表示位 
    bsp_gpio_clear_int_flag(GPIO1, kGPIO1_IO09_GT9147_INT_PIN);
}


//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
static const uint8_t GT9147_CFG_TBL1[]=
{ 
	0X61,						//配置文件的版本号
	0XE0,0X01,					//X 坐标输出最大值480
	0X10,0X01,					//Y 坐标输出最大值272
	0X05,						//输出触点个数上限:1~5
	0X35,						//有中断触发方式
	0X00,
	0X05,						//手指按下或松开的去抖次数
	0X08,
	0X1E,						//大面积触摸点个数
	0X08,		
	0X50,						//屏上触摸点从无到有的阈值
	0X3C,						//屏上触摸点从有到无的阈值			
	0X0F,
	0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
};

//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
static const uint8_t GT9147_CFG_TBL2[]=
{
	0x82,0xe0,0x01,0x10,0x01,0x05,0x0d,0x00,0x01,0x08,
	0x28,0x05,0x50,0x32,0x03,0x05,0x00,0x00,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x89,0x28,0x0a,
	0x17,0x15,0x31,0x0d,0x00,0x00,0x02,0x9b,0x03,0x25,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
	0x00,0x0f,0x94,0x94,0xc5,0x02,0x07,0x00,0x00,0x04,
	0x8d,0x13,0x00,0x5c,0x1e,0x00,0x3c,0x30,0x00,0x29,
	0x4c,0x00,0x1e,0x78,0x00,0x1e,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,
	0x18,0x1a,0x00,0x00,0x00,0x00,0x1f,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0x00,0x02,0x04,0x05,0x06,0x08,0x0a,0x0c,
	0x0e,0x1d,0x1e,0x1f,0x20,0x22,0x24,0x28,0x29,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
};

//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
int8_t bsp_gt9147_send_cfg(const uint8_t * cfg_tab, uint8_t cfg_tab_len)
{	
	uint8_t i = 0;
	uint8_t buf[2] = {0};
	uint8_t crc = 0;
	uint8_t mode = 0;
	
	if(cfg_tab[0] != gt9147_dev.cfg_v){
		mode = 1;//是否写入到GT9147 FLASH?	即是否掉电保存
		for(i = 0; i < cfg_tab_len; i ++){
			crc += cfg_tab[i];//计算校验和
		}

		crc = (~crc) + 1;		
		bsp_gt9147_write(kGT9147_ADDR, kGT9147_CFGS_REG, cfg_tab, cfg_tab_len);//发送寄存器配置
		buf[0] = crc;
		buf[1] = mode;
		bsp_gt9147_write(kGT9147_ADDR, kGT9147_CHECK_REG, buf, 2);//写入校验和,和配置更新标记
		GT9147_LOG("配置文件版本号:%02x \r\n", bsp_gt9147_read_byte(kGT9147_ADDR, kGT9147_CFGS_REG)); 
	}else{//不需要配置
		GT9147_LOG("debug: bsp_gt9147_send_cfg do nothing\r\n"); 
	}

	GT9147_LOG("中断方式:%02x \r\n", bsp_gt9147_read_byte(kGT9147_ADDR, 0x804D));

	return 0;
}


int8_t bsp_gt9147_init(void)
{
	uint8_t temp = 0;
	uint8_t mode = 0;
	stPIN_CFG gt9147_cfg;
	
	memset(&gt9147_dev, 0, sizeof(stGT9147_DEV));
	//i2c 初始化
	bsp_i2c_master_init(I2C2);	

	//引脚初始化
	/*	I2C2_SDA: IOMUXC_SW_MUX_CTL_PAD_UART5_RX_DATA
	 *	I2C2_SCL: IOMUXC_SW_MUX_CTL_PAD_UART5_TX_DATA
	 *	CT_INT:   GPIO1_IO09
	 *	CT_RST:   SNVS_TAMPER9
	 */
	IOMUXC_SetPinMux(IOMUXC_UART5_TX_DATA_I2C2_SCL, 1); //复用为I2C2_SCL
	IOMUXC_SetPinConfig(IOMUXC_UART5_TX_DATA_I2C2_SCL, 0X70b0);
	IOMUXC_SetPinMux(IOMUXC_UART5_RX_DATA_I2C2_SDA, 1); //复用为I2C2_SDA 	
	IOMUXC_SetPinConfig(IOMUXC_UART5_RX_DATA_I2C2_SDA, 0X70b0);

	gt9147_cfg.dire = gpio_digital_output;
	gt9147_cfg.output_logic = 0;

	//初始化INT引脚
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09,0X10b0);	
	bsp_gpio_init(GPIO1, kGPIO1_IO09_GT9147_INT_PIN, &gt9147_cfg);

	//初始化RST引脚 
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0); //复用为GPIO5_IO09
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0X10b0);
	bsp_gpio_init(GPIO5, kGPIO5_IO09_GT9147_RST_PIN, &gt9147_cfg);

	//gt9147上电初始化
	bsp_gt9147_up_init();
	// bsp_gt9147_read_bytes(kGT9147_ADDR, kGT9147_GSTID_REG,&mode,1);	//读取触摸点的状态
	// bsp_gt9147_write(kGT9147_ADDR, kGT9147_GSTID_REG, &temp, 1);//清标志
	gt9147_dev.cfg_v = bsp_gt9147_read_byte(kGT9147_ADDR, kGT9147_CFGS_REG);
	GT9147_LOG("配置文件版本号:0x%02x \r\n", gt9147_dev.cfg_v); 
	usr_delay_ms(10);
	bsp_gt9147_read_bytes(kGT9147_ADDR, kGT9147_PID_REG, gt9147_dev.id, sizeof(gt9147_dev.id) - 1);
	GT9147_LOG("产品 ID:%s \r\n", gt9147_dev.id);	
	if(strcmp((char*)gt9147_dev.id, kGT9147_ID) == 0){
		uint8_t temp[2] = {0x00, 0x02};			
		bsp_gt9147_write(kGT9147_ADDR, kGT9147_CTRL_REG, &temp[1], 1);
		usr_delay_ms(10);
		bsp_gt9147_send_cfg(GT9147_CFG_TBL2, sizeof(GT9147_CFG_TBL2));
		usr_delay_ms(10);
		bsp_gt9147_write(kGT9147_ADDR, kGT9147_CTRL_REG, &temp[0], 1);
		usr_delay_ms(10);
	}else{
		GT9147_LOG("error bsp_gt9147_init\r\n");	
		return -1;
	}

	//初始化INT引脚，使能中断 
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0); //复用为GPIO1_IO09
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09 , 0x0088);	
	gt9147_cfg.dire = gpio_digital_input;
	gt9147_cfg.int_mode = gpio_fall_int;
	bsp_gpio_init(GPIO1, kGPIO1_IO09_GT9147_INT_PIN, &gt9147_cfg);
	GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
	bsp_int_system_regiser_irq_handler(GPIO1_Combined_0_15_IRQn , bsp_gt9147_gpio01_io09_irq, NULL);
	bsp_gpio_int_enable(GPIO1, kGPIO1_IO09_GT9147_INT_PIN);
	usr_delay_ms(100);
	gt9147_dev.init_flag = emGT9147_INIT_FINISHED;	//标记GT9147初始化完成

	return 0;
}





