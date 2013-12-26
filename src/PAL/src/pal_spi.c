/*
 * pal_spi.c
 *
 *  Created on: 2013-3-16
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"

#ifdef PAL_SPI
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static volatile PAL_SPI_INFO g_vstSPI1Info;

static volatile PAL_SPI_INFO g_vstSPI2Info;

static volatile PAL_SPI_INFO g_vstSPI3Info;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_spi_getinfo(PAL_SPI_NUM ePortNum, volatile PAL_SPI_INFO **pSPIInfo)
{
	if(ePortNum == e_pal_spi1)
	{
		*pSPIInfo = &g_vstSPI1Info;
		return SUCCESS;
	}
	if(e_pal_spi2 == ePortNum)
	{
		*pSPIInfo = &g_vstSPI2Info;
		return SUCCESS;
	}
	if(e_pal_spi3 == ePortNum)
	{
		*pSPIInfo = &g_vstSPI3Info;
		return SUCCESS;
	}

	return ERROR;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_spi_init(const PAL_SPI_CFG *pConfig)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	volatile PAL_SPI_INFO *pSPIInfo;

	if(!pConfig)
		return ERROR;

	if(!pal_spi_getinfo(pConfig->ePortNum, &pSPIInfo))
		return ERROR;

	pSPIInfo->pSPI			=	pConfig->pSPI;
	pSPIInfo->pGpioCSPort	=	pConfig->pGpioCSPort;
	pSPIInfo->u16GpioCSPin	=	pConfig->u16GpioCSPin;

	if(pConfig->u32APB1Clk)
		RCC_APB1PeriphClockCmd(pConfig->u32APB1Clk, ENABLE);
	if(pConfig->u32APB2Clk)
		RCC_APB2PeriphClockCmd(pConfig->u32APB2Clk, ENABLE);
	if(pConfig->u32AHBClk)
		RCC_AHBPeriphClockCmd(pConfig->u32AHBClk, ENABLE);

	/* Configure SD_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioSCKPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(pConfig->pGpioSCKPort, &GPIO_InitStructure);

	/* Configure SD_SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioMISOPin;
	GPIO_Init(pConfig->pGpioMISOPort, &GPIO_InitStructure);

	/* Configure SD_SPI pins: MOSI */
	GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioMOSIPin;
	GPIO_Init(pConfig->pGpioMOSIPort, &GPIO_InitStructure);

	/* Configure SD_SPI_CS_PIN pin: SD Card CS pin */
	GPIO_InitStructure.GPIO_Pin = pConfig->u16GpioCSPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pConfig->pGpioCSPort, &GPIO_InitStructure);

	GPIO_PinAFConfig(pConfig->pGpioSCKPort, pConfig->u16GpioSCKPinSrc, pConfig->u8GpioSCKPinAF);

	GPIO_PinAFConfig(pConfig->pGpioMISOPort, pConfig->u16GpioMISOPinSrc, pConfig->u8GpioMISOPinAF);

	GPIO_PinAFConfig(pConfig->pGpioMOSIPort, pConfig->u16GpioMOSIPinSrc, pConfig->u8GpioMOSIPinAF);

	/* SD_SPI Config */
	SPI_Init(pSPIInfo->pSPI, (SPI_InitTypeDef *)&pConfig->stSPITypeDef);

	SPI_RxFIFOThresholdConfig(pSPIInfo->pSPI, pConfig->u16RxFIFOThreshold);

	SPI_Cmd(pSPIInfo->pSPI, ENABLE); /* SD_SPI enable */

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus pal_spi_transmit_byte(PAL_SPI_NUM ePortNum, uint8_t u8SendData, uint8_t *pReadData)
{
	volatile PAL_SPI_INFO *pSPIInfo;
	uint32_t u32I;

	if(!pReadData)
		return ERROR;

	if(!pal_spi_getinfo(ePortNum, &pSPIInfo))
		return ERROR;

	/*!< Wait until the transmit buffer is empty */
	u32I = 10000;
	while(u32I--)
	{
		if(SPI_I2S_GetFlagStatus(pSPIInfo->pSPI, SPI_I2S_FLAG_TXE) == SET)
		{
			break;
		}
	}
	if(!u32I)
		return ERROR;
//	while(SPI_I2S_GetFlagStatus(pSPIInfo->pSPI, SPI_I2S_FLAG_TXE) == RESET)
//	{
//	}

	/*!< Send the byte */
	SPI_SendData8(pSPIInfo->pSPI, u8SendData);

	/*!< Wait to receive a byte*/
	u32I = 10000;
	while(u32I--)
	{
		if(SPI_I2S_GetFlagStatus(pSPIInfo->pSPI, SPI_I2S_FLAG_RXNE) == SET)
		{
			break;
		}
	}
	if(!u32I)
		return ERROR;
//	while(SPI_I2S_GetFlagStatus(pSPIInfo->pSPI, SPI_I2S_FLAG_RXNE) == RESET)
//	{
//	}

	/*!< Return the byte read from the SPI bus */
	*pReadData = SPI_ReceiveData8(pSPIInfo->pSPI);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//void pal_spi_cs_hight(PAL_SPI_NUM ePortNum)
//{
//	volatile PAL_SPI_INFO *pSPIInfo;
//	GPIO_TypeDef		*pGpioCSPort;
//	uint16_t			u16GpioCSPin;
//
//	if(!pal_spi_getinfo(ePortNum, &pSPIInfo))
//		return;
//
//	pGpioCSPort = pSPIInfo->pGpioCSPort;
//	u16GpioCSPin = pSPIInfo->u16GpioCSPin;
//
//	GPIO_SetBits(pGpioCSPort, u16GpioCSPin);
//}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//void pal_spi_cs_low(PAL_SPI_NUM ePortNum)
//{
//	volatile PAL_SPI_INFO *pSPIInfo;
//	GPIO_TypeDef		*pGpioCSPort;
//	uint16_t			u16GpioCSPin;
//
//	if(!pal_spi_getinfo(ePortNum, &pSPIInfo))
//		return;
//
//	pGpioCSPort = pSPIInfo->pGpioCSPort;
//	u16GpioCSPin = pSPIInfo->u16GpioCSPin;
//
//	GPIO_ResetBits(pGpioCSPort, u16GpioCSPin);
//}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------



#endif
