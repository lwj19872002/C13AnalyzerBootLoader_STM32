/*
 * ctrl_linker.h
 *
 *  Created on: 2013-4-16
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_LINKER_H_
#define CTRL_LINKER_H_

#if(SYSCFG_DEBUG)
#define CTRL_LINKER_DEBUG		true
#else
#define CTRL_LINKER_DEBUG		false
#endif

typedef enum{
	e_linker_P1		=	0x01,
	e_linker_P2		=	0x02,
	e_linker_P3		=	0x03,
} CTRL_LINKER_PORT;

typedef struct{
	bool				bInitOK;
	const PAL_COM_CFG	*pPalCOMCfg;
} CTRL_LINKER_INFO;

ErrorStatus ctrl_linker_init(void);
ErrorStatus ctrl_linker_send(CTRL_LINKER_PORT ePort, uint8_t *pdat, uint16_t u16Len);
ErrorStatus ctrl_linker_read(CTRL_LINKER_PORT ePort, uint8_t * pRecDat, uint16_t u16Maxlen, uint16_t *pu16RecLen);

#endif /* CTRL_LINKER_H_ */
