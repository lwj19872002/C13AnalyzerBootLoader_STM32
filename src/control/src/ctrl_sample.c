/*
 * ctrl_sample.c
 *
 *  Created on: 2013-4-8
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_SAMPLE
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static volatile CTRL_SAMPLE_INFO g_sstSampleInfo;
static volatile uint32_t g_svu32SampleFlag;
static volatile uint32_t g_svu32SampleTime;
static volatile uint32_t g_svu32SampleDex;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void ctrl_sample_init_channel(void);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_sample_init(void)
{
	ErrorStatus eRet;

	memset((uint8_t *)&g_sstSampleInfo, 0x00, sizeof(CTRL_SAMPLE_INFO));

//	eRet = pal_timer_init(&g_cstSampleTimer);
//	if(!eRet)
//		return ERROR;

	eRet = ERROR;
#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
	eRet =	ss_adc_ad7739_init();
#endif
	if(!eRet)
	{
		return ERROR;
	}
	else
	{
		ctrl_sample_init_channel();
	}

//	eRet = pal_timer_set_userfunc(g_cstSampleTimer.eTimerNum, ctrl_sample_timer_handler);
//	if(!eRet)
//		return ERROR;

//	g_sstSampleInfo.eTimerNum	=	g_cstSampleTimer.eTimerNum;
	g_sstSampleInfo.eStatue		=	e_sample_idle;
	g_sstSampleInfo.bBlock		=	false;

	ctrl_sample_set((CTRL_SAMPLE_SETING *)&g_cstCtrlSampleSetting);

	ctrl_event_add_20ms_thread(ctrl_sample_timer_handler);

	return SUCCESS;
}
////-------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------
//void ctrl_sample_timer_handler(void)
//{
//	uint8_t u8Dex;
//	uint32_t u32Temp;
//	uint32_t u32BuffDex;
//	ErrorStatus	eRet;
////	uint32_t	u32Buff[8];
//
//
//	for(u8Dex=0; u8Dex<g_sstSampleInfo.stUserCfg.u8SampleChannel; u8Dex++)
//	{
//		eRet = ERROR;
//#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
//		eRet = ss_adc_ad7739_sample_onech_once(g_sstSampleInfo.stUserCfg.aeADCChannels[u8Dex], &u32Temp);
//		//eRet = ss_adc_ad7739_blocksample_once(g_sstSampleInfo.stUserCfg.aeADCChannels[u8Dex], &u32Temp);
//
////		u32Temp = 1234;
////		eRet = SUCCESS;
//#else
//		u32Temp = 1234;
//		eRet = SUCCESS;
//#endif
//		if(!eRet)
//			break;
//
//		u32BuffDex = u8Dex * g_sstSampleInfo.stUserCfg.u32SamlePoints;
//		u32BuffDex += g_svu32SampleFlag;
//		g_sstSampleInfo.stUserCfg.pu32UserDataBuff[u32BuffDex] = u32Temp;
//	}
//
//	if(eRet)
//	{
//		g_svu32SampleFlag++;
//	}
//
////#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
////	eRet = ss_adc_ad7739_sample_multch_once(g_sstSampleInfo.stUserCfg.aeADCChannels, g_sstSampleInfo.stUserCfg.u8SampleChannel, u32Buff);
////#else
////	eRet = ERROR;
////#endif
////
////	if(eRet)
////	{
////		for(u8Dex=0; u8Dex<g_sstSampleInfo.stUserCfg.u8SampleChannel; u8Dex++)
////		{
////			g_sstSampleInfo.stUserCfg.pu32UserDataBuff[u8Dex * g_sstSampleInfo.stUserCfg.u32SamlePoints + g_sstSampleInfo.u32SampleFlag] = u32Buff[u8Dex];
////		}
////		g_sstSampleInfo.u32SampleFlag++;
////	}
//
//
//	u32Temp = g_sstSampleInfo.stUserCfg.u32SamlePoints;
//	if(g_svu32SampleFlag >= u32Temp)
//	{
//		g_sstSampleInfo.eStatue = e_sample_idle;
//		g_svu32SampleFlag = 0;
//		pal_timer_stop(g_sstSampleInfo.eTimerNum);
//	}
//
//}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_sample_timer_handler(void)
{
	uint8_t u8Dex;
	uint32_t u32Temp;
	uint32_t u32BuffDex;
	uint32_t u32SampleDex;
	ErrorStatus	eRet;
//	uint32_t	u32Buff[8];

	if(g_sstSampleInfo.bBlock)
		return;

	g_svu32SampleTime++;
	u32SampleDex = g_svu32SampleDex;
	if(g_svu32SampleTime >= u32SampleDex)
	{

		for(u8Dex=0; u8Dex<g_sstSampleInfo.stSetting.u8SampleChannel; u8Dex++)
		{
			eRet = ERROR;
#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
//			eRet = ss_adc_ad7739_sample_onech_once(g_sstSampleInfo.stUserCfg.aeADCChannels[u8Dex], &u32Temp);
			eRet = ss_adc_ad7739_sample_onech_once(g_sstSampleInfo.stSetting.aeADCChannels[u8Dex], \
					(uint32_t *)&g_sstSampleInfo.au32ADCData[g_sstSampleInfo.stSetting.aeADCChannels[u8Dex]]);
			//eRet = ss_adc_ad7739_blocksample_once(g_sstSampleInfo.stUserCfg.aeADCChannels[u8Dex], &u32Temp);

#else
			u32Temp = 1234;
			eRet = SUCCESS;
#endif
			if(!eRet)
				break;

			if(e_sample_sampling == g_sstSampleInfo.eStatue)
			{
				u32BuffDex = u8Dex * g_sstSampleInfo.stUserCfg.u32SamlePoints;
				u32BuffDex += g_svu32SampleFlag;
				g_sstSampleInfo.stUserCfg.pu32UserDataBuff[u32BuffDex] = g_sstSampleInfo.au32ADCData[g_sstSampleInfo.stSetting.aeADCChannels[u8Dex]];
			}
		}

		if(e_sample_sampling == g_sstSampleInfo.eStatue)
		{
			if(eRet)
			{
				g_svu32SampleFlag++;
			}

	//#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
	//	eRet = ss_adc_ad7739_sample_multch_once(g_sstSampleInfo.stUserCfg.aeADCChannels, g_sstSampleInfo.stUserCfg.u8SampleChannel, u32Buff);
	//#else
	//	eRet = ERROR;
	//#endif
	//
	//	if(eRet)
	//	{
	//		for(u8Dex=0; u8Dex<g_sstSampleInfo.stUserCfg.u8SampleChannel; u8Dex++)
	//		{
	//			g_sstSampleInfo.stUserCfg.pu32UserDataBuff[u8Dex * g_sstSampleInfo.stUserCfg.u32SamlePoints + g_sstSampleInfo.u32SampleFlag] = u32Buff[u8Dex];
	//		}
	//		g_sstSampleInfo.u32SampleFlag++;
	//	}


			u32Temp = g_sstSampleInfo.stUserCfg.u32SamlePoints;
			if(g_svu32SampleFlag >= u32Temp)
			{
				g_sstSampleInfo.eStatue = e_sample_dataok;
				g_svu32SampleFlag = 0;
	//			pal_timer_stop(g_sstSampleInfo.eTimerNum);
			}
		}
		else
		{
			g_svu32SampleFlag = 0;
		}

		g_svu32SampleTime = 0;
	}

}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
bool ctrl_sample_is_ok(void)
{
	CTRL_SAMPLE_STATUE eStatus;

//	pal_timer_enter_critical(g_sstSampleInfo.eTimerNum);
	eStatus = g_sstSampleInfo.eStatue;
//	pal_timer_exit_critical(g_sstSampleInfo.eTimerNum);

	if(e_sample_dataok == eStatus)
	{
		g_sstSampleInfo.eStatue = e_sample_idle;
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_sample_set(CTRL_SAMPLE_SETING *pSampleSet)
{
	if(e_sample_idle != g_sstSampleInfo.eStatue)
		return ERROR;

	if(!pSampleSet)
		return ERROR;

	if(pSampleSet->u8SampleChannel == 0)
		return ERROR;

	memcpy((uint8_t *)&g_sstSampleInfo.stSetting, pSampleSet, sizeof(CTRL_SAMPLE_SETING));

	g_svu32SampleFlag = 0;
	g_svu32SampleDex = g_sstSampleInfo.stSetting.u32SamplePeriod / 10000;
	g_svu32SampleTime = 0;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_sample_start(CTRL_SAMPLE_USER_INFO *pUserCfg)
{
	ErrorStatus eRet;
	CTRL_SAMPLE_STATUE eStatus;

	eRet = SUCCESS;
//	pal_timer_enter_critical(g_sstSampleInfo.eTimerNum);
	eStatus = g_sstSampleInfo.eStatue;
//	pal_timer_exit_critical(g_sstSampleInfo.eTimerNum);

	if(e_sample_idle != eStatus)
		return ERROR;

	if(!pUserCfg)
		return ERROR;

	if(pUserCfg->u32SamlePoints == 0)
	{
		return ERROR;
	}

	if(!pUserCfg->pu32UserDataBuff)
	{
		return ERROR;
	}

	memcpy((uint8_t *)&g_sstSampleInfo.stUserCfg, pUserCfg, sizeof(CTRL_SAMPLE_USER_INFO));

//	pal_timer_stop(g_sstSampleInfo.eTimerNum);
//
//	eRet = pal_timer_set_period(g_sstSampleInfo.eTimerNum, g_sstSampleInfo.stUserCfg.u32SamplePeriod);
//	if(!eRet)
//		return ERROR;

	g_sstSampleInfo.eStatue = e_sample_sampling;
	g_svu32SampleFlag = 0;

//	eRet = pal_timer_start(g_sstSampleInfo.eTimerNum);

//	if(eRet)
//	{
//		g_sstSampleInfo.eStatue = e_sample_busy;
//	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_sample_stop(void)
{
//	pal_timer_enter_critical(g_sstSampleInfo.eTimerNum);

	if(e_sample_idle != g_sstSampleInfo.eStatue)
	{
		//pal_timer_stop(g_sstSampleInfo.eTimerNum);

		g_sstSampleInfo.eStatue = e_sample_idle;
	}

//	pal_timer_exit_critical(g_sstSampleInfo.eTimerNum);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void ctrl_sample_init_channel(void)
{
#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)

#endif
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_sample_once(CTRL_SAMPLE_USER_INFO *pUserInfo)
{
	ErrorStatus eRet;
	uint8_t u8Dex;
	uint32_t u32Temp;

	for(u8Dex=0; u8Dex<g_sstSampleInfo.stSetting.u8SampleChannel; u8Dex++)
	{
		eRet = ERROR;
#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
		eRet = ss_adc_ad7739_sample_onech_once(g_sstSampleInfo.stSetting.aeADCChannels[u8Dex], &u32Temp);
#endif
		if(!eRet)
			break;

		pUserInfo->pu32UserDataBuff[u8Dex] = u32Temp;
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_sample_get_adc_value(uint32_t *pau32ADCVal)
{
	if(!pau32ADCVal)
		return ERROR;

#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
	memcpy(pau32ADCVal, (uint8_t *)g_sstSampleInfo.au32ADCData, ((e_ad7739_ch7 + 1)*(sizeof(uint32_t))));

	return SUCCESS;
#else

	return ERROR;
#endif
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_sample_block(bool bBlock)
{
	g_sstSampleInfo.bBlock = bBlock;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
