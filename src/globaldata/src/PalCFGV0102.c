/*
 * PalCFGV0300.c
 *
 *  Created on: 2013-10-11
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "global.h"
#include "palconfig.h"

#if(SYSCFG_HW_CURVER == SYSCFG_HW_V0102)

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_INDICATIVELED_GPIO
// IndicateLED ------- PC13
const PAL_GPIO_INIT_ST g_cstPalIndicateLEDGPIOcfg = {
	GPIOC,

	{
		GPIO_Pin_13,
		GPIO_Mode_OUT,
		GPIO_Speed_50MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP,
	},

	RCC_AHBPeriph_GPIOC,
	0x00000000,
	0x00000000,
};
#endif

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_LINKERP1_UART
const PAL_COM_CFG g_cstPalLinkerP1Uart = {
	e_pal_com1,
	USART1,

	GPIOC, GPIO_Pin_5, GPIO_PinSource5, GPIO_AF_7,

	GPIOC, GPIO_Pin_4, GPIO_PinSource4, GPIO_AF_7,

	{
		115200,
		USART_WordLength_8b,
		USART_StopBits_1,
		USART_Parity_No,
		USART_Mode_Rx | USART_Mode_Tx,
		USART_HardwareFlowControl_None,
	},

	true, true,

	{
		USART1_IRQn,
		0,
		2,
		ENABLE,
	},

	RCC_APB2Periph_USART1,
	0x00000000,
	RCC_AHBPeriph_GPIOC,
};
#endif

#ifdef PALCFG_LINKERP2_UART
const PAL_COM_CFG g_cstPalLinkerP2Uart = {
	e_pal_com2,
	USART2,

	GPIOA, GPIO_Pin_3, GPIO_PinSource3, GPIO_AF_7,

	GPIOA, GPIO_Pin_2, GPIO_PinSource2, GPIO_AF_7,

	{
		115200,
		USART_WordLength_8b,
		USART_StopBits_1,
		USART_Parity_No,
		USART_Mode_Rx | USART_Mode_Tx,
		USART_HardwareFlowControl_None,
	},

	true, true,

	{
		USART2_IRQn,
		0,
		3,
		ENABLE,
	},

	0x00000000,
	RCC_APB1Periph_USART2,
	RCC_AHBPeriph_GPIOA,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_STDIO_UART
// stdio ------- USART3
// RX ---------- PB11
// TX ---------- PB10
const PAL_COM_CFG g_cstPalStdioUart = {
	e_pal_com3,
	USART3,

	GPIOB, GPIO_Pin_11, GPIO_PinSource11, GPIO_AF_7,

	GPIOB, GPIO_Pin_10, GPIO_PinSource10, GPIO_AF_7,

	{
		115200,
		USART_WordLength_8b,
		USART_StopBits_1,
		USART_Parity_No,
		USART_Mode_Rx | USART_Mode_Tx,
		USART_HardwareFlowControl_None,
	},

	false, false,

	{
		USART3_IRQn,
		0,
		4,
		DISABLE,
	},

	0x00000000,
	RCC_APB1Periph_USART3,
	RCC_AHBPeriph_GPIOB,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_BUZZER_PWM
// Buzzer ------------- TIM16  ch1
// PA12
const PAL_PWM_CFG g_cstPalBuzzerPWM = {
	e_pal_pwm1,

	TIM16,
	TIM_Channel_1,

	GPIOA, GPIO_Pin_12, GPIO_PinSource12, GPIO_AF_1,

	{
		72000000 / 100000,
		TIM_CounterMode_Up,
		(100000 / 500 ),
		0,
		0,
	},

	{
		TIM_OCMode_PWM2,
		TIM_OutputState_Enable,
		TIM_OutputNState_Enable,
		(500 / 2),
		TIM_OCPolarity_Low,
		TIM_OCNPolarity_High,
		TIM_OCIdleState_Set,
		TIM_OCIdleState_Reset,
	},

	500,
	5000,						// 50%

	RCC_APB2Periph_TIM16,
	0x00000000,
	RCC_AHBPeriph_GPIOA,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_AD7739_SPI
// AD7739 ------ SPI2
// SCK --------- PB13
// MOSI -------- PB15
// MISO -------- PB14
// CS ---------- PB12
const PAL_SPI_CFG g_cstPalAD7739SPI = {
	e_pal_spi1,
	SPI2,

	// SCK
	GPIOB,GPIO_Pin_13,GPIO_PinSource13,GPIO_AF_5,

	// MOSI
	GPIOB,GPIO_Pin_15,GPIO_PinSource15,GPIO_AF_5,

	// MISO
	GPIOB,GPIO_Pin_14,GPIO_PinSource14,GPIO_AF_5,

	// CS
	GPIOB,GPIO_Pin_12,GPIO_PinSource12,GPIO_AF_5,

	{
			SPI_Direction_2Lines_FullDuplex,
			SPI_Mode_Master,
			SPI_DataSize_8b,
			SPI_CPOL_High,
			SPI_CPHA_2Edge,
			SPI_NSS_Soft,
			SPI_BaudRatePrescaler_2,
			SPI_FirstBit_MSB,
			7,
	},

	SPI_RxFIFOThreshold_QF,

	0x00000000,
	RCC_APB1Periph_SPI2,
	RCC_AHBPeriph_GPIOB,
};
#endif

#ifdef PALCFG_AD7739_RDYPIN
// RDY Pin ---- GPF4
const PAL_GPIOEXTI_CFG g_cstPalAD7739RdyPinExti = {
	GPIOF, GPIO_Pin_4, 4,

	EXTI_PortSourceGPIOF,
	EXTI_PinSource4,

	{
		EXTI_Line4,
		EXTI_Mode_Interrupt,
		EXTI_Trigger_Falling,
		ENABLE,
	},

	{
		EXTI4_IRQn,
		0x0F,
		0x0F,
		ENABLE,
	},

	RCC_APB2Periph_SYSCFG,
	0x00000000,
	RCC_AHBPeriph_GPIOF,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_SDCARD_SPI
// SDCard --- SPI1
// SCK ------ PA5
// MOSI ----- PA7
// MISO ----- PA6
// CS ------- PA4
const PAL_SPI_CFG g_cstPalSDCardSPI = {
	e_pal_spi2,
	SPI1,

	// SCK
	GPIOA,GPIO_Pin_5,GPIO_PinSource5,GPIO_AF_5,

	// MOSI
	GPIOA,GPIO_Pin_7,GPIO_PinSource7,GPIO_AF_5,

	// MISO
	GPIOA,GPIO_Pin_6,GPIO_PinSource6,GPIO_AF_5,

	// CS
	GPIOA,GPIO_Pin_4,GPIO_PinSource4,GPIO_AF_5,

	{
			SPI_Direction_2Lines_FullDuplex,
			SPI_Mode_Master,
			SPI_DataSize_8b,
			SPI_CPOL_High,
			SPI_CPHA_2Edge,
			SPI_NSS_Soft,
			SPI_BaudRatePrescaler_4,
			SPI_FirstBit_MSB,
			7,
	},

	SPI_RxFIFOThreshold_QF,

	RCC_APB2Periph_SPI1,
	0x00000000,
	RCC_AHBPeriph_GPIOA,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_SAMPLE_TIMER
// Sampler ---------- timer3
const PAL_TIMER_CFG g_cstSampleTimer = {
	e_pal_timer1,

	TIM3,

	{
		TIM3_IRQn,
		0,
		1,
		DISABLE,
	},

	{
		72000000/7200,
		TIM_CounterMode_Up,
		0xFFFF,
		0,
		0,
	},

	500000, // 500 ms

	0x00000000,
	RCC_APB1Periph_TIM3,
	0x00000000,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_LIGHTSOURCE_PWM
// Light source ---------- PWM Timer2 channel 2
// OUTPUT ---------------- GPA1
const PAL_PWM_CFG g_cstPalLightSourcePWM = {
	e_pal_pwm2,

	TIM2,
	TIM_Channel_2,

	GPIOA,GPIO_Pin_1,GPIO_PinSource1,GPIO_AF_1,

	{
		(72000000 / 40000 ),
		TIM_CounterMode_Up,
		(40000 / PALCFG_LS_FREQ ),
		0,
		0,
	},

	{
		TIM_OCMode_PWM2,
		TIM_OutputState_Enable,
		TIM_OutputNState_Enable,
		(40000 / PALCFG_LS_FREQ * PALCFG_LS_RATIO),
		TIM_OCPolarity_Low,
		TIM_OCNPolarity_High,
		TIM_OCIdleState_Set,
		TIM_OCIdleState_Reset,
	},

	PALCFG_LS_FREQ,
	PALCFG_LS_RATIO * 10000,						// 50%

	0x00000000,
	RCC_APB1Periph_TIM2,
	RCC_AHBPeriph_GPIOA,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_PUMP_GPIO
// Pump Control ------- PA15
const PAL_GPIO_INIT_ST g_cstPalPumpGPIOcfg = {
	GPIOA,

	{
		GPIO_Pin_15,
		GPIO_Mode_OUT,
		GPIO_Speed_50MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP,
	},

	RCC_AHBPeriph_GPIOA,
	0x00000000,
	0x00000000,
};

// Pump Control ---------- PWM Timer8 channel 1
//              ---------- GPA15
const PAL_PWM_CFG g_cstPalPumpPWMcfg = {
	e_pal_pwm3,

	TIM8,
	TIM_Channel_1,

	GPIOA,GPIO_Pin_15,GPIO_PinSource15,GPIO_AF_2,

	{
		(72000000 / 1000000 ),
		TIM_CounterMode_Up,
		(1000000 / 10000 ),
		0,
		0,
	},

	{
		TIM_OCMode_PWM2,
		TIM_OutputState_Enable,
		TIM_OutputNState_Enable,
		((1000000 / 10000 ) * 0.7),
		TIM_OCPolarity_Low,
		TIM_OCNPolarity_High,
		TIM_OCIdleState_Set,
		TIM_OCIdleState_Reset,
	},

	10000,
	7000,						// 50%

	RCC_APB2Periph_TIM8,
	0x00000000,
	RCC_AHBPeriph_GPIOA,
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PALCFG_RTC
const PAL_RTC_CFG g_cstPalRTCCfg={
	{
		RTC_HourFormat_24, 0x7F, 0xFF,
	},

	{
		12, 0, 0, RTC_H12_AM,
	},

	{
		RTC_Weekday_Wednesday, RTC_Month_September, 4, 13,
	},
};
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

#endif
