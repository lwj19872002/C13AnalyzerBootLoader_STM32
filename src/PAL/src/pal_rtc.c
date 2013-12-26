/*
 * pal_rtc.c
 *
 *  Created on: 2013-9-4
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "globaldata.h"

#ifdef PAL_RTC

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_rtc_init(const PAL_RTC_CFG *pstConfig)
{
	ErrorStatus			eRet;
	RTC_TimeTypeDef		stTime;
	RTC_DateTypeDef		stDate;

	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != PAL_RTC_FLAG)
	{
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		RCC_BackupResetCmd(ENABLE);
		RCC_BackupResetCmd(DISABLE);

		RCC_LSEConfig(RCC_LSE_ON);

		/* Wait till LSE is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable the RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		eRet = RTC_Init((RTC_InitTypeDef *)&pstConfig->RTC_InitStructure);
		if (!eRet)
		{
			goto out;
		}

		memcpy((uint8_t *)&stTime, (uint8_t *)&pstConfig->RTC_TimeStructure, sizeof(RTC_TimeTypeDef));
		eRet = RTC_SetTime(RTC_Format_BIN, &stTime);
		if(!eRet)
		{
			goto out;
		}

		memcpy((uint8_t *)&stDate, (uint8_t *)&pstConfig->RTC_DateStructure, sizeof(RTC_DateTypeDef));
		eRet = RTC_SetDate(RTC_Format_BIN, &stDate);
		if(!eRet)
		{
			goto out;
		}

		RTC_WriteBackupRegister(RTC_BKP_DR0, PAL_RTC_FLAG);

		PWR_BackupAccessCmd(DISABLE);
	}
	else
	{
		/* Wait for RTC APB registers synchronisation */
		eRet = RTC_WaitForSynchro();
	}

out:
	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_rtc_GetDateTime(PAL_RTC_DATE_TIME *pstDateTime)
{
	if(!pstDateTime)
		return ERROR;

	RTC_GetTime(RTC_Format_BIN, (RTC_TimeTypeDef *)(&pstDateTime->u8Hours));

	RTC_GetDate(RTC_Format_BIN, (RTC_DateTypeDef *)(&pstDateTime->u8WeekDay));

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_rtc_SetDateTime(PAL_RTC_DATE_TIME *pstDateTime)
{
	ErrorStatus eRet;

	if(!pstDateTime)
		return ERROR;

//	RTC_WriteProtectionCmd(DISABLE);

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	eRet = RTC_SetTime(RTC_Format_BIN, (RTC_TimeTypeDef *)(&pstDateTime->u8Hours));
	if(!eRet)
		goto out;

	eRet = RTC_SetDate(RTC_Format_BIN, (RTC_DateTypeDef *)(&pstDateTime->u8WeekDay));
	if(!eRet)
		goto out;

	PWR_BackupAccessCmd(DISABLE);

//	RTC_WriteProtectionCmd(ENABLE);

out:
	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
