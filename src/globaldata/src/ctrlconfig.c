/*
 * ctrlconfig.c
 *
 *  Created on: 2013-5-31
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
const CTRL_SAMPLE_SETING g_cstCtrlSampleSetting = {
	2,

#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
	{CTRLCFG_C12_ADC_CHANNEL, CTRLCFG_C13_ADC_CHANNEL},
#endif

	CTRLCFG_SAMPLE_PERIOD_US,
};
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
