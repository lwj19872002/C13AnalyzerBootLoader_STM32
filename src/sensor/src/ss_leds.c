/*
 * ss_leds.c
 *
 *  Created on: 2013-4-1
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_LEDS
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

static SS_LEDS_INFO g_sstLEDsInfo;

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_leds_init(const SS_LEDS_CFG *pConfig)
{
	uint32_t			u32Dex;
	GPIO_InitTypeDef	GPIO_InitStructure;

	if(!pConfig)
		return ERROR;

	memset(&g_sstLEDsInfo, 0x00, sizeof(SS_LEDS_INFO));

	g_sstLEDsInfo.pPalConfig = pConfig;

	if(pConfig->u32APB1Clk)
		RCC_APB1PeriphClockCmd(pConfig->u32APB1Clk, ENABLE);
	if(pConfig->u32APB2Clk)
		RCC_APB2PeriphClockCmd(pConfig->u32APB2Clk, ENABLE);
	if(pConfig->u32AHBClk)
		RCC_AHBPeriphClockCmd(pConfig->u32AHBClk, ENABLE);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	for(u32Dex=0; u32Dex<SS_LEDS_NUM_OF_ALL; u32Dex++)
	{
		GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioLEDsPin[u32Dex];
		GPIO_Init(pConfig->GpioLEDsPort[u32Dex], &GPIO_InitStructure);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_leds_one_on(uint8_t u8Num)
{
	if(u8Num >= SS_LEDS_NUM_OF_ALL)
		return ERROR;

	GPIO_ResetBits(g_sstLEDsInfo.pPalConfig->GpioLEDsPort[u8Num], g_sstLEDsInfo.pPalConfig->u16GpioLEDsPin[u8Num]);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_leds_one_off(uint8_t u8Num)
{
	if(u8Num >= SS_LEDS_NUM_OF_ALL)
		return ERROR;

	GPIO_SetBits(g_sstLEDsInfo.pPalConfig->GpioLEDsPort[u8Num], g_sstLEDsInfo.pPalConfig->u16GpioLEDsPin[u8Num]);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
