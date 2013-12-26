/*
 * pal_stdio.c
 *
 *  Created on: 2013-3-14
 *      Author: WLD-LuoWenjie
 */

#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef PAL_STDIO

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

const PAL_COM_CFG *g_pStdioUartCFG;

ErrorStatus pal_stdio_init(const PAL_COM_CFG *pConfig)
{
	if(!pConfig)
		return ERROR;

	g_pStdioUartCFG = pConfig;

	pal_com_init(g_pStdioUartCFG);

	return SUCCESS;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(g_pStdioUartCFG->pUSART, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(g_pStdioUartCFG->pUSART, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}

#endif
