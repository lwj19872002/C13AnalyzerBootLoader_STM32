/*
 * pal_systick.h
 *
 *  Created on: 2013-2-26
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_SYSTICK_H_
#define PAL_SYSTICK_H_

//#define PAL_SYSTICK_ENTER_CRITICAL() do{ SysTick_Disable_Int(); }while(0)
//#define PAL_SYSTICK_EXIT_CRITICAL() do{ SysTick_Enable_Int(); }while(0)

#define PAL_SYSTICK_ENTER_CRITICAL() do{ __disable_irq(); }while(0)
#define PAL_SYSTICK_EXIT_CRITICAL() do{ __enable_irq(); }while(0)

typedef void (* PAL_SYSTICK_APP_HANDLER)(void);

void SysTick_Handler(void);

ErrorStatus pal_systick_init(uint32_t u32Ms);

ErrorStatus pal_systick_set_app_handler(PAL_SYSTICK_APP_HANDLER pAppFunc);

#endif /* PAL_SYSTICK_H_ */

