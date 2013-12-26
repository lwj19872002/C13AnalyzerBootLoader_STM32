/*
 * ss_buzzer.h
 *
 *  Created on: 2013-4-8
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_BUZZER_H_
#define SS_BUZZER_H_

typedef enum{
	e_buzzer_tone_1		=	0x00,
	e_buzzer_tone_2		=	0x01,
	e_buzzer_tone_3		=	0x02,
	e_buzzer_tone_4		=	0x03,
	e_buzzer_tone_5		=	0x04,
	e_buzzer_tone_6		=	0x05,
} SS_BUZZER_TONE;

typedef struct{
	const PAL_PWM_CFG		*pPalPWMCfg;
} SS_BUZZER_INFO;

ErrorStatus ss_buzzer_init(void);
ErrorStatus ss_buzzer_on(void);
ErrorStatus ss_buzzer_off(void);
ErrorStatus ss_buzzer_set_tone(SS_BUZZER_TONE eTone);

#endif /* SS_BUZZER_H_ */
