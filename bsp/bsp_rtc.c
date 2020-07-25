#include "usr_app.h"


/*
 * @description	: 判断指定年份是否为闰年，闰年条件如下:
 * @param - year: 要判断的年份
 * @return 		: 1 是闰年，0 不是闰年
 */
uint8_t bsp_rtc_isleapyear(uint16_t year)
{	
	uint8_t value=0;
	
	if(year % 400 == 0){
		value = 1;
	} else {
		if((year % 4 == 0) && (year % 100 != 0)){
			value = 1;
		} else {
			value = 0;
		}			
	}

	return value;
}

/*
 * @description		: 将时间转换为秒数
 * @param - datetime: 要转换日期和时间。
 * @return 			: 转换后的秒数
 */
uint32_t bsp_rtc_coverdate_to_seconds(const stRTC_DATETIME *datetime)
{
	uint16_t i = 0;
	uint32_t seconds = 0;
	uint32_t days = 0;
	uint16_t monthdays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

	for (i = kYEAR_RANGE_START; i < datetime->year; i++)
	{
		days += kDAYS_A_YEAR; /* 平年，每年365天 */
		if (bsp_rtc_isleapyear(i))
			days += 1; /* 闰年多加一天 		*/
	}

	days += monthdays[datetime->month];
	if (bsp_rtc_isleapyear(i) && (datetime->month >= 3))
		days += 1; /* 闰年，并且当前月份大于等于3月的话加一天 */

	days += datetime->day - 1;

	seconds = days * kSECONDS_A_DAY +
			  datetime->hour * kSECONDS_A_HOUR +
			  datetime->minute * kSECONDS_A_MINUTE +
			  datetime->second;

	return seconds;
}

int8_t bsp_rtc_set_datetime(const stRTC_DATETIME *datetime)
{
	uint32_t tmp = SNVS->LPCR;
	uint32_t seconds = 0;

	bsp_rtc_en(disable);

	seconds = bsp_rtc_coverdate_to_seconds(datetime);
	//RTC_LOG("****************seconds = %ld\r\n", seconds);	
	SNVS->LPSRTCMR = (uint32_t)(seconds >> 17); //设置高16位
	SNVS->LPSRTCLR = (uint32_t)(seconds << 15); //设置地16位
	// seconds = bsp_rtc_get_seconds();	
	// RTC_LOG("----------------seconds = %ld\r\n", seconds);
	if (tmp & 0x01) {
		bsp_rtc_en(enable);
	}

	return 0;
}


/*
 * @description		: 将秒数转换为时间
 * @param - seconds	: 要转换的秒数
 * @param - datetime: 转换后的日期和时间
 * @return 			: 无
 */
void bsp_rtc_convertseconds_to_datetime(uint32_t seconds, stRTC_DATETIME *datetime)
{
    uint32_t x;
    uint32_t  secondsRemaining = 0, days = 0;
    uint16_t daysInYear = 0;

    /* 每个月的天数       */
    uint8_t daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    secondsRemaining = seconds; /* 剩余秒数初始化 */
    days = secondsRemaining / kSECONDS_A_DAY + 1; 		/* 根据秒数计算天数,加1是当前天数 */
    secondsRemaining = secondsRemaining % kSECONDS_A_DAY; /*计算天数以后剩余的秒数 */

	/* 计算时、分、秒 */
    datetime->hour = secondsRemaining / kSECONDS_A_HOUR;
    secondsRemaining = secondsRemaining % kSECONDS_A_HOUR;
    datetime->minute = secondsRemaining / kSECONDS_A_MINUTE;
    datetime->second = secondsRemaining % kSECONDS_A_MINUTE;

    /* 计算年 */
    daysInYear = kDAYS_A_YEAR;
    datetime->year = kYEAR_RANGE_START;
    while(days > daysInYear) {
        /* 根据天数计算年 */
        days -= daysInYear;
        datetime->year++;

        /* 处理闰年 */
        if (!bsp_rtc_isleapyear(datetime->year)){
			daysInYear = kDAYS_A_YEAR;
		}else{
			/*闰年，天数加一 */
            daysInYear = kDAYS_A_YEAR + 1;
		}	
    }
	/*根据剩余的天数计算月份 */
    if(bsp_rtc_isleapyear(datetime->year)){ /* 如果是闰年的话2月加一天 */
 		daysPerMonth[2] = 29;
	}       

    for(x = 1; x <= 12; x++) {
        if (days <= daysPerMonth[x]) {
            datetime->month = x;
            break;
        } else {
            days -= daysPerMonth[x];
        }
    }

    datetime->day = days;
}

uint32_t bsp_rtc_get_seconds(void)
{
	uint32_t seconds = 0;

	//seconds = ((uint64_t)((uint64_t)(SNVS->LPSRTCMR) << 32)) | (SNVS->LPSRTCLR);
	seconds = (SNVS->LPSRTCMR << 17) | (SNVS->LPSRTCLR >> 15);

	return seconds;
}

void bsp_rtc_get_datetime(stRTC_DATETIME *datetime)
{
    uint32_t seconds = 0;
    seconds = bsp_rtc_get_seconds();	
    bsp_rtc_convertseconds_to_datetime(seconds, datetime);
}



void bsp_rtc_en(FunctionalState NewState)
{
	if (NewState == enable){
		SNVS->LPCR |= 1 << 0;
		while ((SNVS->LPCR & 0x1) == 0);
	}
	else if (NewState == disable){
		SNVS->LPCR &= ~(1 << 0);
		while ((SNVS->LPCR & 0x1) == 1);
	}
}

int8_t bsp_rtc_init(void)
{
	stRTC_DATETIME rtc_date;

	SNVS->HPCOMR |= (1 << 8) | (1 << 31);
	bsp_rtc_en(enable);

	memset(&rtc_date, 0, sizeof(stRTC_DATETIME));
	rtc_date.year = 2020;
	rtc_date.month = 7;
	rtc_date.day = 25;
	rtc_date.hour = 8;
	rtc_date.minute = 0;
	rtc_date.second = 0;

	bsp_rtc_set_datetime(&rtc_date);

	return 0;
}
