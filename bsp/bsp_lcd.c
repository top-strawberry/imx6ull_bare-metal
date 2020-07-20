#include "usr_app.h"


stLCD_DEV_INFO lcd_dev;


static uint16_t bsp_lcd_read_id(void)
{
	uint8_t id = 0;
    stPIN_CFG lcd_cfg;

    // IOMUX_SW_MUX->GPIO1_IO03 = 0x5;    // 复用为GPIO1——IO03
    // IOMUX_SW_PAD->GPIO1_IO03 = 0X10B0; //设置GPIO1_IO03电气属性
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0x10b0);

    lcd_cfg.dir = gpio_digital_output;
    lcd_cfg.output_logic = gpio_set;
    lcd_cfg.int_mode = gpio_no_int;
    bsp_gpio_init(GPIO3, kGPIO3_IO03_LCD_VSYNC_PIN, &lcd_cfg);



	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0xF080);

	IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0xF080);

	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0xF080);

	lcd_cfg.dir = gpio_digital_input;
	bsp_gpio_init(GPIO3, kGPIO3_IO12_LCD_DATA07_PIN, &lcd_cfg);
	bsp_gpio_init(GPIO3, kGPIO3_IO20_LCD_DATA15_PIN, &lcd_cfg);
	bsp_gpio_init(GPIO3, kGPIO3_IO28_LCD_DATA23_PIN, &lcd_cfg);

	id = (uint8_t) bsp_gpio_pin_read (GPIO3, kGPIO3_IO28_LCD_DATA23_PIN);
	id |= (uint8_t) bsp_gpio_pin_read (GPIO3, kGPIO3_IO20_LCD_DATA15_PIN) << 1;
	id |= (uint8_t) bsp_gpio_pin_read (GPIO3, kGPIO3_IO12_LCD_DATA07_PIN) << 2;

	if(id == 0) return kLCD_4342;

	return 0;
}

static void bsp_lcd_io_init(void)
{
	stPIN_CFG bl_cfg;

	IOMUXC_SetPinMux (IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA00_LCDIF_DATA00, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA01_LCDIF_DATA01, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA02_LCDIF_DATA02, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA03_LCDIF_DATA03, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA04_LCDIF_DATA04, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA05_LCDIF_DATA05, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA06_LCDIF_DATA06, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA07_LCDIF_DATA07, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA08_LCDIF_DATA08, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA08_LCDIF_DATA08, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA09_LCDIF_DATA09, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA11_LCDIF_DATA11, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA12_LCDIF_DATA12, 0xb9);

	IOMUXC_SetPinMux (IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA13_LCDIF_DATA13, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA14_LCDIF_DATA14, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA15_LCDIF_DATA15, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA16_LCDIF_DATA16, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA16_LCDIF_DATA16, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA17_LCDIF_DATA17, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA18_LCDIF_DATA18, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA19_LCDIF_DATA19, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA20_LCDIF_DATA20, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA21_LCDIF_DATA21, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA22_LCDIF_DATA22, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_DATA23_LCDIF_DATA23, 0xb9);

	IOMUXC_SetPinMux (IOMUXC_LCD_CLK_LCDIF_CLK, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_CLK_LCDIF_CLK, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0xb9);
	IOMUXC_SetPinMux (IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);
	IOMUXC_SetPinConfig (IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0xb9);


	bl_cfg.dir = gpio_digital_output;
	bl_cfg.output_logic = gpio_set;
	bl_cfg.int_mode = gpio_no_int;
	IOMUXC_SetPinMux (IOMUXC_GPIO1_IO08_GPIO1_IO08, 0);
	IOMUXC_SetPinConfig (IOMUXC_GPIO1_IO08_GPIO1_IO08, 0x01b0);
	bsp_gpio_init (GPIO1, kGPIO1_IO08_LCD_BL_PIN, &bl_cfg);

	kLCD_BL_SET();

}

static void bsp_lcd_reset(uint32_t ms)
{
	kLCD_RESET_SET();
	usr_delay_ms (ms);
	kLCD_RESET_CLR ();
}


/**
  * @brief
  * @param	:
  *		loop_div:	设置DIV_SELECT，范围 27-54
  *		pre_div:
  * 	div；
  *		LCD_CLK = 24 * loop_div / (pre_div * div)
  * @retval
  */
static int8_t bsp_lcd_clk_init(uint8_t loop_div, uint8_t pre_div, uint8_t div)
{
	//不使用小数分频器
	CCM_ANALOG->PLL_VIDEO_NUM = 0;
	CCM_ANALOG->PLL_VIDEO_DENOM = 0;

	CCM_ANALOG->PLL_VIDEO = (1 << CCM_ENABLE) | (2 << CCM_POST_DIV_SELECT) |(loop_div << CCM_DIV_SELECT);
	CCM_ANALOG->MISC2 &= ~(3 << CCM_VIDEO_DIV);

	CCM->CSCDR2 &= ~(7 << CCM_LCDIF1_PRE_CLK_SEL);
	CCM->CSCDR2 |= (2 << CCM_LCDIF1_PRE_CLK_SEL);

	CCM->CSCDR2 &= ~(7 << CCM_LCDIF1_PRED);
	CCM->CSCDR2 |= ((pre_div - 1) << CCM_LCDIF1_PRED);

	CCM->CBCMR &= ~(7 << CCM_LCDIF1_PODF);
	CCM->CBCMR |= ((div - 1) << CCM_LCDIF1_PODF);

	CCM->CSCDR2    &= ~(7 << CCM_LCDIF1_CLK_SEL);
	CCM->CSCDR2    |= (0 << CCM_LCDIF1_CLK_SEL);

	return 0;
}

int8_t bsp_lcd_init()
{
	uint16_t lcd_id = 0 ;
	lcd_id = bsp_lcd_read_id ();
	LCD_LOG("lcd id is 0x%x!\r\n", lcd_id);
	bsp_lcd_io_init();
	bsp_lcd_reset(10);

	memset (&lcd_dev, 0, sizeof(stLCD_DEV_INFO));
	if(lcd_id == kLCD_4342){
		lcd_dev.height = 272;
		lcd_dev.width = 480;
		lcd_dev.hspw = 1;
		lcd_dev.hbpd = 40;
		lcd_dev.hfpd = 5;
		lcd_dev.vspw = 1;
		lcd_dev.vbpd = 8;
		lcd_dev.vfpd = 8;
		bsp_lcd_clk_init(27, 8, 8);
	}

	lcd_dev.pix_size = 4;
	lcd_dev.frambuffer = kLCD_FRAMBUFFER;
	lcd_dev.forecolor = LCD_WHITE;
	lcd_dev.backcolor = LCD_BLACK;

	//控制器接口配置
	LCDIF->CTRL = 0;
	LCDIF->CTRL = (1 << LCD_MASTER) | (3 << LCD_WORD_LENGTH) |
				   	(3 << LCD_LCD_DATABUS_WIDTH) | (1 << LCD_DOTCLK_MODE) |
				   	(1 << LCD_BYPASS_COUNT);
	LCDIF->CTRL1 = 0;
	LCDIF->CTRL1 = 7 << LCD_BYTE_PACKING_FORMAT;
	LCDIF->TRANSFER_COUNT = 0;
	LCDIF->TRANSFER_COUNT = (lcd_dev.height << LCD_V_COUNT) |lcd_dev.width;
	LCDIF->VDCTRL0 = 0;
	LCDIF->VDCTRL0 = (1 << LCD_VSYNC_PULSE_WIDTH_UNIT) |
					   (1 << LCD_VSYNC_PERIOD_UNIT) |
					   (1 << LCD_ENABLE_POL) |
					   (1 << LCD_ENABLE_PRESENT) |
					   (lcd_dev.vspw << LCD_VSYNC_PULSE_WIDTH);
	LCDIF->VDCTRL1 = lcd_dev.vspw + lcd_dev.vbpd  + lcd_dev.height + lcd_dev.vfpd;
	LCDIF->VDCTRL2 = (lcd_dev.hspw << LCD_HSYNC_PULSE_WIDTH) |(lcd_dev.hspw + lcd_dev.hbpd + lcd_dev.width + lcd_dev.vfpd) ;
	LCDIF->VDCTRL3 = ((lcd_dev.hspw + lcd_dev.hbpd) << LCD_HORIZONTAL_WAIT_CNT) |(lcd_dev.vspw + lcd_dev.vbpd);
	LCDIF->VDCTRL4 = (1 << LCD_SYNC_SIGNALS_ON) | lcd_dev.width;
	LCDIF->CUR_BUF = (uint32_t)lcd_dev.frambuffer;
	LCDIF->NEXT_BUF = (uint32_t)lcd_dev.frambuffer;

	kLCD_ENABLE();
	usr_delay_ms (20);
	usr_lcd_clear(LCD_RED);

	lcd_dev.forecolor = LCD_RED;
    lcd_dev.backcolor = LCD_WHITE;
    usr_lcd_show_string(10, 40, 260, 32, 32,(char*)"ALPAH IMX6U");
    usr_lcd_show_string(10, 80, 240, 24, 24,(char*)"RGBLCD TEST");
    usr_lcd_show_string(10, 110, 240, 16, 16,(char*)"ATOM&ALIENTEK");
    usr_lcd_show_string(10, 130, 240, 12, 12,(char*)"2020/7/22");
	usr_lcd_show_xnum(10, 28, 20200722, 8, 16, 0);
	return 0;
}


















