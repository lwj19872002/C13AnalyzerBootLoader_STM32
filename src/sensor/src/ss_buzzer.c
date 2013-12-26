/*
 * ss_buzzer.c
 *
 *  Created on: 2013-4-8
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_BUZZER

const static uint32_t g_csau32ToneTable[e_buzzer_tone_6 + 1] = {
	100,
	200,
	300,
	400,
	500,
	2000,
};

static SS_BUZZER_INFO g_sstBuzzerInfo;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_buzzer_init(void)
{
	ErrorStatus eRet;

	eRet = pal_pwm_init(&g_cstPalBuzzerPWM);
	if(eRet)
	{
		g_sstBuzzerInfo.pPalPWMCfg = &g_cstPalBuzzerPWM;
		pal_pwm_output_off(g_sstBuzzerInfo.pPalPWMCfg->ePWMNum);
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_buzzer_on(void)
{
	if(!g_sstBuzzerInfo.pPalPWMCfg)
		return ERROR;

	pal_pwm_output_on(g_sstBuzzerInfo.pPalPWMCfg->ePWMNum);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_buzzer_off(void)
{
	if(!g_sstBuzzerInfo.pPalPWMCfg)
		return ERROR;

	pal_pwm_output_off(g_sstBuzzerInfo.pPalPWMCfg->ePWMNum);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_buzzer_set_tone(SS_BUZZER_TONE eTone)
{
	if(!g_sstBuzzerInfo.pPalPWMCfg)
		return ERROR;

	return pal_pwm_set_freq(g_sstBuzzerInfo.pPalPWMCfg->ePWMNum, g_csau32ToneTable[eTone]);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------


#endif
