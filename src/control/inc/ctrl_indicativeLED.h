/*
 * ctrl_indicativeLED.h
 *
 *  Created on: 2013-2-27
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_INDICATIVELED_H_
#define CTRL_INDICATIVELED_H_

#include "pal_gpio.h"

#if(SYSCFG_DEBUG)
#define CTRL_IDLED_DEBUG			true
#else
#define CTRL_IDLED_DEBUG			false
#endif

typedef struct{
	const PAL_GPIO_INIT_ST *pPalGpioCfg;
} CTRL_IDLED_INFO;

ErrorStatus ctrl_IDLED_init(void);
void ctrl_IDLED_blink(void);

#endif /* CTRL_INDICATIVELED_H_ */
