/*
 * pal_uart.h
 *
 *  Created on: 2013-3-1
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_COM_H_
#define PAL_COM_H_

#define PAL_COM1
#define PAL_COM2
#define PAL_COM3
#define PAL_COM4
#define PAL_COM5

#define PAL_COM_BUFFER_LEN				(1 << 9)

#define PAL_COM_INTERRUPT_CLEAR(UARTx, IRQn)	if(USART_GetITStatus(UARTx, IRQn) != RESET) \
												{ \
													USART_ClearITPendingBit(UARTx, IRQn); \
												}

typedef enum{
	e_pal_com1	=	0x01,
	e_pal_com2	=	0x02,
	e_pal_com3	=	0x03,
	e_pal_com4	=	0x04,
	e_pal_com5	=	0x05,
}PAL_COM_NUM;

typedef struct{
	PAL_COM_NUM			eCOMX;

	USART_TypeDef*		pUSART;

	GPIO_TypeDef		*pGpioRxPort;
	uint16_t			u16GpioRxPin;
	uint16_t			u16GpioRxPinSrc;
	uint8_t				u8GpioRxPinAF;

	GPIO_TypeDef		*pGpioTxPort;
	uint16_t			u16GpioTxPin;
	uint16_t			u16GpioTxPinSrc;
	uint8_t				u8GpioTxPinAF;

	USART_InitTypeDef	stCOMTypeDef;

	bool				bRxInt;
	bool				bTxInt;

	NVIC_InitTypeDef	stNVIC;

	uint32_t			u32APB2Clk;
	uint32_t			u32APB1Clk;
	uint32_t			u32AHBClk;
}PAL_COM_CFG;

typedef struct{
	USART_TypeDef*		pUSART;

	uint8_t				u8RxBuffer[PAL_COM_BUFFER_LEN];
	uint8_t				u8TxBuffer[PAL_COM_BUFFER_LEN];
	uint16_t			u16TxBufDex;
	uint16_t			u16TxSendDex;
	uint16_t			u16RxBufDex;
	uint16_t			u16RxReadDex;
}PAL_COM_CTRL_INFO;

ErrorStatus pal_com_init(const PAL_COM_CFG *pConfig);
ErrorStatus pal_com_send_data(PAL_COM_NUM eCOM, uint8_t *pdat, uint16_t u16Len);
ErrorStatus pal_com_get_recdata(PAL_COM_NUM eCOM, uint8_t * pRecDat, uint16_t u16Maxlen, uint16_t *pu16RecLen);

#endif /* PAL_UART_H_ */
