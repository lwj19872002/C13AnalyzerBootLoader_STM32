/*
 * pal_timer.h
 *
 *  Created on: 2013-3-19
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_TIMER_H_
#define PAL_TIMER_H_

#define PAL_TIM1
#define PAL_TIM2
#define PAL_TIM3

typedef enum {
	e_pal_timer1	=	1,
	e_pal_timer2	=	2,
	e_pal_timer3	=	3,
} PAL_TIMER_NUM;

typedef enum {
	e_pal_timerstatue_off	=	0,
	e_pal_timerstatue_on	=	1,
} PAL_TIMER_STATUE;

typedef struct {
	PAL_TIMER_NUM			eTimerNum;

	TIM_TypeDef				*pTimer;
	NVIC_InitTypeDef		stNVIC;
	TIM_TimeBaseInitTypeDef	stTimerInit;

	uint32_t				u32PeriodUS;			// 定时时间，单位为us

	uint32_t				u32APB2Clk;
	uint32_t				u32APB1Clk;
	uint32_t				u32AHBClk;
} PAL_TIMER_CFG;

typedef struct {
	const PAL_TIMER_CFG			*pConfig;

	uint32_t				u32PeriodUS;
	PAL_TIMER_STATUE		eStatue;
} PAL_TIMER_INFO;

typedef void (* PAL_TIMER_INT_HANDLER)(void);


ErrorStatus pal_timer_init(const PAL_TIMER_CFG *pConfig);
ErrorStatus pal_timer_set_userfunc(PAL_TIMER_NUM eTimerNum, PAL_TIMER_INT_HANDLER pUserFunc);
ErrorStatus pal_timer_start(PAL_TIMER_NUM eTimerNum);
ErrorStatus pal_timer_stop(PAL_TIMER_NUM eTimerNum);
ErrorStatus pal_timer_set_period(PAL_TIMER_NUM eTimerNum, uint32_t u32PeriodUS);

void pal_timer_enter_critical(PAL_TIMER_NUM eTimerNum);
void pal_timer_exit_critical(PAL_TIMER_NUM eTimerNum);

#endif /* PAL_TIMER_H_ */
