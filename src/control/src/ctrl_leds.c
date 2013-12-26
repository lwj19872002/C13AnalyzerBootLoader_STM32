/*
 * ctrl_leds.c
 *
 *  Created on: 2013-2-26
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_LEDS

static CTRL_LEDS_INFO g_sstCtrlLEDSInfo;

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_leds_init(void)
{
	ErrorStatus eRet;

	memset(&g_sstCtrlLEDSInfo, 0x00, sizeof(CTRL_LEDS_INFO));

	g_sstCtrlLEDSInfo.u32BlinkFreq = CTRL_LEDS_BLINK_FREQ_DEF;

	eRet = ss_leds_init(&g_cstLEDsCfg);

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// Called by main every 100ms
void ctrl_leds_thread(void)
{
	uint8_t u8Dex;
	static uint32_t su32Flag = 0;
	static uint32_t su32LEDBlink = 0x00;

	if(!g_sstCtrlLEDSInfo.u32BlinkLEDs)
		return;

	su32Flag++;
	if(su32Flag > g_sstCtrlLEDSInfo.u32BlinkFreq)
	{
		su32LEDBlink ^= g_sstCtrlLEDSInfo.u32BlinkLEDs;
		for(u8Dex=0; u8Dex<CTRL_LEDS_MAX_NUM; u8Dex++)
		{
			if((g_sstCtrlLEDSInfo.u32BlinkLEDs >> u8Dex) & 0x00000001)
			{
				if((su32LEDBlink >> u8Dex) & 0x00000001)
					ss_leds_one_on(u8Dex);
				else
					ss_leds_one_off(u8Dex);
			}
		}

		su32Flag=0;
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_leds_on(CTRL_LEDS_NUM eNum)
{
	uint8_t u8Dex;

	for(u8Dex=0; u8Dex<CTRL_LEDS_MAX_NUM; u8Dex++)
	{
		if((eNum >> u8Dex) & 0x0001)
		{
			ss_leds_one_on(u8Dex);
		}
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_leds_off(CTRL_LEDS_NUM eNum)
{
	uint8_t u8Dex;

	for(u8Dex=0; u8Dex<CTRL_LEDS_MAX_NUM; u8Dex++)
	{
		if((eNum >> u8Dex) & 0x0001)
		{
			ss_leds_one_off(u8Dex);
		}
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_leds_blink_on(CTRL_LEDS_NUM eNums)
{
	g_sstCtrlLEDSInfo.u32BlinkLEDs |= (uint32_t)eNums;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_leds_blink_off(CTRL_LEDS_NUM eNums)
{
	g_sstCtrlLEDSInfo.u32BlinkLEDs &= (~(uint32_t)eNums);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_leds_set_blink_freq(uint32_t u32Freq)
{
	if(!u32Freq)
		return ERROR;

	if(u32Freq > CTRL_LEDS_BLINK_FREQ_MAX)
		return ERROR;

	g_sstCtrlLEDSInfo.u32BlinkFreq = u32Freq;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
