#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

#define RTC_LOG            PRINTF

#define kSECONDS_A_DAY		(86400)
#define kSECONDS_A_HOUR		(3600)
#define kSECONDS_A_MINUTE	(60)
#define kDAYS_A_YEAR			(365)
#define kYEAR_RANGE_START 	(1970)
#define kYEAR_RANGE_END		(2099)

typedef struct{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}stRTC_DATETIME;

extern int8_t bsp_rtc_init(void);
extern void bsp_rtc_en(FunctionalState NewState);
extern uint32_t bsp_rtc_get_seconds(void);
extern void bsp_rtc_get_datetime(stRTC_DATETIME *datetime);

#ifdef __cplusplus
}
#endif

#endif




