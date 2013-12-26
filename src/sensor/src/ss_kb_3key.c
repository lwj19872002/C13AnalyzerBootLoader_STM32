/*
 * ss_keyboard_3key.c
 *
 *  Created on: 2013-4-13
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_KB_3KEY
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static SS_KB_3KEY_INFO g_sstSSKB3KeyInfo;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void ss_kb_3key_decode(uint16_t u16IOData);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_kb_3key_init(const SS_KB_3KEY_CFG *pConfig)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	if(!pConfig)
		return ERROR;

	memset(&g_sstSSKB3KeyInfo, 0x00, sizeof(SS_KB_3KEY_INFO));

	g_sstSSKB3KeyInfo.pSSConfig = pConfig;
	g_sstSSKB3KeyInfo.eKeyValue = e_key_null;

	if(pConfig->u32APB1Clk)
		RCC_APB1PeriphClockCmd(pConfig->u32APB1Clk, ENABLE);
	if(pConfig->u32APB2Clk)
		RCC_APB2PeriphClockCmd(pConfig->u32APB2Clk, ENABLE);
	if(pConfig->u32AHBClk)
		RCC_AHBPeriphClockCmd(pConfig->u32AHBClk, ENABLE);

	GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioCOMPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pConfig->GpioCOMPort, &GPIO_InitStructure);
	GPIO_SetBits(pConfig->GpioCOMPort, pConfig->u16GpioCOMPin);		// 把公共线输出高电平

	GPIO_InitStructure.GPIO_Pin = (pConfig->u16GpioK1Pin) | (pConfig->u16GpioK2Pin) | (pConfig->u16GpioK3Pin);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pConfig->GpioCOMPort, &GPIO_InitStructure);		// 把其他扫描引脚设置为下拉输入模式

	g_sstSSKB3KeyInfo.u16IOMask = (0x0001 << pConfig->u8K1PinShift)|(0x0001 << pConfig->u8K2PinShift)|(0x0001 << pConfig->u8K3PinShift);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// 按键扫描线程，必须每10ms调用一次
void ss_kb_3key_thread(bool *pbNewKey)
{
	static uint16_t su16IOBuff = 0x00;
	static uint32_t su32Flag = 0;
	uint16_t u16Temp;

	u16Temp = (g_sstSSKB3KeyInfo.pSSConfig->GpioCOMPort->IDR) & (g_sstSSKB3KeyInfo.u16IOMask);

	*pbNewKey = false;
	if((SS_KB_3KEY_SHAKE_DELAY < su32Flag) && (u16Temp == 0))
	{
		ss_kb_3key_decode(su16IOBuff);
		su16IOBuff = u16Temp;
		su32Flag = 0;
		*pbNewKey = true;
	}

	if(u16Temp && (u16Temp == su16IOBuff))
	{
		su32Flag++;
	}
	else
	{
		su16IOBuff = u16Temp;
		su32Flag = 0;
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void ss_kb_3key_decode(uint16_t u16IOData)
{
	if(u16IOData & (0x0001 << g_sstSSKB3KeyInfo.pSSConfig->u8K1PinShift))
	{
		g_sstSSKB3KeyInfo.eKeyValue = e_key_1;
	}
	else if(u16IOData & (0x0001 << g_sstSSKB3KeyInfo.pSSConfig->u8K2PinShift))
	{
		g_sstSSKB3KeyInfo.eKeyValue = e_key_2;
	}
	else if(u16IOData & (0x0001 << g_sstSSKB3KeyInfo.pSSConfig->u8K3PinShift))
	{
		g_sstSSKB3KeyInfo.eKeyValue = e_key_3;
	}
	else
	{
		g_sstSSKB3KeyInfo.eKeyValue = e_key_null;
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_kb_3key_read_key(SS_KB_KEY_VALUE *peKeyValue)
{
	if(!peKeyValue)
		return ERROR;

	*peKeyValue = g_sstSSKB3KeyInfo.eKeyValue;
	g_sstSSKB3KeyInfo.eKeyValue = e_key_null;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------


#endif
