/*
 * pal_exti.c
 *
 *  Created on: 2013-4-12
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"

#ifdef PAL_GPIOEXTI
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

static P_PAL_GPIOEXTI_USER_FUN g_pEXTIUserFun[16] = {
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL,
};

#define DONE_EXTI_IRQHandler(x)	do \
								{ \
									if((EXTI_GetITStatus(EXTI_Line##x) != RESET)) \
									{ \
										if(g_pEXTIUserFun[x]) \
											(*g_pEXTIUserFun[x])(); \
										EXTI_ClearITPendingBit(EXTI_Line##x); \
									} \
								} \
								while(0)

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles External lines 0 interrupt request.
  * @param  None
  * @retval None
***/
void EXTI0_IRQHandler(void)
{
	NVIC_DisableIRQ(EXTI0_IRQn);
	DONE_EXTI_IRQHandler(0);
	NVIC_EnableIRQ(EXTI0_IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles External lines 1 interrupt request.
  * @param  None
  * @retval None
***/
void EXTI1_IRQHandler(void)
{
	NVIC_DisableIRQ(EXTI1_IRQn);
	DONE_EXTI_IRQHandler(1);
	NVIC_EnableIRQ(EXTI1_IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles External lines 2 interrupt request.
  * @param  None
  * @retval None
***/
void EXTI2_TS_IRQHandler(void)
{
	NVIC_DisableIRQ(EXTI2_TS_IRQn);
	DONE_EXTI_IRQHandler(2);
	NVIC_EnableIRQ(EXTI2_TS_IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles External lines 3 interrupt request.
  * @param  None
  * @retval None
***/
void EXTI3_IRQHandler(void)
{
	NVIC_DisableIRQ(EXTI3_IRQn);
	DONE_EXTI_IRQHandler(3);
	NVIC_EnableIRQ(EXTI3_IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles External lines 4 interrupt request.
  * @param  None
  * @retval None
***/
void EXTI4_IRQHandler(void)
{
	NVIC_DisableIRQ(EXTI4_IRQn);
	DONE_EXTI_IRQHandler(4);
	NVIC_EnableIRQ(EXTI4_IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles External lines 5 to 9 interrupt request.
  * @param  None
  * @retval None
***/
void EXTI9_5_IRQHandler(void)
{
	NVIC_DisableIRQ(EXTI9_5_IRQn);
	DONE_EXTI_IRQHandler(5);
	DONE_EXTI_IRQHandler(6);
	DONE_EXTI_IRQHandler(7);
	DONE_EXTI_IRQHandler(8);
	DONE_EXTI_IRQHandler(9);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles External lines 10 to 15 interrupt request.
  * @param  None
  * @retval None
***/
void EXTI15_10_IRQHandler(void)
{
	NVIC_DisableIRQ(EXTI15_10_IRQn);
	DONE_EXTI_IRQHandler(10);
	DONE_EXTI_IRQHandler(11);
	DONE_EXTI_IRQHandler(12);
	DONE_EXTI_IRQHandler(13);
	DONE_EXTI_IRQHandler(14);
	DONE_EXTI_IRQHandler(15);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_gpioexti_init(const PAL_GPIOEXTI_CFG *pConfig)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	if(!pConfig)
		return ERROR;

	if(pConfig->u32AHBClk)
		RCC_AHBPeriphClockCmd(pConfig->u32AHBClk, ENABLE);
	if(pConfig->u32APB1Clk)
		RCC_APB1PeriphClockCmd(pConfig->u32APB1Clk, ENABLE);
	if(pConfig->u32APB2Clk)
		RCC_APB2PeriphClockCmd(pConfig->u32APB2Clk, ENABLE);

	/* Configure pin in input pull-down mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioPin;
	GPIO_Init(pConfig->pGpioPort, &GPIO_InitStructure);

	/* Connect EXTI Line to pin */
	SYSCFG_EXTILineConfig(pConfig->u8EXTIPortSource, pConfig->u8EXTIPinSource);

	/* Configure EXTI line */
	EXTI_Init((EXTI_InitTypeDef *)&pConfig->stInitTypeDef);

	/* Enable and set EXTI Interrupt to the lowest priority */
	NVIC_Init((NVIC_InitTypeDef *)&pConfig->stNvicInit);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_gpioexti_set_userfun(const PAL_GPIOEXTI_CFG *pPalCfg, P_PAL_GPIOEXTI_USER_FUN pUserFunction)
{
	if((!pPalCfg) || (!pUserFunction))
		return ERROR;

	g_pEXTIUserFun[pPalCfg->u32ExtiLineNum] = pUserFunction;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------


#endif
