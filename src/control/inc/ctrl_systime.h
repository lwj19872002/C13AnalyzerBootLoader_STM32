/*
 * ctrl_systime.h
 *
 *  Created on: 2013-9-4
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_SYSTIME_H_
#define CTRL_SYSTIME_H_

#if(SYSCFG_DEBUG)
#define CTRL_SYSTIME_DEBUG				true
#else
#define CTRL_SYSTIME_DEBUG				false
#endif

typedef struct{
	uint16_t		u16Year;
	uint8_t			u8Month;
	uint8_t			u8Date;

	uint8_t			u8Hours;
	uint8_t			u8Minutes;
	uint8_t			u8Seconds;
}CTRL_SYSTIME_DATA;

ErrorStatus ctrl_systime_init(void);
ErrorStatus ctrl_systime_GetSystemTime(CTRL_SYSTIME_DATA *pstSysTime);
ErrorStatus ctrl_systime_SetSystemTime(CTRL_SYSTIME_DATA *pstSysTime);

#endif /* CTRL_SYSTIME_H_ */
