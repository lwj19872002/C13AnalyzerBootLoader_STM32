/*
 * ctrl_debug.c
 *
 *  Created on: 2013-4-4
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_DEBUG

void ctrl_debug_init(void)
{
	pal_stdio_init(&g_cstPalStdioUart);
}


#endif
