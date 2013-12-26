/*
 * ctrl_event.c
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

#ifdef CTRL_EVENT
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static volatile uint64_t g_vu64SystemTime = 0;		// 记录仪器开机的秒数
static volatile uint64_t g_vu64SystemTicks = 0;		// 记录仪器开机的滴答数
static volatile uint32_t g_vu32EventFlag = 0;

static CTRL_EVENT_INFO	g_sstCtrlEventInfo;

static CTRL_EVENT_DELAY_INFO	g_sstCtrlEventDelayInfo;

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_event_is_1Min(void);
static ErrorStatus ctrl_event_is_1S(void);
static ErrorStatus ctrl_event_is_10ms(void);
static ErrorStatus ctrl_event_is_20ms(void);
static ErrorStatus ctrl_event_is_100ms(void);
static ErrorStatus ctrl_event_is_50ms(void);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_event_get_ticks(uint64_t *pu64Ticks)
{
	if(pu64Ticks)
		*pu64Ticks = g_vu64SystemTicks;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_init(void)
{
	g_vu64SystemTime = 0;
	g_vu64SystemTicks = 0;

	memset(&g_sstCtrlEventInfo, 0x00, sizeof(CTRL_EVENT_INFO));

	g_sstCtrlEventDelayInfo.bStart = false;
	g_sstCtrlEventDelayInfo.u32DelayMS = 0;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_StartSystemTicks(void)
{
	ErrorStatus Ret;

	Ret = pal_systick_set_app_handler(ctrl_event_handler);

	if(SUCCESS == Ret)
	{
		Ret = pal_systick_init(g_stc_u32SystemTickTime);
	}

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_event_handler(void)
{
	static uint32_t su32TimeFlag = 0;

	su32TimeFlag++;

	PAL_SYSTICK_ENTER_CRITICAL();

	if(su32TimeFlag)
	{
		g_vu32EventFlag |= e_event_flag_1Ms;

		g_vu64SystemTicks++;

		if((su32TimeFlag%e_event_10Ms) == 0)
		{
			g_vu32EventFlag |= e_event_flag_10Ms;
		}
		if((su32TimeFlag%e_event_20Ms) == 0)
		{
			g_vu32EventFlag |= e_event_flag_20Ms;
		}
		if((su32TimeFlag%e_event_50Ms) == 0)
		{
			g_vu32EventFlag |= e_event_flag_50Ms;
		}
		if((su32TimeFlag%e_event_100Ms) == 0)
		{
			g_vu32EventFlag |= e_event_flag_100Ms;
		}
		if((su32TimeFlag%e_event_1S) == 0)
		{
			g_vu32EventFlag |= e_event_flag_1S;

			g_vu64SystemTime++;
		}
		if((su32TimeFlag%e_event_1Min) == 0)
		{
			g_vu32EventFlag |= e_event_flag_1Min;

			su32TimeFlag = 0;
		}
	}

	PAL_SYSTICK_EXIT_CRITICAL();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_event_is_50ms(void)
{
	ErrorStatus Ret;

	PAL_SYSTICK_ENTER_CRITICAL();

	if(g_vu32EventFlag&e_event_flag_50Ms)
	{
		g_vu32EventFlag &= (~e_event_flag_50Ms);

		Ret = SUCCESS;
	}
	else
	{
		Ret = ERROR;
	}

	PAL_SYSTICK_EXIT_CRITICAL();

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_event_is_1Min(void)
{
	ErrorStatus Ret;

	PAL_SYSTICK_ENTER_CRITICAL();

	if(g_vu32EventFlag&e_event_flag_1Min)
	{
		g_vu32EventFlag &= (~e_event_flag_1Min);

		Ret = SUCCESS;
	}
	else
	{
		Ret = ERROR;
	}

	PAL_SYSTICK_EXIT_CRITICAL();

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_event_is_1S(void)
{
	ErrorStatus Ret;

	PAL_SYSTICK_ENTER_CRITICAL();

	if(g_vu32EventFlag&e_event_flag_1S)
	{
		g_vu32EventFlag &= (~e_event_flag_1S);

		Ret = SUCCESS;
	}
	else
	{
		Ret = ERROR;
	}

	PAL_SYSTICK_EXIT_CRITICAL();

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_event_is_10ms(void)
{
	ErrorStatus Ret;

	PAL_SYSTICK_ENTER_CRITICAL();

	if(g_vu32EventFlag&e_event_flag_10Ms)
	{
		g_vu32EventFlag &= (~e_event_flag_10Ms);

		Ret = SUCCESS;
	}
	else
	{
		Ret = ERROR;
	}

	PAL_SYSTICK_EXIT_CRITICAL();

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_event_is_20ms(void)
{
	ErrorStatus Ret;

	PAL_SYSTICK_ENTER_CRITICAL();

	if(g_vu32EventFlag&e_event_flag_20Ms)
	{
		g_vu32EventFlag &= (~e_event_flag_20Ms);

		Ret = SUCCESS;
	}
	else
	{
		Ret = ERROR;
	}

	PAL_SYSTICK_EXIT_CRITICAL();

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_event_is_100ms(void)
{
	ErrorStatus Ret;

	PAL_SYSTICK_ENTER_CRITICAL();

	if(g_vu32EventFlag&e_event_flag_100Ms)
	{
		g_vu32EventFlag &= (~e_event_flag_100Ms);

		Ret = SUCCESS;
	}
	else
	{
		Ret = ERROR;
	}

	PAL_SYSTICK_EXIT_CRITICAL();

	return Ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// 有main函数调用
void ctrl_event_main_thread(void)
{
	uint32_t u32Dex;

	if(ctrl_event_is_10ms())
	{
		for(u32Dex=0; u32Dex<g_sstCtrlEventInfo.u10MSThreadTotal; u32Dex++)
		{
			if(g_sstCtrlEventInfo.ap10MSThread[u32Dex])
				(*g_sstCtrlEventInfo.ap10MSThread[u32Dex])();
		}
	}

	if(ctrl_event_is_20ms())
	{
		for(u32Dex=0; u32Dex<g_sstCtrlEventInfo.u20MSThreadTotal; u32Dex++)
		{
			if(g_sstCtrlEventInfo.ap20MSThread[u32Dex])
				(*g_sstCtrlEventInfo.ap20MSThread[u32Dex])();
		}
	}

	if(ctrl_event_is_50ms())
	{
		for(u32Dex=0; u32Dex<g_sstCtrlEventInfo.u50MSThreadTotal; u32Dex++)
		{
			if(g_sstCtrlEventInfo.ap50MSThread[u32Dex])
				(*g_sstCtrlEventInfo.ap50MSThread[u32Dex])();
		}
	}

	if(ctrl_event_is_100ms())
	{
		for(u32Dex=0; u32Dex<g_sstCtrlEventInfo.u100MSThreadTotal; u32Dex++)
		{
			if(g_sstCtrlEventInfo.ap100MSThread[u32Dex])
				(*g_sstCtrlEventInfo.ap100MSThread[u32Dex])();
		}
	}

	if(ctrl_event_is_1S())
	{
		for(u32Dex=0; u32Dex<g_sstCtrlEventInfo.u1SThreadTotal; u32Dex++)
		{
			if(g_sstCtrlEventInfo.ap1SThread[u32Dex])
				(*g_sstCtrlEventInfo.ap1SThread[u32Dex])();
		}
	}

	if(ctrl_event_is_1Min())
	{
		for(u32Dex=0; u32Dex<g_sstCtrlEventInfo.u1MinThreadTotal; u32Dex++)
		{
			if(g_sstCtrlEventInfo.ap1MinThread[u32Dex])
				(*g_sstCtrlEventInfo.ap1MinThread[u32Dex])();
		}
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_add_50ms_thread(P_CTRL_EVENT_THREAD pThread)
{
	if(!pThread)
		return ERROR;

	if(g_sstCtrlEventInfo.u50MSThreadTotal >= CTRL_EVENT_MAX_THREAD)
		return ERROR;

	g_sstCtrlEventInfo.ap50MSThread[g_sstCtrlEventInfo.u50MSThreadTotal] = pThread;
	g_sstCtrlEventInfo.u50MSThreadTotal++;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_add_1min_thread(P_CTRL_EVENT_THREAD pThread)
{
	if(!pThread)
		return ERROR;

	if(g_sstCtrlEventInfo.u1MinThreadTotal >= CTRL_EVENT_MAX_THREAD)
		return ERROR;

	g_sstCtrlEventInfo.ap1MinThread[g_sstCtrlEventInfo.u1MinThreadTotal] = pThread;
	g_sstCtrlEventInfo.u1MinThreadTotal++;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_add_1s_thread(P_CTRL_EVENT_THREAD pThread)
{
	if(!pThread)
		return ERROR;

	if(g_sstCtrlEventInfo.u1SThreadTotal >= CTRL_EVENT_MAX_THREAD)
		return ERROR;

	g_sstCtrlEventInfo.ap1SThread[g_sstCtrlEventInfo.u1SThreadTotal] = pThread;
	g_sstCtrlEventInfo.u1SThreadTotal++;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_add_10ms_thread(P_CTRL_EVENT_THREAD pThread)
{
	if(!pThread)
		return ERROR;

	if(g_sstCtrlEventInfo.u10MSThreadTotal >= CTRL_EVENT_MAX_THREAD)
		return ERROR;

	g_sstCtrlEventInfo.ap10MSThread[g_sstCtrlEventInfo.u10MSThreadTotal] = pThread;
	g_sstCtrlEventInfo.u10MSThreadTotal++;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_add_20ms_thread(P_CTRL_EVENT_THREAD pThread)
{
	if(!pThread)
		return ERROR;

	if(g_sstCtrlEventInfo.u20MSThreadTotal >= CTRL_EVENT_MAX_THREAD)
		return ERROR;

	g_sstCtrlEventInfo.ap20MSThread[g_sstCtrlEventInfo.u20MSThreadTotal] = pThread;
	g_sstCtrlEventInfo.u20MSThreadTotal++;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_event_add_100ms_thread(P_CTRL_EVENT_THREAD pThread)
{
	if(!pThread)
		return ERROR;

	if(g_sstCtrlEventInfo.u100MSThreadTotal >= CTRL_EVENT_MAX_THREAD)
		return ERROR;

	g_sstCtrlEventInfo.ap100MSThread[g_sstCtrlEventInfo.u100MSThreadTotal] = pThread;
	g_sstCtrlEventInfo.u100MSThreadTotal++;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
bool ctrl_event_delay_ms(uint32_t u32MS)
{
	uint64_t u64Temp;
	bool bRet;

	PAL_SYSTICK_ENTER_CRITICAL();
	u64Temp = g_vu64SystemTicks;
	PAL_SYSTICK_EXIT_CRITICAL();

	bRet = false;
	if(g_sstCtrlEventDelayInfo.bStart)
	{
		if((u64Temp - g_sstCtrlEventDelayInfo.u64StartTicks) >= g_sstCtrlEventDelayInfo.u32DelayMS)
		{
			bRet = true;
			g_sstCtrlEventDelayInfo.bStart = false;
		}
	}
	else
	{
		g_sstCtrlEventDelayInfo.u64StartTicks = u64Temp;
		g_sstCtrlEventDelayInfo.u32DelayMS = u32MS;
		g_sstCtrlEventDelayInfo.bStart = true;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_event_delay_clear(void)
{
	g_sstCtrlEventDelayInfo.bStart = false;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

#endif
