/*
 * pal_rtc.h
 *
 *  Created on: 2013-9-4
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_RTC_H_
#define PAL_RTC_H_

#define PAL_RTC_FLAG			0x32F2

typedef struct{
	RTC_InitTypeDef RTC_InitStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
}PAL_RTC_CFG;

typedef struct{
	uint8_t			u8WeekDay;
	uint8_t			u8Month;
	uint8_t			u8Date;
	uint8_t			u8Year;

	uint8_t			u8Hours;
	uint8_t			u8Minutes;
	uint8_t			u8Seconds;
	uint8_t			u8AMPM;
}PAL_RTC_DATE_TIME;

ErrorStatus pal_rtc_init(const PAL_RTC_CFG *pstConfig);
ErrorStatus pal_rtc_GetDateTime(PAL_RTC_DATE_TIME *pstDateTime);
ErrorStatus pal_rtc_SetDateTime(PAL_RTC_DATE_TIME *pstDateTime);

#endif /* PAL_RTC_H_ */
