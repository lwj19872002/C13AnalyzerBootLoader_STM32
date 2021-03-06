/*
 * SensorCFGV0200.c
 *
 *  Created on: 2013-10-11
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "global.h"
#include "pal.h"
#include "sensor.h"
#include "ssconfig.h"

#if(SYSCFG_HW_CURVER == SYSCFG_HW_V0200)
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef SSCFG_ADC_ADC7739
// RDY Pin ---- PB2
//const SS_ADC_AD7739_CFG g_cstADCAD7739Cfg = {
//		GPIOB,
//		GPIO_Pin_2,
//
//		0x00000000,
//		0x00000000,
//		RCC_AHBPeriph_GPIOB,
//};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef SSCFG_SDCARD
// DEC pin ---- GPF4
const SS_SD_CFG g_cstSDCardCfg = {
	GPIOF,
	GPIO_Pin_4,

	0x00000000,
	0x00000000,
	RCC_AHBPeriph_GPIOF,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef SSCFG_LEDS
//  ע��ʹ������������չIO
//------------------------------------------------------------------
// LED1    LED2    LED3    LED4    LED5    LED6    LED7    LED8
// GPB0    GPB1    GPB2    GPE7    GPE8    GPE9    GPE10   GPE11
//------------------------------------------------------------------
// LED9    LED10   LED11   LED12   LED13   LED14   LED15   LED16
// GPE12   GPE13   GPE14   GPC6    GPC7    GPC8    GPC9    GPA8
//------------------------------------------------------------------
const SS_LEDS_CFG g_cstLEDsCfg = {
	{
		GPIOB, GPIOB, GPIOB, GPIOE, GPIOE,			// LED1 ...... LED5
		GPIOE, GPIOE, GPIOE, GPIOE, GPIOE,			// LED6 ...... LED10
		GPIOE, GPIOC, GPIOC, GPIOC, GPIOC, GPIOA,	// LED11 ...... LED16
	},
	{
		GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_7, GPIO_Pin_8,
		GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13,
		GPIO_Pin_14, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_8,
	},

	0x00000000,
	0x00000000,
	(RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOA),
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef SSCFG_RADIOTUBE
//------------------------------------------------------------------
// RT1    RT2    RT3    RT4    RT5    RT6    RT7    RT8    RT9    RT10
// GPF2   GPC3   GPC2   GPC1   GPC0   GPF10  GPF9   GPC13  GPB9   GPB8
//------------------------------------------------------------------
// RT11   RT12   RT13   RT14   RT15   RT16   RT17   RT18   RT19   RT20
// GPB7   GPB6   GPB5   GPB4   GPD7   GPD6   GPD5   GPD4   GPD3   GPD1
//------------------------------------------------------------------
const SS_RADIOTUBE_CFG g_cstRadiotubeCfg = {
	{
		GPIOF, GPIOC, GPIOC, GPIOC, GPIOC,		// Radiotube1 ...... Radiotube5
		GPIOF, GPIOF, GPIOC, GPIOB, GPIOB,		// Radiotube6 ...... Radiotube10
		GPIOB, GPIOB, GPIOB, GPIOB, GPIOD,		// Radiotube11 ...... Radiotube15
		GPIOD, GPIOD, GPIOD, GPIOD, GPIOD,		// Radiotube16 ...... Radiotube20
	},
	{
		GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_2, GPIO_Pin_1, GPIO_Pin_0,
		GPIO_Pin_10, GPIO_Pin_9, GPIO_Pin_13, GPIO_Pin_9, GPIO_Pin_8,
		GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4, GPIO_Pin_7,
		GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4, GPIO_Pin_3, GPIO_Pin_1,
	},

	0x00000000,
	0x00000000,
	(RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD),
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef SSCFG_LCD_NH012864
// CS Pin    ---- GPC8
// CD Pin    ---- GPC9
// WR Pin    ---- GPC6
// RD Pin    ---- GPC7
// REST Pin  ---- GPA8
// DBus Pins ---- GPD8 ... GPD15
const SS_LCD_NH012864_CFG g_cstLCDNH012864Cfg = {
	GPIOC, GPIO_Pin_8,
	GPIOC, GPIO_Pin_9,
	GPIOC, GPIO_Pin_6,
	GPIOC, GPIO_Pin_7,
	GPIOA, GPIO_Pin_8,
	GPIOD, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, 8,

	0x00000000,
	0x00000000,
	(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOC),
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef SSCFG_KB_3KEY
// COMPin ------------ GPE0
// Key1 -------------- GPE1
// Key2 -------------- GPE2
// Key3 -------------- GPE3
const SS_KB_3KEY_CFG g_cstKB3KeyCfg = {
	GPIOE,

	GPIO_Pin_0,

	GPIO_Pin_1, 1,

	GPIO_Pin_2, 2,

	GPIO_Pin_3, 3,

	0x00000000,
	0x00000000,
	RCC_AHBPeriph_GPIOE,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
