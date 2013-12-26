/*
 * ctrl_lightsource.h
 *
 *  Created on: 2013-4-9
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_LIGHTSOURCE_H_
#define CTRL_LIGHTSOURCE_H_

#define CTRL_LIGHTSOURCE_MAXFREQ_HZ			15
#define CTRL_LIGHTSOURCE_MINFREQ_HZ			5

typedef struct{
	bool				bInitOK;
	bool				bLSisON;

	const PAL_PWM_CFG	*pPalCfg;
} CTRL_LS_INFO;

ErrorStatus ctrl_ls_init(void);
void ctrl_ls_on(void);
void ctlr_ls_off(void);
ErrorStatus ctrl_ls_set_freq(uint32_t u32Hz);

#endif /* CTRL_LIGHTSOURCE_H_ */
