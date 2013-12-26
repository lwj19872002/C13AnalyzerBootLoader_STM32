/*
 * pal_exti.h
 *
 *  Created on: 2013-4-12
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_GPIOEXTI_H_
#define PAL_GPIOEXTI_H_

typedef void (* P_PAL_GPIOEXTI_USER_FUN)(void);

typedef struct{
	GPIO_TypeDef			*pGpioPort;
	uint16_t				u16GpioPin;
	uint32_t				u32ExtiLineNum;

	uint8_t					u8EXTIPortSource;
	uint8_t					u8EXTIPinSource;

	EXTI_InitTypeDef		stInitTypeDef;

	NVIC_InitTypeDef		stNvicInit;

	uint32_t				u32APB2Clk;
	uint32_t				u32APB1Clk;
	uint32_t				u32AHBClk;
} PAL_GPIOEXTI_CFG;

ErrorStatus pal_gpioexti_init(const PAL_GPIOEXTI_CFG *pConfig);

ErrorStatus pal_gpioexti_set_userfun(const PAL_GPIOEXTI_CFG *pPalCfg, P_PAL_GPIOEXTI_USER_FUN pUserFunction);

#endif /* PAL_EXTI_H_ */
