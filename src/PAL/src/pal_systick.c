/*
 * pal_systick.c
 *
 *  Created on: 2013-2-26
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "globaldata.h"

#ifdef PAL_SYSTICK
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
PAL_SYSTICK_APP_HANDLER g_pAppHandler = NULL;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
//	__disable_interrupt();

	if(g_pAppHandler)
	{
		g_pAppHandler();
	}

//	__enable_interrupt();
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_systick_init(uint32_t u32Ms)
{
	uint32_t u32Temp;

	if((!u32Ms)||(1000<u32Ms))
	{
		return ERROR;
	}

	u32Temp = SystemCoreClock/1000*u32Ms;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	if(SysTick_Config(u32Temp))
		return ERROR;
	else
		return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_systick_set_app_handler(PAL_SYSTICK_APP_HANDLER pAppFunc)
{
	if(!pAppFunc)
		return ERROR;

	g_pAppHandler = pAppFunc;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
