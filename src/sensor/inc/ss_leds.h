/*
 * ss_leds.h
 *
 *  Created on: 2013-4-1
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_LEDS_H_
#define SS_LEDS_H_

// 被控制的LED的总数
#define SS_LEDS_NUM_OF_ALL			16

typedef struct{
	GPIO_TypeDef		*GpioLEDsPort[SS_LEDS_NUM_OF_ALL];
	uint16_t			u16GpioLEDsPin[SS_LEDS_NUM_OF_ALL];

	uint32_t			u32APB2Clk;
	uint32_t			u32APB1Clk;
	uint32_t			u32AHBClk;
} SS_LEDS_CFG;

typedef struct{
	const SS_LEDS_CFG *pPalConfig;
} SS_LEDS_INFO;

ErrorStatus ss_leds_init(const SS_LEDS_CFG *pConfig);

// 输入： uint8_t u8Num ------ 需要被操作的led， 输入范围：0~(SS_LEDS_NUM_OF_ALL-1)
ErrorStatus ss_leds_one_on(uint8_t u8Num);
ErrorStatus ss_leds_one_off(uint8_t u8Num);

#endif /* SS_LEDS_H_ */
