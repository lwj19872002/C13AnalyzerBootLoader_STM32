/*
 * pal_gpio.c
 *
 *  Created on: 2013-2-27
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "globaldata.h"

#ifdef PAL_GPIO
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_gpio_init(const PAL_GPIO_INIT_ST *pConfig)
{

	if(!pConfig)
		return ERROR;

	if(pConfig->RCC_AHB)
		RCC_AHBPeriphClockCmd(pConfig->RCC_AHB, ENABLE);
	if(pConfig->RCC_APB1)
		RCC_APB1PeriphClockCmd(pConfig->RCC_APB1, ENABLE);
	if(pConfig->RCC_APB2)
		RCC_APB2PeriphClockCmd(pConfig->RCC_APB2, ENABLE);

	GPIO_Init(pConfig->pPort, (GPIO_InitTypeDef *)&pConfig->PinSt);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_gpio_setpin(const PAL_GPIO_INIT_ST *pConfig)
{
	if(!pConfig)
		return ERROR;

	GPIO_SetBits(pConfig->pPort, pConfig->PinSt.GPIO_Pin);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_gpio_resetpin(const PAL_GPIO_INIT_ST *pConfig)
{
	if(!pConfig)
		return ERROR;

	GPIO_ResetBits(pConfig->pPort, pConfig->PinSt.GPIO_Pin);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
