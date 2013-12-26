/*
 * pal_timer.c
 *
 *  Created on: 2013-3-19
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"


#ifdef PAL_TIMER
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static PAL_TIMER_INT_HANDLER pTim1UserFunc = NULL;
static PAL_TIMER_INT_HANDLER pTim2UserFunc = NULL;
static PAL_TIMER_INT_HANDLER pTim3UserFunc = NULL;

#ifdef PAL_TIM1
static volatile PAL_TIMER_INFO g_svstTimer1Info;
#endif
#ifdef PAL_TIM2
static volatile PAL_TIMER_INFO g_svstTimer2Info;
#endif
#ifdef PAL_TIM3
static volatile PAL_TIMER_INFO g_svstTimer3Info;
#endif

static ErrorStatus pal_timer_getinfo(PAL_TIMER_NUM eTimerNum, volatile PAL_TIMER_INFO **ppTimerInfo);

//extern void ctrl_sample_timer_handler(void);

//extern void ctrl_IDLED_blink(void);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus pal_timer_getinfo(PAL_TIMER_NUM eTimerNum, volatile PAL_TIMER_INFO **ppTimerInfo)
{
#ifdef PAL_TIM1
	if(e_pal_timer1 == eTimerNum)
	{
		*ppTimerInfo = &g_svstTimer1Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_TIM2
	if(e_pal_timer2 == eTimerNum)
	{
		*ppTimerInfo = &g_svstTimer2Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_TIM3
	if(e_pal_timer3 == eTimerNum)
	{
		*ppTimerInfo = &g_svstTimer3Info;
		return SUCCESS;
	}
#endif

	return ERROR;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_timer_init(const PAL_TIMER_CFG *pConfig)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	volatile PAL_TIMER_INFO *pTimerInfo;

	if(!pConfig)
		return ERROR;

	if(!pal_timer_getinfo(pConfig->eTimerNum, &pTimerInfo))
		return ERROR;

	/* clock enable */
	if(pConfig->u32APB2Clk)
	{
		RCC_APB2PeriphClockCmd(pConfig->u32APB2Clk, ENABLE);
	}

	if(pConfig->u32APB1Clk)
	{
		RCC_APB1PeriphClockCmd(pConfig->u32APB1Clk, ENABLE);
	}

	if(pConfig->u32AHBClk)
	{
		RCC_AHBPeriphClockCmd(pConfig->u32AHBClk, ENABLE);
	}

	pTimerInfo->pConfig = pConfig;

	memcpy(&TIM_TimeBaseStructure, &pConfig->stTimerInit, sizeof(TIM_TimeBaseInitTypeDef));

	TIM_TimeBaseStructure.TIM_Period = (uint32_t)((SystemCoreClock / pConfig->stTimerInit.TIM_Prescaler + 1) * (pConfig->u32PeriodUS / 1000000.00));

	TIM_TimeBaseInit(pConfig->pTimer, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(pConfig->pTimer, ENABLE);

	NVIC_Init((NVIC_InitTypeDef *)&pConfig->stNVIC);

	TIM_ITConfig(pConfig->pTimer, TIM_IT_Update, DISABLE);
	NVIC_DisableIRQ((IRQn_Type)pConfig->stNVIC.NVIC_IRQChannel);
	TIM_Cmd(pConfig->pTimer, DISABLE);

	pTimerInfo->eStatue = e_pal_timerstatue_off;
	pTimerInfo->u32PeriodUS = pConfig->u32PeriodUS;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void TIM2_IRQHandler(void)
{
//	__disable_interrupt();

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
//		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if(pTim1UserFunc)
			(*pTim1UserFunc)();

//		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	}

//	__enable_interrupt();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void TIM3_IRQHandler(void)
{
//	__disable_interrupt();

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);

		if(pTim2UserFunc)
			(*pTim2UserFunc)();

		//ctrl_sample_timer_handler();

//		ctrl_IDLED_blink();

		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	}

//	__enable_interrupt();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void TIM4_IRQHandler(void)
{
//	__disable_interrupt();

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
//		TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

		if(pTim3UserFunc)
			(*pTim3UserFunc)();

//		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	}

//	__enable_interrupt();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_timer_set_userfunc(PAL_TIMER_NUM eTimerNum, PAL_TIMER_INT_HANDLER pUserFunc)
{
	volatile PAL_TIMER_INFO *pTimerInfo;

	if(!pUserFunc)
		return ERROR;

	if(!pal_timer_getinfo(eTimerNum, &pTimerInfo))
		return ERROR;

	TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, DISABLE);
	NVIC_DisableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
	TIM_Cmd(pTimerInfo->pConfig->pTimer, DISABLE);

	if(TIM2 == pTimerInfo->pConfig->pTimer)
	{
		pTim1UserFunc = pUserFunc;
	}
	else if(TIM3 == pTimerInfo->pConfig->pTimer)
	{
		pTim2UserFunc = pUserFunc;
	}
	else if(TIM4 == pTimerInfo->pConfig->pTimer)
	{
		pTim3UserFunc = pUserFunc;
	}
	else
	{
		return ERROR;
	}

	if(e_pal_timerstatue_on == pTimerInfo->eStatue)
	{
		TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, ENABLE);
		NVIC_EnableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
		TIM_Cmd(pTimerInfo->pConfig->pTimer, ENABLE);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_timer_start(PAL_TIMER_NUM eTimerNum)
{
	volatile PAL_TIMER_INFO *pTimerInfo;

	if(!pal_timer_getinfo(eTimerNum, &pTimerInfo))
		return ERROR;

//	if(e_pal_timerstatue_on == pTimerInfo->eStatue)
//		return SUCCESS;

	TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
	TIM_Cmd(pTimerInfo->pConfig->pTimer, ENABLE);

	pTimerInfo->eStatue = e_pal_timerstatue_on;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_timer_stop(PAL_TIMER_NUM eTimerNum)
{
	volatile PAL_TIMER_INFO *pTimerInfo;

	if(!pal_timer_getinfo(eTimerNum, &pTimerInfo))
		return ERROR;

//	if(e_pal_timerstatue_off == pTimerInfo->eStatue)
//		return SUCCESS;

	TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, DISABLE);
	NVIC_DisableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
	TIM_Cmd(pTimerInfo->pConfig->pTimer, DISABLE);

	pTimerInfo->eStatue = e_pal_timerstatue_off;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_timer_set_period(PAL_TIMER_NUM eTimerNum, uint32_t u32PeriodUS)
{
	volatile PAL_TIMER_INFO *pTimerInfo;
	uint32_t u32Temp;

	if(!u32PeriodUS)
		return ERROR;

	if(!pal_timer_getinfo(eTimerNum, &pTimerInfo))
		return ERROR;

	TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, DISABLE);
	NVIC_DisableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
	TIM_Cmd(pTimerInfo->pConfig->pTimer, DISABLE);

	TIM_SetCounter(pTimerInfo->pConfig->pTimer, 0);

	pTimerInfo->u32PeriodUS = u32PeriodUS;
	u32Temp = (uint32_t)((SystemCoreClock / (pTimerInfo->pConfig->pTimer->PSC + 1)) * (u32PeriodUS / 1000000.00));

	TIM_SetAutoreload(pTimerInfo->pConfig->pTimer, u32Temp);

	if(e_pal_timerstatue_on == pTimerInfo->eStatue)
	{
		TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, ENABLE);
		NVIC_EnableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
		TIM_Cmd(pTimerInfo->pConfig->pTimer, ENABLE);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void pal_timer_enter_critical(PAL_TIMER_NUM eTimerNum)
{
	volatile PAL_TIMER_INFO *pTimerInfo;

	if(!pal_timer_getinfo(eTimerNum, &pTimerInfo))
		return;

	TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, DISABLE);
	//NVIC_DisableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
	//TIM_Cmd(pTimerInfo->pConfig->pTimer, DISABLE);

	//__disable_interrupt();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void pal_timer_exit_critical(PAL_TIMER_NUM eTimerNum)
{
	volatile PAL_TIMER_INFO *pTimerInfo;

	if(!pal_timer_getinfo(eTimerNum, &pTimerInfo))
		return;

	if(e_pal_timerstatue_on == pTimerInfo->eStatue)
	{
		//NVIC_EnableIRQ((IRQn_Type)pTimerInfo->pConfig->stNVIC.NVIC_IRQChannel);
		TIM_ITConfig(pTimerInfo->pConfig->pTimer, TIM_IT_Update, ENABLE);
		//TIM_Cmd(pTimerInfo->pConfig->pTimer, ENABLE);

		//__enable_interrupt();
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
