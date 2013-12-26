/*
 * ss_radiotube.c
 *
 *  Created on: 2013-4-1
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_RADIOTUBE

static SS_RADIOTUBE_INFO g_sstSSRadioTubeInfo;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_radiotube_init(const SS_RADIOTUBE_CFG *pConfig)
{
	uint32_t			u32Dex;
	GPIO_InitTypeDef	GPIO_InitStructure;

	if(!pConfig)
		return ERROR;

	memset(&g_sstSSRadioTubeInfo, 0x00, sizeof(SS_RADIOTUBE_INFO));

	g_sstSSRadioTubeInfo.pPalConfig = pConfig;

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

	for(u32Dex=0; u32Dex<SS_RADIOTUBE_NUM_OF_ALL; u32Dex++)
	{
		GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioPin[u32Dex];
		GPIO_Init(pConfig->GpioPort[u32Dex], &GPIO_InitStructure);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_radiotube_on(uint8_t u8Num)
{
	if(u8Num >= SS_RADIOTUBE_NUM_OF_ALL)
		return ERROR;

	GPIO_SetBits(g_sstSSRadioTubeInfo.pPalConfig->GpioPort[u8Num], g_sstSSRadioTubeInfo.pPalConfig->u16GpioPin[u8Num]);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_radiotube_off(uint8_t u8Num)
{
	if(u8Num >= SS_RADIOTUBE_NUM_OF_ALL)
		return ERROR;

	GPIO_ResetBits(g_sstSSRadioTubeInfo.pPalConfig->GpioPort[u8Num], g_sstSSRadioTubeInfo.pPalConfig->u16GpioPin[u8Num]);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
