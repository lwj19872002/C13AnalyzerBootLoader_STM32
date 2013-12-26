/*
 * pal_pwm.h
 *
 *  Created on: 2013-3-18
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_PWM_H_
#define PAL_PWM_H_

#define PAL_PWM1
#define PAL_PWM2
#define PAL_PWM3
#define PAL_PWM4
#define PAL_PWM5

typedef enum {
	e_pal_pwm1	=	1,
	e_pal_pwm2	=	2,
	e_pal_pwm3	=	3,
	e_pal_pwm4	=	4,
	e_pal_pwm5	=	5,
} PAL_PWM_NUM;

typedef enum {
	e_pwm_ouput_on	=	1,
	e_pwm_ouput_off	=	2,
} PAL_PWM_OUTPUT_STATUE;

typedef struct {
	PAL_PWM_NUM				ePWMNum;

	TIM_TypeDef				*pTimer;
	uint16_t				u16Channel;

	GPIO_TypeDef			*pGpioPort;
	uint16_t				u16GpioPin;
	uint16_t				u16GpioPinSrc;
	uint8_t					u8GpioPinAF;

	TIM_TimeBaseInitTypeDef	stTimeBaseInit;
	TIM_OCInitTypeDef		stOCInit;

	uint32_t				u32FreqDef;
	uint32_t				u32RatioDef;		// x% * 100, 此处的占空比为扩大100倍后的数值，如：20.1% 应写为2010

	uint32_t				u32APB2Clk;
	uint32_t				u32APB1Clk;
	uint32_t				u32AHBClk;
} PAL_PWM_CFG;

typedef struct {
	TIM_TypeDef				*pTimer;

	uint16_t				u16Channel;

	__IO uint32_t 			*pARR;
	__IO uint32_t 			*pCRR;

	PAL_PWM_OUTPUT_STATUE	eOutputStatue;
	uint32_t				u32Freq;
	uint32_t				u32Ratio;
} PAL_PWM_INFO;

ErrorStatus pal_pwm_init(const PAL_PWM_CFG *pConfig);
void pal_pwm_output_on(PAL_PWM_NUM ePWMNum);
void pal_pwm_output_off(PAL_PWM_NUM ePWMNum);
ErrorStatus pal_pwm_set_freq(PAL_PWM_NUM ePWMNum, uint32_t u32Freq);
ErrorStatus pal_pwm_set_ratio(PAL_PWM_NUM ePWMNum, uint32_t u32Ratio);

#endif /* PAL_PWM_H_ */
