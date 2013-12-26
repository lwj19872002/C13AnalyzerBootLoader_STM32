/*
 * ctrl_lightsource.c
 *
 *  Created on: 2013-4-9
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_LIGHTSOURCE
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static CTRL_LS_INFO g_sstLSInfo;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_ls_init(void)
{
	ErrorStatus eRet;

	memset(&g_sstLSInfo, 0x00, sizeof(CTRL_LS_INFO));

	eRet = pal_pwm_init(&g_cstPalLightSourcePWM);
	if(eRet)
	{
		g_sstLSInfo.pPalCfg = &g_cstPalLightSourcePWM;
		g_sstLSInfo.bInitOK = true;
		g_sstLSInfo.bLSisON = false;

		pal_pwm_output_off(g_sstLSInfo.pPalCfg->ePWMNum);
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_ls_on(void)
{
	if(!g_sstLSInfo.bInitOK)
		return;

	pal_pwm_output_on(g_sstLSInfo.pPalCfg->ePWMNum);

	g_sstLSInfo.bLSisON = true;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctlr_ls_off(void)
{
	if(!g_sstLSInfo.bInitOK)
		return;

	pal_pwm_output_off(g_sstLSInfo.pPalCfg->ePWMNum);

	g_sstLSInfo.bLSisON = false;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_ls_set_freq(uint32_t u32Hz)
{
	if(!g_sstLSInfo.bInitOK)
		return ERROR;

	if((u32Hz > CTRL_LIGHTSOURCE_MAXFREQ_HZ) || (u32Hz < CTRL_LIGHTSOURCE_MINFREQ_HZ))
		return ERROR;

	return pal_pwm_set_freq(g_sstLSInfo.pPalCfg->ePWMNum, u32Hz);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

#endif
