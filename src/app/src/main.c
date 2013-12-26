/*
 * main.c
 *
 *  Created on: 2013-2-25
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"
#include "App.h"

void main(void)
{
	ErrorStatus eRet;

	ctrl_event_init();
	ctrl_IDLED_init();
	ctrl_debug_init();

	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("\n\r===================BootLoader===================\n\r"));
	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("================= SV:%s ======================\n\r", SYSCFG_SW_VERSION));
	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("================= HV:%s =====================\n\r", SYSCFG_HW_VERSION));
	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("http://www.wldyq.com\n\r"));
	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("=================================================\n\r"));
//	ctrl_kb_init();

	ctrl_linker_init();

	ctrl_systime_init();

	protocol_init();

	BL_Initialize();

	eRet = ctrl_event_StartSystemTicks();
	if(!eRet)
	{
		UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Failed to start system ticks!\n\r"));
		while(1);
	}

	while(1)
	{
		ctrl_event_main_thread();
		BL_Thread();
	}
}
