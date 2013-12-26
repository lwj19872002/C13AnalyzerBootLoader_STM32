/*
 * ctrl_linker.c
 *
 *  Created on: 2013-4-16
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_LINKER
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static CTRL_LINKER_INFO g_sstCtrlLinkerP1Info;
static CTRL_LINKER_INFO g_sstCtrlLinkerP2Info;
static CTRL_LINKER_INFO g_sstCtrlLinkerP3Info;

static ErrorStatus ctrl_linker_get_info(CTRL_LINKER_PORT ePort, CTRL_LINKER_INFO **pInfo);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_linker_init(void)
{
	ErrorStatus eRet;

	g_sstCtrlLinkerP1Info.pPalCOMCfg = &g_cstPalLinkerP1Uart;
	eRet = pal_com_init(&g_cstPalLinkerP1Uart);
	g_sstCtrlLinkerP1Info.bInitOK = false;
	if(eRet)
	{
		g_sstCtrlLinkerP1Info.bInitOK = true;
	}
	else
	{
		UTIL_DEBUG_PRINTF(CTRL_LINKER_DEBUG, ("Initialize linker port1 error!\n\r"));
	}

	g_sstCtrlLinkerP2Info.bInitOK = false;
	g_sstCtrlLinkerP2Info.pPalCOMCfg = &g_cstPalLinkerP2Uart;
	eRet = pal_com_init(&g_cstPalLinkerP2Uart);
	if(eRet)
	{
		g_sstCtrlLinkerP2Info.bInitOK = true;
	}
	else
	{
		UTIL_DEBUG_PRINTF(CTRL_LINKER_DEBUG, ("Initialize linker port2 error!\n\r"));
	}

	g_sstCtrlLinkerP3Info.bInitOK = false;

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ctrl_linker_get_info(CTRL_LINKER_PORT ePort, CTRL_LINKER_INFO **pInfo)
{
	ErrorStatus eRet;

	if(!pInfo)
		return ERROR;

	switch (ePort)
	{
	case e_linker_P1:
		*pInfo = &g_sstCtrlLinkerP1Info;
		eRet = SUCCESS;
		break;
	case e_linker_P2:
		*pInfo = &g_sstCtrlLinkerP2Info;
		eRet = SUCCESS;
		break;
	case e_linker_P3:
		*pInfo = &g_sstCtrlLinkerP3Info;
		eRet = SUCCESS;
		break;
	default:
		eRet = ERROR;
		break;
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_linker_send(CTRL_LINKER_PORT ePort, uint8_t *pdat, uint16_t u16Len)
{
	CTRL_LINKER_INFO	*pInfo;

	if(!ctrl_linker_get_info(ePort, &pInfo))
	{
		return ERROR;
	}
	if(!pInfo->bInitOK)
	{
		return ERROR;
	}

	return pal_com_send_data(pInfo->pPalCOMCfg->eCOMX, pdat, u16Len);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_linker_read(CTRL_LINKER_PORT ePort, uint8_t * pRecDat, uint16_t u16Maxlen, uint16_t *pu16RecLen)
{
	CTRL_LINKER_INFO	*pInfo;

	if(!ctrl_linker_get_info(ePort, &pInfo))
	{
		return ERROR;
	}
	if(!pInfo->bInitOK)
	{
		return ERROR;
	}

	return pal_com_get_recdata(pInfo->pPalCOMCfg->eCOMX, pRecDat, u16Maxlen, pu16RecLen);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
