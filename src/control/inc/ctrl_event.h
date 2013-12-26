/*
 * ctrl_event.h
 *
 *  Created on: 2013-2-26
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_EVENT_H_
#define CTRL_EVENT_H_

#define CTRL_EVENT_MAX_THREAD			10

typedef void (* P_CTRL_EVENT_THREAD)(void);

typedef enum {
	e_event_1Ms		=	1,
	e_event_10Ms	=	10,
	e_event_20Ms	=	20,
	e_event_50Ms	=	50,
	e_event_100Ms	=	100,
	e_event_1S		=	1000,
	e_event_1Min	=	60000,
} CTRL_EVENT_TIME_DEX;

typedef enum {
	e_event_flag_1Ms	=	0x00000001,
	e_event_flag_10Ms	=	0x00000002,
	e_event_flag_20Ms	=	0x00000004,
	e_event_flag_50Ms	=	0x00000008,
	e_event_flag_100Ms	=	0x00000010,
	e_event_flag_1S		=	0x00000020,
	e_event_flag_1Min	=	0x00000040,
} CTRL_EVENT_FLAG;

typedef struct{
	uint32_t			u1MSThreadTotal;
	P_CTRL_EVENT_THREAD	ap1MSThread[CTRL_EVENT_MAX_THREAD];

	uint32_t			u10MSThreadTotal;
	P_CTRL_EVENT_THREAD	ap10MSThread[CTRL_EVENT_MAX_THREAD];

	uint32_t			u20MSThreadTotal;
	P_CTRL_EVENT_THREAD	ap20MSThread[CTRL_EVENT_MAX_THREAD];

	uint32_t			u50MSThreadTotal;
	P_CTRL_EVENT_THREAD	ap50MSThread[CTRL_EVENT_MAX_THREAD];

	uint32_t			u100MSThreadTotal;
	P_CTRL_EVENT_THREAD	ap100MSThread[CTRL_EVENT_MAX_THREAD];

	uint32_t			u1SThreadTotal;
	P_CTRL_EVENT_THREAD	ap1SThread[CTRL_EVENT_MAX_THREAD];

	uint32_t			u1MinThreadTotal;
	P_CTRL_EVENT_THREAD	ap1MinThread[CTRL_EVENT_MAX_THREAD];
} CTRL_EVENT_INFO;

typedef struct{
	bool		bStart;
	uint32_t	u32DelayMS;
	uint64_t	u64StartTicks;
} CTRL_EVENT_DELAY_INFO;

void ctrl_event_get_ticks(uint64_t *pu64Ticks);

ErrorStatus ctrl_event_init(void);
ErrorStatus ctrl_event_StartSystemTicks(void);
void ctrl_event_handler(void);

void ctrl_event_main_thread(void);

ErrorStatus ctrl_event_add_50ms_thread(P_CTRL_EVENT_THREAD pThread);
ErrorStatus ctrl_event_add_1min_thread(P_CTRL_EVENT_THREAD pThread);
ErrorStatus ctrl_event_add_1s_thread(P_CTRL_EVENT_THREAD pThread);
ErrorStatus ctrl_event_add_10ms_thread(P_CTRL_EVENT_THREAD pThread);
ErrorStatus ctrl_event_add_20ms_thread(P_CTRL_EVENT_THREAD pThread);
ErrorStatus ctrl_event_add_100ms_thread(P_CTRL_EVENT_THREAD pThread);

bool ctrl_event_delay_ms(uint32_t u32MS);
void ctrl_event_delay_clear(void);			// 在每次状态改变时调用。保证在每个线程使用延时函数是都是正常的。

#endif /* CTRL_EVENT_H_ */
