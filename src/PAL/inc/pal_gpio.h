/*
 * pal_gpio.h
 *
 *  Created on: 2013-2-27
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_GPIO_H_
#define PAL_GPIO_H_

typedef struct{
	GPIO_TypeDef		*pPort;
	GPIO_InitTypeDef	PinSt;
	uint32_t			RCC_AHB;
	uint32_t			RCC_APB1;
	uint32_t			RCC_APB2;
}PAL_GPIO_INIT_ST;

ErrorStatus pal_gpio_init(const PAL_GPIO_INIT_ST *pConfig);
ErrorStatus pal_gpio_setpin(const PAL_GPIO_INIT_ST *pConfig);
ErrorStatus pal_gpio_resetpin(const PAL_GPIO_INIT_ST *pConfig);

#endif /* PAL_GPIO_H_ */
