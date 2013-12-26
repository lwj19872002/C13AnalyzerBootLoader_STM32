/*
 * BootLoader.c
 *
 *  Created on: 2013-9-5
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"
#include "App.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static ST_BL_INFO g_sstBootLoaderInfo;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static void BL_Process_WaitCMD_Init(void);
static E_BL_WAITCMD_RETURN BL_Process_WaitCMD(void);
static void BL_BootApplication(uint32_t u32AppAddr);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ErrorStatus BL_Initialize(void)
{
	g_sstBootLoaderInfo.eThreadStatus = e_BL_Init;
	return SUCCESS;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void BL_Thread(void)
{
	E_BL_WAITCMD_RETURN	eWaitCMDReturn;

	switch(g_sstBootLoaderInfo.eThreadStatus)
	{
	case e_BL_Init:
		BL_Process_WaitCMD_Init();
		g_sstBootLoaderInfo.eThreadStatus = e_BL_WaitCMD;
		break;
	case e_BL_WaitCMD:
		eWaitCMDReturn = BL_Process_WaitCMD();
		switch(eWaitCMDReturn)
		{
		case e_WaitCMDR_None:
			break;
		case e_WaitCMDR_UpdateApp:
			g_sstBootLoaderInfo.eThreadStatus = e_BL_UpdateApp;
			break;
		case e_WaitCMDR_MannulControl:
			break;
		case e_WaitCMDR_BootApp:
			g_sstBootLoaderInfo.eThreadStatus = e_BL_BootApp;
			break;
		default:
			g_sstBootLoaderInfo.eThreadStatus = e_BL_BootApp;
			break;
		}
		break;
	case e_BL_UpdateApp:
		if(e_UDStatus_End == protocol_GetUpdateFWStatus())
		{
			protocol_ResetUpdateFWStatus();
			g_sstBootLoaderInfo.eThreadStatus = e_BL_BootApp;
		}
		break;
	case e_BL_MannulControl:
		break;
	case e_BL_BootApp:
		BL_BootApplication(BL_APP_START_ADDR);
		break;
	case e_BL_Error:
		break;
	default:
		break;
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static void BL_Process_WaitCMD_Init(void)
{
	g_sstBootLoaderInfo.eWaitCMDStatus = e_WaitCMD_Init;
	g_sstBootLoaderInfo.u32WaitCMDFlag = 0;
}


static E_BL_WAITCMD_RETURN BL_Process_WaitCMD(void)
{
	E_BL_WAITCMD_RETURN		eRet;

	eRet = e_WaitCMDR_None;

	switch(g_sstBootLoaderInfo.eWaitCMDStatus)
	{
	case e_WaitCMD_Init:
		g_sstBootLoaderInfo.eWaitCMDStatus = e_WaitCMD_CheckCMD;
		break;
	case e_WaitCMD_CheckCMD:
		if(e_UDStatus_Start == protocol_GetUpdateFWStatus())
		{
			eRet = e_WaitCMDR_UpdateApp;
		}
		else
		{
			g_sstBootLoaderInfo.eWaitCMDStatus = e_WaitCMD_DelayInit;
		}
		break;
	case e_WaitCMD_DelayInit:
		ctrl_event_delay_clear();
		g_sstBootLoaderInfo.eWaitCMDStatus = e_WaitCMD_Delay;
		break;
	case e_WaitCMD_Delay:
		if(ctrl_event_delay_ms(100))
		{
			g_sstBootLoaderInfo.u32WaitCMDFlag++;
			if(g_sstBootLoaderInfo.u32WaitCMDFlag > BL_WAITCMD_TIMEOUT)
			{
				g_sstBootLoaderInfo.eWaitCMDStatus = e_WaitCMD_TimeOut;
			}
			else
			{
				g_sstBootLoaderInfo.eWaitCMDStatus = e_WaitCMD_CheckCMD;
			}
		}
		break;
	case e_WaitCMD_TimeOut:
		eRet = e_WaitCMDR_BootApp;
		break;
	default:
		eRet = e_WaitCMDR_BootApp;
		break;
	}

	return eRet;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
 * 功能：实现到应用程序的跳转
 * 输入：uint32_t u32AppAddr ------ 应用程序的FLASH地址
 * 输出：无
 * 返回：无
 */
static void BL_BootApplication(uint32_t u32AppAddr)
{
	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Boot application.........address: 0x%X\n\r",u32AppAddr));

	__disable_interrupt();	                         //禁止全局中断

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, u32AppAddr - BL_MCUFLASH_BASE); //重映射中断向量表到应用程序的位置，应用程序中不用再重映射

	__set_MSP(*(uint32_t *)u32AppAddr);                  //初始化栈指针

	((void (*)(void))(*(uint32_t *)(u32AppAddr + 4)))(); //跳转

	while(1);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
