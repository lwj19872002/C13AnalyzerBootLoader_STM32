/*
 * pal_pwm.c
 *
 *  Created on: 2013-3-18
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"

#ifdef PAL_PWM

static ErrorStatus pal_pwm_getinfo(PAL_PWM_NUM ePWMNum, volatile PAL_PWM_INFO **ppPWMInfo);

#ifdef PAL_PWM1
static volatile PAL_PWM_INFO g_svstPWM1Info;
#endif

#ifdef PAL_PWM2
static volatile PAL_PWM_INFO g_svstPWM2Info;
#endif

#ifdef PAL_PWM3
static volatile PAL_PWM_INFO g_svstPWM3Info;
#endif

#ifdef PAL_PWM4
static volatile PAL_PWM_INFO g_svstPWM4Info;
#endif

#ifdef PAL_PWM5
static volatile PAL_PWM_INFO g_svstPWM5Info;
#endif

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus pal_pwm_getinfo(PAL_PWM_NUM ePWMNum, volatile PAL_PWM_INFO **ppPWMInfo)
{
#ifdef PAL_PWM1
	if(e_pal_pwm1 == ePWMNum)
	{
		*ppPWMInfo = &g_svstPWM1Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_PWM2
	if(e_pal_pwm2 == ePWMNum)
	{
		*ppPWMInfo = &g_svstPWM2Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_PWM3
	if(e_pal_pwm3 == ePWMNum)
	{
		*ppPWMInfo = &g_svstPWM3Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_PWM4
	if(e_pal_pwm4 == ePWMNum)
	{
		*ppPWMInfo = &g_svstPWM4Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_PWM5
	if(e_pal_pwm5 == ePWMNum)
	{
		*ppPWMInfo = &g_svstPWM5Info;
		return SUCCESS;
	}
#endif
	return ERROR;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_pwm_init(const PAL_PWM_CFG *pConfig)
{
	volatile PAL_PWM_INFO *pPWMInfo;
	GPIO_InitTypeDef GPIO_InitStructure;
	ErrorStatus eRet;

	if(!pConfig)
	{
		return ERROR;
	}

	if(!pal_pwm_getinfo(pConfig->ePWMNum, &pPWMInfo))
	{
		return ERROR;
	}

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

	GPIO_StructInit(&GPIO_InitStructure);
	/* GPIO Configuration: alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

	GPIO_Init(pConfig->pGpioPort, &GPIO_InitStructure);

	GPIO_PinAFConfig(pConfig->pGpioPort, pConfig->u16GpioPinSrc, pConfig->u8GpioPinAF);

	TIM_TimeBaseInit(pConfig->pTimer, (TIM_TimeBaseInitTypeDef *)&pConfig->stTimeBaseInit);

	switch(pConfig->u16Channel)
	{
	case TIM_Channel_1:
		if(IS_TIM_LIST1_PERIPH(pConfig->pTimer))
		{
			TIM_OC1Init(pConfig->pTimer, (TIM_OCInitTypeDef *)&pConfig->stOCInit);
			TIM_OC1PreloadConfig(pConfig->pTimer, TIM_OCPreload_Disable);
			pPWMInfo->pCRR = &(pConfig->pTimer->CCR1);
			eRet = SUCCESS;
		}
		else
			eRet = ERROR;
		break;
	case TIM_Channel_2:
		if(IS_TIM_LIST2_PERIPH(pConfig->pTimer))
		{
			TIM_OC2Init(pConfig->pTimer, (TIM_OCInitTypeDef *)&pConfig->stOCInit);
			TIM_OC2PreloadConfig(pConfig->pTimer, TIM_OCPreload_Disable);
			pPWMInfo->pCRR = &(pConfig->pTimer->CCR2);
			eRet = SUCCESS;
		}
		else
			eRet = ERROR;
		break;
	case TIM_Channel_3:
		if(IS_TIM_LIST3_PERIPH(pConfig->pTimer))
		{
			TIM_OC3Init(pConfig->pTimer, (TIM_OCInitTypeDef *)&pConfig->stOCInit);
			TIM_OC3PreloadConfig(pConfig->pTimer, TIM_OCPreload_Disable);
			pPWMInfo->pCRR = &(pConfig->pTimer->CCR3);
			eRet = SUCCESS;
		}
		else
			eRet = ERROR;
		break;
	case TIM_Channel_4:
		if(IS_TIM_LIST3_PERIPH(pConfig->pTimer))
		{
			TIM_OC4Init(pConfig->pTimer, (TIM_OCInitTypeDef *)&pConfig->stOCInit);
			TIM_OC4PreloadConfig(pConfig->pTimer, TIM_OCPreload_Disable);
			pPWMInfo->pCRR = &(pConfig->pTimer->CCR4);
			eRet = SUCCESS;
		}
		else
			eRet = ERROR;
		break;
	default:
		eRet = ERROR;
		break;
	}

	if(!eRet)
		return eRet;

	TIM_ARRPreloadConfig(pConfig->pTimer, ENABLE);

	TIM_Cmd(pConfig->pTimer, ENABLE);

	if(IS_TIM_LIST6_PERIPH(pConfig->pTimer))
	{
		TIM_CtrlPWMOutputs(pConfig->pTimer, ENABLE);
	}

	pPWMInfo->pTimer		=	pConfig->pTimer;
	pPWMInfo->u16Channel	=	pConfig->u16Channel;
	pPWMInfo->eOutputStatue	=	e_pwm_ouput_on;
	pPWMInfo->u32Freq		=	pConfig->u32FreqDef;
	pPWMInfo->u32Ratio		=	pConfig->u32RatioDef;
	pPWMInfo->pARR			=	&(pConfig->pTimer->ARR);

	eRet = SUCCESS;

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void pal_pwm_output_on(PAL_PWM_NUM ePWMNum)
{
	volatile PAL_PWM_INFO *vpPWMInfo;
	uint32_t u32ARR;
	uint32_t u32Prescaler;
	float	fTemp;

	if(!pal_pwm_getinfo(ePWMNum, &vpPWMInfo))
		return;

	if(e_pwm_ouput_on == vpPWMInfo->eOutputStatue)
		return;

	fTemp = vpPWMInfo->u32Ratio / 10000.00;
	u32Prescaler = vpPWMInfo->pTimer->PSC;
	u32ARR = SystemCoreClock / (u32Prescaler + 1) / vpPWMInfo->u32Freq;
	*(vpPWMInfo->pARR) = u32ARR;

	*(vpPWMInfo->pCRR) = (uint32_t)(u32ARR * fTemp);

	vpPWMInfo->eOutputStatue = e_pwm_ouput_on;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void pal_pwm_output_off(PAL_PWM_NUM ePWMNum)
{
	volatile PAL_PWM_INFO *vpPWMInfo;

	if(!pal_pwm_getinfo(ePWMNum, &vpPWMInfo))
		return;

	if(e_pwm_ouput_off == vpPWMInfo->eOutputStatue)
		return;

	*(vpPWMInfo->pCRR) = 0;

	vpPWMInfo->eOutputStatue = e_pwm_ouput_off;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_pwm_set_freq(PAL_PWM_NUM ePWMNum, uint32_t u32Freq)
{
	volatile PAL_PWM_INFO *vpPWMInfo;
	uint32_t u32ARR;
	uint32_t u32Prescaler;
	float	fTemp;

	if(!pal_pwm_getinfo(ePWMNum, &vpPWMInfo))
		return ERROR;

	if(u32Freq > 1000000)
		return ERROR;

	if(u32Freq == vpPWMInfo->u32Freq)
		return SUCCESS;

	vpPWMInfo->u32Freq = u32Freq;

	if(e_pwm_ouput_on == vpPWMInfo->eOutputStatue)
	{
		fTemp = vpPWMInfo->u32Ratio / 10000.00;
		u32Prescaler = vpPWMInfo->pTimer->PSC;
		*(vpPWMInfo->pARR) = SystemCoreClock / (u32Prescaler + 1) / u32Freq;
		u32ARR = *(vpPWMInfo->pARR);
		*(vpPWMInfo->pCRR) = (uint32_t)(u32ARR * fTemp);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_pwm_set_ratio(PAL_PWM_NUM ePWMNum, uint32_t u32Ratio)
{
	volatile PAL_PWM_INFO *vpPWMInfo;
	uint32_t u32CRR;
	float	fTemp;

	if(!pal_pwm_getinfo(ePWMNum, &vpPWMInfo))
		return ERROR;

	if(10000 < u32Ratio)
		return ERROR;

	vpPWMInfo->u32Ratio = u32Ratio;

	if(e_pwm_ouput_on == vpPWMInfo->eOutputStatue)
	{
		fTemp = u32Ratio / 10000.00;
		u32CRR = *(vpPWMInfo->pARR);
		*(vpPWMInfo->pCRR) = (uint32_t)(u32CRR * fTemp);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

#endif
