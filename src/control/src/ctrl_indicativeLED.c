/*
 * ctrl_indicateLED.c
 *
 *  Created on: 2013-2-27
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_INDICATIVELED

static CTRL_IDLED_INFO g_sstCtrlIDLEDInfo;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_IDLED_init(void)
{
	ErrorStatus Ret;

	memset(&g_sstCtrlIDLEDInfo, 0x00, sizeof(CTRL_IDLED_INFO));

	g_sstCtrlIDLEDInfo.pPalGpioCfg = &g_cstPalIndicateLEDGPIOcfg;

	Ret = pal_gpio_init(g_sstCtrlIDLEDInfo.pPalGpioCfg);

	if(!Ret)
	{
		return ERROR;
	}

	Ret = ctrl_event_add_100ms_thread(ctrl_IDLED_blink);
	if(!Ret)
	{
		UTIL_DEBUG_PRINTF(CTRL_IDLED_DEBUG, ("Ctrl indicative led thread initialized failed!\n\r"));
	}

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_IDLED_blink(void)
{
	static uint32_t su32Flag = 0;

	su32Flag++;

	if(su32Flag&0x01)
		pal_gpio_resetpin(g_sstCtrlIDLEDInfo.pPalGpioCfg);
	else
		pal_gpio_setpin(g_sstCtrlIDLEDInfo.pPalGpioCfg);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
