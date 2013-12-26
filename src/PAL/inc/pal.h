/*
 * pal.h
 *
 *  Created on: 2013-2-26
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_H_
#define PAL_H_

//-------------------------------------PAL------------------------------------------
#define PAL_GPIO
#define PAL_SYSTICK
#define PAL_STDIO
#define PAL_COM
//#define PAL_SPI
//#define PAL_PWM
//#define PAL_TIMER
//#define PAL_GPIOEXTI
#define PAL_RTC

#include "stm32f30x_it.h"
#include "pal_systick.h"
#include "pal_gpio.h"
#include "pal_com.h"
#include "pal_stdio.h"
#include "pal_spi.h"
#include "pal_pwm.h"
#include "pal_timer.h"
#include "pal_gpioexti.h"
#include "pal_rtc.h"

__STATIC_INLINE void pal_disable_all_interrupt(void)
{
//	SysTick_Disable_Int();
//	__disable_interrupt();
//	__disable_fiq();
}

__STATIC_INLINE void pal_enable_all_interrupt(void)
{
//	SysTick_Enable_Int();
//	__enable_interrupt();
//	__enable_fiq();
}

#endif /* PAL_H_ */
