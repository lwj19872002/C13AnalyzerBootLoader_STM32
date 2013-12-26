/*
 * ctrl_pump.h
 *
 *  Created on: 2013-4-12
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_PUMP_H_
#define CTRL_PUMP_H_

#define CTRL_PUMP_CTRLUSE_GPIO		0
#define CTRL_PUMP_CTRLUSE_PWM		1

#define CTRL_PUMP_CTRLTYPE			CTRL_PUMP_CTRLUSE_GPIO

typedef struct{
	const PAL_GPIO_INIT_ST	*pPalConfig;

	const PAL_PWM_CFG		*pPalPWMCfg;

	bool					bIsOpen;
} CTRL_PUMP_INFO;


ErrorStatus ctrl_pump_init(void);
void ctrl_pump_on(void);
void ctrl_pump_off(void);

#endif /* CTRL_PUMP_H_ */
