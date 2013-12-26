/*
 * pal_spi.h
 *
 *  Created on: 2013-3-16
 *      Author: WLD-LuoWenjie
 */

#ifndef PAL_SPI_H_
#define PAL_SPI_H_

typedef enum {
	e_pal_spi1	=	1,
	e_pal_spi2	=	2,
	e_pal_spi3	=	3,
} PAL_SPI_NUM;

typedef struct{
	PAL_SPI_NUM			ePortNum;

	SPI_TypeDef*		pSPI;

	GPIO_TypeDef		*pGpioSCKPort;
	uint16_t			u16GpioSCKPin;
	uint16_t			u16GpioSCKPinSrc;
	uint8_t				u8GpioSCKPinAF;

	GPIO_TypeDef		*pGpioMOSIPort;
	uint16_t			u16GpioMOSIPin;
	uint16_t			u16GpioMOSIPinSrc;
	uint8_t				u8GpioMOSIPinAF;

	GPIO_TypeDef		*pGpioMISOPort;
	uint16_t			u16GpioMISOPin;
	uint16_t			u16GpioMISOPinSrc;
	uint8_t				u8GpioMISOPinAF;

	GPIO_TypeDef		*pGpioCSPort;
	uint16_t			u16GpioCSPin;
	uint16_t			u16GpioCSPinSrc;
	uint8_t				u8GpioCSPinAF;

	SPI_InitTypeDef		stSPITypeDef;
	uint16_t			u16RxFIFOThreshold;

	uint32_t			u32APB2Clk;
	uint32_t			u32APB1Clk;
	uint32_t			u32AHBClk;
}PAL_SPI_CFG;

typedef struct{
	SPI_TypeDef		*pSPI;

	GPIO_TypeDef		*pGpioCSPort;
	uint16_t			u16GpioCSPin;
}PAL_SPI_INFO;

ErrorStatus pal_spi_getinfo(PAL_SPI_NUM ePortNum, volatile PAL_SPI_INFO **pSPIInfo);
ErrorStatus pal_spi_init(const PAL_SPI_CFG *pConfig);
ErrorStatus pal_spi_transmit_byte(PAL_SPI_NUM ePortNum, uint8_t u8SendData, uint8_t *pReadData);
//void pal_spi_cs_low(PAL_SPI_NUM ePortNum);
//void pal_spi_cs_hight(PAL_SPI_NUM ePortNum);

__STATIC_INLINE void pal_spi_cs_hight(PAL_SPI_NUM ePortNum)
{
	volatile PAL_SPI_INFO *pSPIInfo;
	GPIO_TypeDef		*pGpioCSPort;
	uint16_t			u16GpioCSPin;

	if(!pal_spi_getinfo(ePortNum, &pSPIInfo))
		return;

	pGpioCSPort = pSPIInfo->pGpioCSPort;
	u16GpioCSPin = pSPIInfo->u16GpioCSPin;

	GPIO_SetBits(pGpioCSPort, u16GpioCSPin);
}

__STATIC_INLINE void pal_spi_cs_low(PAL_SPI_NUM ePortNum)
{
	volatile PAL_SPI_INFO *pSPIInfo;
	GPIO_TypeDef		*pGpioCSPort;
	uint16_t			u16GpioCSPin;

	if(!pal_spi_getinfo(ePortNum, &pSPIInfo))
		return;

	pGpioCSPort = pSPIInfo->pGpioCSPort;
	u16GpioCSPin = pSPIInfo->u16GpioCSPin;

	GPIO_ResetBits(pGpioCSPort, u16GpioCSPin);
}

#endif /* PAL_SPI_H_ */
