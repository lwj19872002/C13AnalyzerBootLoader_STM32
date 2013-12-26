/*
 * ctrl_systime.c
 *
 *  Created on: 2013-9-4
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_SYSTIME
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_systime_init(void)
{
	ErrorStatus eRet;
	PAL_RTC_DATE_TIME stRTCTime;

	eRet = pal_rtc_init(&g_cstPalRTCCfg);
	if(!eRet)
	{
		UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("<System time> RTC initialized failed!!!\n\r"));
	}
	else
	{
		eRet = pal_rtc_GetDateTime(&stRTCTime);
		if(!eRet)
		{
			UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("<System time> Can not get system time!!!\n\r"));
			return ERROR;
		}
		UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("<System time> RTC initialized successfully!!!\n\r"));
		UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("<System time> %d-%d-%d %d:%d:%d\n\r", \
											stRTCTime.u8Year + 2000,stRTCTime.u8Month,stRTCTime.u8Date,\
											stRTCTime.u8Hours,stRTCTime.u8Minutes,stRTCTime.u8Seconds));
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_systime_GetSystemTime(CTRL_SYSTIME_DATA *pstSysTime)
{
	ErrorStatus eRet;
	PAL_RTC_DATE_TIME stRTCTime;

	if(!pstSysTime)
		return ERROR;

	eRet = pal_rtc_GetDateTime(&stRTCTime);
	if(!eRet)
		return ERROR;

	pstSysTime->u16Year		=	stRTCTime.u8Year + 2000;
	pstSysTime->u8Month		=	stRTCTime.u8Month;
	pstSysTime->u8Date		=	stRTCTime.u8Date;

	pstSysTime->u8Hours		=	stRTCTime.u8Hours;
	pstSysTime->u8Minutes	=	stRTCTime.u8Minutes;
	pstSysTime->u8Seconds	=	stRTCTime.u8Seconds;

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_systime_SetSystemTime(CTRL_SYSTIME_DATA *pstSysTime)
{
	ErrorStatus eRet;
	PAL_RTC_DATE_TIME stRTCTime;

	if(!pstSysTime)
		return ERROR;

	stRTCTime.u8WeekDay		=	1;
	stRTCTime.u8Year		=	pstSysTime->u16Year - 2000;
	stRTCTime.u8Month		=	pstSysTime->u8Month;
	stRTCTime.u8Date		=	pstSysTime->u8Date;

	stRTCTime.u8Hours		=	pstSysTime->u8Hours;
	stRTCTime.u8Minutes		=	pstSysTime->u8Minutes;
	stRTCTime.u8Seconds		=	pstSysTime->u8Seconds;
	stRTCTime.u8AMPM		=	0;

	eRet = pal_rtc_SetDateTime(&stRTCTime);

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
