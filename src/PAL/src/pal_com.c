/*
 * pal_com.c
 *
 *  Created on: 2013-3-1
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"

#ifdef PAL_COM
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef	PAL_COM1
static volatile PAL_COM_CTRL_INFO g_svstCOM1Info;
#endif
#ifdef	PAL_COM2
static volatile PAL_COM_CTRL_INFO g_svstCOM2Info;
#endif
#ifdef	PAL_COM3
static volatile PAL_COM_CTRL_INFO g_svstCOM3Info;
#endif
#ifdef	PAL_COM4
static volatile PAL_COM_CTRL_INFO g_svstCOM4Info;
#endif
#ifdef	PAL_COM5
static volatile PAL_COM_CTRL_INFO g_svstCOM5Info;
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void pal_com_enter_critical(PAL_COM_NUM eCOM);
static void pal_com_exit_critical(PAL_COM_NUM eCOM);
static ErrorStatus pal_com_getinfo(PAL_COM_NUM eNum, volatile PAL_COM_CTRL_INFO **pCtrlInfo);
static ErrorStatus pal_com_getinfo_low(USART_TypeDef *pUSARTx, volatile PAL_COM_CTRL_INFO **pCtrlInfo);
static ErrorStatus pal_com_getIRQn(USART_TypeDef *pUSARTx, IRQn_Type *pIRQn);

static void pal_com_start_tx(USART_TypeDef *pUSARTx);
static ErrorStatus pal_com_send_data_proc(USART_TypeDef *pUSARTx, uint32_t *redat);
static ErrorStatus pal_com_recive_data_proc(USART_TypeDef *pUSARTx, uint32_t *pDat);

//static ErrorStatus pal_com1_recive_data_proc(uint32_t *pDat);
//static ErrorStatus pal_com1_send_data_proc(uint32_t *redat);

//static ErrorStatus pal_com2_recive_data_proc(uint32_t *pDat);
//static ErrorStatus pal_com2_send_data_proc(uint32_t *redat);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void pal_com_enter_critical(PAL_COM_NUM eCOM)
{
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	IRQn_Type IRQn;

	if(!pal_com_getinfo(eCOM, &pCtrlInfo))
		return;

	if(!pal_com_getIRQn(pCtrlInfo->pUSART, &IRQn))
		return;

	NVIC_DisableIRQ(IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void pal_com_exit_critical(PAL_COM_NUM eCOM)
{
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	IRQn_Type IRQn;

	if(!pal_com_getinfo(eCOM, &pCtrlInfo))
		return;

	if(!pal_com_getIRQn(pCtrlInfo->pUSART, &IRQn))
		return;

	NVIC_EnableIRQ(IRQn);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus pal_com_getinfo(PAL_COM_NUM eNum, volatile PAL_COM_CTRL_INFO **pCtrlInfo)
{
	ErrorStatus	eRet;

	switch(eNum)
	{
#ifdef PAL_COM1
	case e_pal_com1:
		*pCtrlInfo = &g_svstCOM1Info;
		eRet = SUCCESS;
		break;
#endif
#ifdef PAL_COM2
	case e_pal_com2:
		*pCtrlInfo = &g_svstCOM2Info;
		eRet = SUCCESS;
		break;
#endif
#ifdef PAL_COM3
	case e_pal_com3:
		*pCtrlInfo = &g_svstCOM3Info;
		eRet = SUCCESS;
		break;
#endif
#ifdef PAL_COM4
	case e_pal_com4:
		*pCtrlInfo = &g_svstCOM4Info;
		eRet = SUCCESS;
		break;
#endif
#ifdef PAL_COM5
	case e_pal_com5:
		*pCtrlInfo = &g_svstCOM5Info;
		eRet = SUCCESS;
		break;
#endif
	default:
		eRet = ERROR;
		break;
	}
	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus pal_com_getinfo_low(USART_TypeDef *pUSARTx, volatile PAL_COM_CTRL_INFO **pCtrlInfo)
{
#ifdef PAL_COM1
	if(pUSARTx == g_svstCOM1Info.pUSART)
	{
		*pCtrlInfo = &g_svstCOM1Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_COM2
	if(pUSARTx == g_svstCOM2Info.pUSART)
	{
		*pCtrlInfo = &g_svstCOM2Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_COM3
	if(pUSARTx == g_svstCOM3Info.pUSART)
	{
		*pCtrlInfo = &g_svstCOM3Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_COM4
	if(pUSARTx == g_svstCOM4Info.pUSART)
	{
		*pCtrlInfo = &g_svstCOM4Info;
		return SUCCESS;
	}
#endif
#ifdef PAL_COM5
	if(pUSARTx == g_svstCOM5Info.pUSART)
	{
		*pCtrlInfo = &g_svstCOM5Info;
		return SUCCESS;
	}
#endif
	return ERROR;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus pal_com_getIRQn(USART_TypeDef *pUSARTx, IRQn_Type *pIRQn)
{
	ErrorStatus eRet;

	if(pUSARTx == USART1)
	{
		*pIRQn = USART1_IRQn;
		eRet = SUCCESS;
	}
	else if(pUSARTx == USART2)
	{
		*pIRQn = USART2_IRQn;
		eRet = SUCCESS;
	}
	else if(pUSARTx == USART3)
	{
		*pIRQn = USART3_IRQn;
		eRet = SUCCESS;
	}
	else if(pUSARTx == UART4)
	{
		*pIRQn = UART4_IRQn;
		eRet = SUCCESS;
	}
	else if(pUSARTx == UART5)
	{
		*pIRQn = UART5_IRQn;
		eRet = SUCCESS;
	}
	else
	{
		eRet = ERROR;
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_com_init(const PAL_COM_CFG *pConfig)
{
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	GPIO_InitTypeDef GPIO_InitStructure;
//	IRQn_Type IRQn;

	if(!pal_com_getinfo(pConfig->eCOMX, &pCtrlInfo))
	{
		return ERROR;
	}

	memset((void *)pCtrlInfo, 0x00, sizeof(PAL_COM_CTRL_INFO));

	pCtrlInfo->pUSART	=	pConfig->pUSART;

	if(pConfig->u32APB1Clk)
		RCC_APB1PeriphClockCmd(pConfig->u32APB1Clk, ENABLE);
	if(pConfig->u32APB2Clk)
		RCC_APB2PeriphClockCmd(pConfig->u32APB2Clk, ENABLE);
	if(pConfig->u32AHBClk)
		RCC_AHBPeriphClockCmd(pConfig->u32AHBClk, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);

	/* Configure USARTy Rx as alternate function push-pull */
	GPIO_PinAFConfig(pConfig->pGpioRxPort, pConfig->u16GpioRxPinSrc, pConfig->u8GpioRxPinAF);
	GPIO_InitStructure.GPIO_Pin		= pConfig->u16GpioRxPin;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(pConfig->pGpioRxPort, &GPIO_InitStructure);

	/* Configure USARTy Tx as alternate function push-pull */
	GPIO_PinAFConfig(pConfig->pGpioTxPort, pConfig->u16GpioTxPinSrc, pConfig->u8GpioTxPinAF);
	GPIO_InitStructure.GPIO_Pin		= pConfig->u16GpioTxPin;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(pConfig->pGpioTxPort, &GPIO_InitStructure);

	/* Configure USARTy */
	USART_DeInit(pConfig->pUSART);
	USART_Init((USART_TypeDef *)pCtrlInfo->pUSART, (USART_InitTypeDef *)&pConfig->stCOMTypeDef);

	/* Enable USARTy Receive and Transmit interrupts */
	if(pConfig->bRxInt)
		USART_ITConfig(pCtrlInfo->pUSART, USART_IT_RXNE, ENABLE);
	if(pConfig->bTxInt)
		USART_ITConfig(pCtrlInfo->pUSART, USART_IT_TC, ENABLE);

	USART_OverrunDetectionConfig(pCtrlInfo->pUSART, USART_OVRDetection_Disable);

	/* Enable the USARTy */
	USART_Cmd(pCtrlInfo->pUSART, ENABLE);

	//  /* Set the USARTy prescaler */
	//  USART_SetPrescaler(BOARD_TEST_UART_PORT, 0x1);
	//  /* Configure the USARTy IrDA mode */
	//  USART_IrDAConfig(BOARD_TEST_UART_PORT, USART_IrDAMode_Normal);
	//
	//  /* Enable the USARTy IrDA mode */
	//  USART_IrDACmd(BOARD_TEST_UART_PORT, ENABLE);
	if(pConfig->bRxInt || pConfig->bTxInt)
	{
		NVIC_Init((NVIC_InitTypeDef *)&pConfig->stNVIC);
//		pal_com_getIRQn(pCtrlInfo->pUSART, &IRQn);
//		NVIC_EnableIRQ(IRQn);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PAL_COM1
void USART1_IRQHandler(void)
{
	uint32_t ui32Temp;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		ui32Temp = USART_ReceiveData(USART1);

		pal_com_recive_data_proc(USART1, &ui32Temp);
	}

	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);

		if(pal_com_send_data_proc(USART1, &ui32Temp) == SUCCESS)
		{
			USART_SendData(USART1, ui32Temp);
		}
	}

}
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PAL_COM2
void USART2_IRQHandler(void)
{
	uint32_t ui32Temp;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		ui32Temp = USART_ReceiveData(USART2);

		pal_com_recive_data_proc(USART2, &ui32Temp);
	}

	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);

		if(pal_com_send_data_proc(USART2, &ui32Temp) == SUCCESS)
		{
			USART_SendData(USART2, ui32Temp);
		}
	}

}
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PAL_COM3
void USART3_IRQHandler(void)
{
	uint32_t ui32Temp;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		ui32Temp = USART_ReceiveData(USART3);

		pal_com_recive_data_proc(USART3, &ui32Temp);
	}

	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);

		if(pal_com_send_data_proc(USART3, &ui32Temp) == SUCCESS)
		{
			USART_SendData(USART3, ui32Temp);
		}
	}
}
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PAL_COM4
void UART4_IRQHandler(void)
{
	uint32_t ui32Temp;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		ui32Temp = USART_ReceiveData(UART4);

		pal_com_recive_data_proc(UART4, &ui32Temp);
	}

	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);

		if(pal_com_send_data_proc(UART4, &ui32Temp) == SUCCESS)
		{
			USART_SendData(UART4, ui32Temp);
		}
	}
}
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#ifdef PAL_COM5
void UART5_IRQHandler(void)
{
	uint32_t ui32Temp;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		ui32Temp = USART_ReceiveData(UART5);
		pal_com_recive_data_proc(UART5, &ui32Temp);
	}

	if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_TC);

		if(pal_com_send_data_proc(UART5, &ui32Temp) == SUCCESS)
		{
			USART_SendData(UART5, ui32Temp);
		}
	}
}
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// Only be called by the interrupt handler of UART transfer
static ErrorStatus pal_com_send_data_proc(USART_TypeDef *pUSARTx, uint32_t *redat)
{
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	ErrorStatus ret;
	uint16_t u16TxSendDex;
	uint16_t u16TxBufDex;

	if(!pal_com_getinfo_low(pUSARTx, &pCtrlInfo))
		return ERROR;

	u16TxSendDex = pCtrlInfo->u16TxSendDex;
	u16TxBufDex = pCtrlInfo->u16TxBufDex;

    if(!UTIL_FIFO_IS_EMPTY(u16TxSendDex, u16TxBufDex))
    {
        *redat = pCtrlInfo->u8TxBuffer[pCtrlInfo->u16TxSendDex];
        UTIL_FIFO_DEX_INC(pCtrlInfo->u16TxSendDex, PAL_COM_BUFFER_LEN);
        ret = SUCCESS;
    }
	else
	{
		ret = ERROR;
	}

    return ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// This function will be called when user write some
// data to the tx[],and need to send out
static void pal_com_start_tx(USART_TypeDef *pUSARTx)
{
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	uint16_t u16TxSendDex;
	uint16_t u16TxBufDex;

	if(!pal_com_getinfo_low(pUSARTx, &pCtrlInfo))
		return ;

	u16TxSendDex = pCtrlInfo->u16TxSendDex;
	u16TxBufDex = pCtrlInfo->u16TxBufDex;

    if(!UTIL_FIFO_IS_EMPTY(u16TxSendDex, u16TxBufDex))
    {
        USART_SendData(pUSARTx, pCtrlInfo->u8TxBuffer[pCtrlInfo->u16TxSendDex]);
        UTIL_FIFO_DEX_INC(pCtrlInfo->u16TxSendDex, PAL_COM_BUFFER_LEN);
    }
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus pal_com_recive_data_proc(USART_TypeDef *pUSARTx, uint32_t *pDat)
{
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	uint16_t u16RxReadDex;
	uint16_t u16RxBufDex;

	if(!pal_com_getinfo_low(pUSARTx, &pCtrlInfo))
		return ERROR;

	u16RxReadDex = pCtrlInfo->u16RxReadDex;
	u16RxBufDex = pCtrlInfo->u16RxBufDex;

    if(UTIL_FIFO_REMAIN(u16RxReadDex, u16RxBufDex, PAL_COM_BUFFER_LEN) > 0)
    {
    	pCtrlInfo->u8RxBuffer[pCtrlInfo->u16RxBufDex] = (*pDat) & 0xFF;
        UTIL_FIFO_DEX_INC(pCtrlInfo->u16RxBufDex, PAL_COM_BUFFER_LEN);
    }

    return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_com_send_data(PAL_COM_NUM eCOM, uint8_t *pdat, uint16_t u16Len)
{
	uint16_t dex;
	bool bStartup;
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	uint16_t u16TxSendDex;
	uint16_t u16TxBufDex;

	if(!pal_com_getinfo(eCOM, &pCtrlInfo))
		return ERROR;

	if(!pdat)
		return ERROR;

	pal_com_enter_critical(eCOM);
	u16TxSendDex = pCtrlInfo->u16TxSendDex;
	u16TxBufDex = pCtrlInfo->u16TxBufDex;

	if(u16Len > UTIL_FIFO_REMAIN(u16TxSendDex, u16TxBufDex, PAL_COM_BUFFER_LEN))
	{
		pal_com_exit_critical(eCOM);
		return ERROR;
	}

	bStartup = false;

	if(UTIL_FIFO_IS_EMPTY(u16TxSendDex, u16TxBufDex))
	{
		bStartup = true;
	}

	for(dex=0; dex<u16Len; dex++)
	{
		pCtrlInfo->u8TxBuffer[pCtrlInfo->u16TxBufDex] = *pdat;
		pdat++;
		UTIL_FIFO_DEX_INC(pCtrlInfo->u16TxBufDex, PAL_COM_BUFFER_LEN);
	}

	if(bStartup)
	{
		pal_com_start_tx(pCtrlInfo->pUSART);
	}

	pal_com_exit_critical(eCOM);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_com_get_recdata(PAL_COM_NUM eCOM, uint8_t * pRecDat, uint16_t u16Maxlen, uint16_t *pu16RecLen)
{
	uint16_t u16Len;
	uint16_t u16Dex;
	uint16_t u16LenToRead;
	ErrorStatus ret;
	volatile PAL_COM_CTRL_INFO *pCtrlInfo;
	uint16_t u16RxReadDex;
	uint16_t u16RxBufDex;

	if(!pRecDat)
		return ERROR;

	if(u16Maxlen <= 0)
		return ERROR;

	if(!pu16RecLen)
		return ERROR;

	if(!pal_com_getinfo(eCOM, &pCtrlInfo))
		return ERROR;

	pal_com_enter_critical(eCOM);

	u16RxReadDex = pCtrlInfo->u16RxReadDex;
	u16RxBufDex = pCtrlInfo->u16RxBufDex;

	u16Len = UTIL_FIFO_COUNT(u16RxReadDex, u16RxBufDex, PAL_COM_BUFFER_LEN);

	if(u16Len > 0)
	{
		u16LenToRead = (u16Len > u16Maxlen)?u16Maxlen:u16Len;

		for(u16Dex=0; u16Dex<u16LenToRead; u16Dex++)
		{
			*(pRecDat + u16Dex) = pCtrlInfo->u8RxBuffer[pCtrlInfo->u16RxReadDex];
			UTIL_FIFO_DEX_INC(pCtrlInfo->u16RxReadDex, PAL_COM_BUFFER_LEN);
		}

		*pu16RecLen = u16LenToRead;

		ret = SUCCESS;
	}
	else
	{
		ret = ERROR;
	}

	pal_com_exit_critical(eCOM);

	return ret;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
