/*
 * ctrl_keyboard.c
 *
 *  Created on: 2013-4-15
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_KEYBOARD
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static CTRL_KB_INFO	g_sstCtrlKBInfo;

#if(CTRL_KB_SS_LAYER == CTRL_KB_SS_3KEY)
const static uint8_t g_csau8KeyValueTB[4] = {
	CTRL_KB_KEY_NULL,
	CTRL_KB_KEY_FUNC,
	CTRL_KB_KEY_NO,
	CTRL_KB_KEY_YES,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_kb_init(void)
{
	g_sstCtrlKBInfo.bInitOK = false;
	g_sstCtrlKBInfo.pReadKeyValue = NULL;
	g_sstCtrlKBInfo.pScanThread = NULL;
	g_sstCtrlKBInfo.bOpenVoice = true;

	g_sstCtrlKBInfo.u8BufferInDex = 0;
	g_sstCtrlKBInfo.u8BufferOutDex = 0;
	UTIL_FIFO_CLEAN(g_sstCtrlKBInfo.u8BufferInDex, g_sstCtrlKBInfo.u8BufferOutDex);

#if(CTRL_KB_SS_LAYER == CTRL_KB_SS_3KEY)
	if(!ss_kb_3key_init(&g_cstKB3KeyCfg))
	{
		return ERROR;
	}

	g_sstCtrlKBInfo.pReadKeyValue = ss_kb_3key_read_key;
	g_sstCtrlKBInfo.pScanThread = ss_kb_3key_thread;
	g_sstCtrlKBInfo.bInitOK = true;

	if(!ctrl_event_add_10ms_thread(ctrl_kb_scan_thread))
	{
		UTIL_DEBUG_PRINTF(CTRL_KB_DEBUG, ("Ctrl keyboard event thread initialized failed!\n\r"));
		return ERROR;
	}

	return SUCCESS;
#else
	return ERROR;
#endif
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// 按键控件扫描线程，调用sensor层相应的扫描线程,每10ms调用一次
void ctrl_kb_scan_thread(void)
{
	bool bNewKey;
	SS_KB_KEY_VALUE eSSKey;

	bNewKey = false;

	if(g_sstCtrlKBInfo.bInitOK && (g_sstCtrlKBInfo.pScanThread))
	{
		(*g_sstCtrlKBInfo.pScanThread)(&bNewKey);
	}

	if(bNewKey)
	{
		if((*g_sstCtrlKBInfo.pReadKeyValue)(&eSSKey))
		{
			if(UTIL_FIFO_REMAIN(g_sstCtrlKBInfo.u8BufferOutDex, g_sstCtrlKBInfo.u8BufferInDex, CTRL_KB_BUFFER_LENGTH))
			{
				g_sstCtrlKBInfo.au8KeyBuffer[g_sstCtrlKBInfo.u8BufferInDex++] = g_csau8KeyValueTB[eSSKey];
				UTIL_FIFO_DEX_INC(g_sstCtrlKBInfo.u8BufferInDex, CTRL_KB_BUFFER_LENGTH);
			}
		}
	}

	if(bNewKey && g_sstCtrlKBInfo.bOpenVoice)
	{
		ctrl_voice_key_down();
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
uint8_t ctrl_kb_read_key(void)
{
	uint8_t u8Key;


	u8Key = CTRL_KB_KEY_NULL;

	if(UTIL_FIFO_COUNT(g_sstCtrlKBInfo.u8BufferOutDex, g_sstCtrlKBInfo.u8BufferInDex, CTRL_KB_BUFFER_LENGTH))
	{
		u8Key = g_sstCtrlKBInfo.au8KeyBuffer[g_sstCtrlKBInfo.u8BufferOutDex];
		UTIL_FIFO_DEX_INC(g_sstCtrlKBInfo.u8BufferOutDex, CTRL_KB_BUFFER_LENGTH);
	}

	return u8Key;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_kb_AddKeyManul(uint8_t u8Key)
{
	ErrorStatus eRet;

	eRet = SUCCESS;

	if(UTIL_FIFO_REMAIN(g_sstCtrlKBInfo.u8BufferOutDex, g_sstCtrlKBInfo.u8BufferInDex, CTRL_KB_BUFFER_LENGTH))
	{
		g_sstCtrlKBInfo.au8KeyBuffer[g_sstCtrlKBInfo.u8BufferInDex] = u8Key;
		UTIL_FIFO_DEX_INC(g_sstCtrlKBInfo.u8BufferInDex, CTRL_KB_BUFFER_LENGTH);
	}
	else
	{
		eRet = ERROR;
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_kb_CleanBuffer(void)
{
	UTIL_FIFO_CLEAN(g_sstCtrlKBInfo.u8BufferInDex, g_sstCtrlKBInfo.u8BufferOutDex);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

#endif
