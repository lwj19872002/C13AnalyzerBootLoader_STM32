/*
 * ctrl_leds.h
 *
 *  Created on: 2013-2-26
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_LEDS_H_
#define CTRL_LEDS_H_

#define CTRL_LEDS_MAX_NUM				SS_LEDS_NUM_OF_ALL

#define CTRL_LEDS_BLINK_FREQ_DEF		5		// 默认LED闪烁频率， 单位100ms

#define CTRL_LEDS_BLINK_FREQ_MAX		1000	// 最大闪烁频率

typedef enum{
	e_leds_1	=	0x00000001,
	e_leds_2	=	0x00000002,
	e_leds_3	=	0x00000004,
	e_leds_4	=	0x00000008,
	e_leds_5	=	0x00000010,
	e_leds_6	=	0x00000020,
	e_leds_7	=	0x00000040,
	e_leds_8	=	0x00000080,
	e_leds_9	=	0x00000100,
	e_leds_10	=	0x00000200,
	e_leds_11	=	0x00000400,
	e_leds_12	=	0x00000800,
	e_leds_13	=	0x00001000,
	e_leds_14	=	0x00002000,
	e_leds_15	=	0x00004000,
	e_leds_16	=	0x00008000,
} CTRL_LEDS_NUM;

typedef struct{
	uint32_t u32BlinkLEDs;		// 需要闪烁的LED，每一位代表一个LED
								// --------------------------------------
								//| bit31 | bit30 | ...... | bit1 | bit0 |
								// --------------------------------------
								//| LED32 | LED31 | ...... | LED2 | LED1 |
								// --------------------------------------
	uint32_t u32BlinkFreq;		// 闪烁平率，100ms为单位

} CTRL_LEDS_INFO;

ErrorStatus ctrl_leds_init(void);
void ctrl_leds_thread(void);
void ctrl_leds_on(CTRL_LEDS_NUM eNum);
void ctrl_leds_off(CTRL_LEDS_NUM eNum);
void ctrl_leds_blink_on(CTRL_LEDS_NUM eNums);
void ctrl_leds_blink_off(CTRL_LEDS_NUM eNums);
ErrorStatus ctrl_leds_set_blink_freq(uint32_t u32Freq);

#endif /* CTRL_LEDS_H_ */
