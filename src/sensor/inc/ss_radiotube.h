/*
 * ss_radiotube.h
 *
 *  Created on: 2013-4-1
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_RADIOTUBE_H_
#define SS_RADIOTUBE_H_

#define SS_RADIOTUBE_NUM_OF_ALL			20		// ���Ƶ�ŷ�����


typedef struct{
	GPIO_TypeDef		*GpioPort[SS_RADIOTUBE_NUM_OF_ALL];
	uint16_t			u16GpioPin[SS_RADIOTUBE_NUM_OF_ALL];

	uint32_t			u32APB2Clk;
	uint32_t			u32APB1Clk;
	uint32_t			u32AHBClk;
} SS_RADIOTUBE_CFG;

typedef struct{
	const SS_RADIOTUBE_CFG *pPalConfig;
} SS_RADIOTUBE_INFO;


ErrorStatus ss_radiotube_init(const SS_RADIOTUBE_CFG *pConfig);

// ���룺 uint8_t u8Num ------ ��Ҫ�������ĵ�ŷ��� ���뷶Χ��0~(SS_RADIOTUBE_NUM_OF_ALL-1)
ErrorStatus ss_radiotube_on(uint8_t u8Num);
ErrorStatus ss_radiotube_off(uint8_t u8Num);

#endif /* SS_RADIOTUBE_H_ */
