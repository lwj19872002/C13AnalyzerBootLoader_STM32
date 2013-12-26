/*
 * ctrl_pump.c
 *
 *  Created on: 2013-4-12
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_PUMP
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static CTRL_PUMP_INFO g_sstPumpInfo;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_pump_init(void)
{
	ErrorStatus eRet;

	memset(&g_sstPumpInfo, 0x00, sizeof(CTRL_PUMP_INFO));

#if(CTRL_PUMP_CTRLTYPE == CTRL_PUMP_CTRLUSE_GPIO)
	g_sstPumpInfo.pPalConfig = &g_cstPalPumpGPIOcfg;

	eRet = pal_gpio_init(g_sstPumpInfo.pPalConfig);
	if(!eRet)
		return ERROR;

	g_sstPumpInfo.bIsOpen = false;
	pal_gpio_resetpin(g_sstPumpInfo.pPalConfig);

	return SUCCESS;
#elif(CTRL_PUMP_CTRLTYPE == CTRL_PUMP_CTRLUSE_PWM)
	g_sstPumpInfo.pPalPWMCfg = &g_cstPalPumpPWMcfg;

	eRet = pal_pwm_init(g_sstPumpInfo.pPalPWMCfg);
	if(!eRet)
		return ERROR;

	return SUCCESS;
#else
	return ERROR;
#endif
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_pump_on(void)
{
	g_sstPumpInfo.bIsOpen = true;

#if(CTRL_PUMP_CTRLTYPE == CTRL_PUMP_CTRLUSE_GPIO)
	pal_gpio_setpin(g_sstPumpInfo.pPalConfig);
#elif(CTRL_PUMP_CTRLTYPE == CTRL_PUMP_CTRLUSE_PWM)
	pal_pwm_output_on(g_sstPumpInfo.pPalPWMCfg->ePWMNum);
#else

#endif
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_pump_off(void)
{
	g_sstPumpInfo.bIsOpen = false;

#if(CTRL_PUMP_CTRLTYPE == CTRL_PUMP_CTRLUSE_GPIO)
	pal_gpio_resetpin(g_sstPumpInfo.pPalConfig);
#elif(CTRL_PUMP_CTRLTYPE == CTRL_PUMP_CTRLUSE_PWM)
	pal_pwm_output_off(g_sstPumpInfo.pPalPWMCfg->ePWMNum);
#else

#endif
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
