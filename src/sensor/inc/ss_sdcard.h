/*
 * ss_sdcard.h
 *
 *  Created on: 2013-3-29
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_SDCARD_H_
#define SS_SDCARD_H_

//
//! if enable CRC
//
#define SS_SD_CRC_EN               0

//
//! if enable multi block read function
//
#define SS_SD_READ_MULTI_BLOCK_EN  1

//
//! if enable multi block write function
//
#define SS_SD_WRITE_MULTI_BLOCK_EN 1

//
//! if enable block erase function
//
#define SS_SD_ERASE_BLOCK_EN       1

//
//! SD SPI max(normal) clock freq
//
#define SS_SD_SPI_CLOCK            1000000


//*****************************************************************************
//
//! @}
//
//*****************************************************************************


//*****************************************************************************
//
//! \addtogroup SDCard_Info_Struct SD Card Info Struct
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SDCard_Type SD Card Type
//! @{
//
//*****************************************************************************

#define SS_SD_TYPE_SDSC_V1         0
#define SS_SD_TYPE_SDSC_V2         1
#define SS_SD_TYPE_SDHC_SDXC       2

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//
//! SD Card Block Size, should be 512
//
#define SS_SD_BLOCK_SIZE           512

//
//! SD Card Block bit length, 9 means 512byte
//
#define SS_SD_BLOCK_SIZE_NBITS     9


typedef struct
{
    //
    // Card Type
    //
    unsigned long ulCardType;

    //
    //! Total Block Number(Block size is 512)
    //
    unsigned long ulBlockNumber;

    //
    // Erase Unit(Block Number)
    //
    unsigned long ulEraseUnit;

    unsigned long ulReadTimeout;
    unsigned long ulWriteTimeout;
    unsigned long ulEraseTimeout;

    //
    //! CID register
    //
    unsigned char pucCID[16];
} SS_SDCardDeviceInfo;

typedef struct{
	GPIO_TypeDef		*pGpioDecPort;
	uint16_t			u16GpioDecPin;

	uint32_t			u32APB2Clk;
	uint32_t			u32APB1Clk;
	uint32_t			u32AHBClk;
} SS_SD_CFG;

typedef struct{
	const PAL_SPI_CFG	*pSpiConfig;
	const SS_SD_CFG		*pSDConfig;
} SS_SD_INFO;

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SDCard_Error_Code SD Card Error Code
//! @{
//
//*****************************************************************************

typedef enum{
	//
	// No error occurs
	//
	e_SD_NO_ERR					=	0x00,
	e_SD_ERR_NO_CARD			=	0x01,
	e_SD_ERR_USER_PARAM			=	0x02,
	e_SD_ERR_CARD_PARAM			=	0x03,

	//
	//! voltage is not support
	//
	e_SD_ERR_VOL_NOT_SUSP		=	0x04,
	e_SD_ERR_OVER_CARD_RANGE	=	0x05,
	//
	// SD Card Command error code
	//
	e_SD_ERR_CMD_RESP_TYPE		=	0x10,
	e_SD_ERR_CMD_TIMEOUT		=	0x11,
	e_SD_ERR_CMD_RESP			=	0x12,

	//
	// Data error code
	//
	e_SD_ERR_DATA_CRC16			=	0x20,
	e_SD_ERR_DATA_START_TOK		=	0x21,
	e_SD_ERR_DATA_RESP			=	0x22,

	//
	// Wait error code
	//
	e_SD_ERR_TIMEOUT_WAIT		=	0x30,
	e_SD_ERR_TIMEOUT_READ		=	0x31,
	e_SD_ERR_TIMEOUT_WRITE		=	0x32,
	e_SD_ERR_TIMEOUT_ERASE		=	0x33,
	e_SD_ERR_TIMEOUT_WAIT_IDLE	=	0x34,

	//
	// Write error code
	//
	e_SD_ERR_WRITE_BLK			=	0x40,
	e_SD_ERR_WRITE_BLK_NUMS		=	0x41,
	e_SD_ERR_WRITE_PROTECT		=	0x42,

	// Hard error
	e_SD_ERR_HardWare			=	0x50,
} SS_SD_ERROR;

//*****************************************************************************
//
//! @}
//
//*****************************************************************************
//*****************************************************************************
//
// SD Card Hardware Abstraction Layer
// {
//
//*****************************************************************************
//
//! SPI CS Pin -> 0
//
__STATIC_INLINE void SS_SDSPICSAssert(SS_SD_INFO *pConfig)
{
	pal_spi_cs_low(pConfig->pSpiConfig->ePortNum);
}

//
//! SPI CS Pin -> 1
//
__STATIC_INLINE void SS_SDSPICSDeAssert(SS_SD_INFO *pConfig)
{
	pal_spi_cs_hight(pConfig->pSpiConfig->ePortNum);
}

//
//! SPI Write/Send a byte(dump read)
//
__STATIC_INLINE SS_SD_ERROR SS_SDSPIByteWrite(SS_SD_INFO *pConfig, unsigned char ucSendByte)
{
	unsigned char ucReadByte;
	ErrorStatus	eRet;

	if(!pConfig)
		return e_SD_ERR_USER_PARAM;

	eRet = pal_spi_transmit_byte(pConfig->pSpiConfig->ePortNum, ucSendByte, &ucReadByte);

	if(!eRet)
		return e_SD_ERR_DATA_RESP;
	else
		return e_SD_ERR_NO_CARD;
}

//
//! SPI Read/Receive a byte
//
__STATIC_INLINE SS_SD_ERROR SS_SDSPIByteRead(SS_SD_INFO *pConfig, unsigned char *pucReadByte)
{
	ErrorStatus	eRet;

	if(!pConfig)
		return e_SD_ERR_USER_PARAM;

	eRet = pal_spi_transmit_byte(pConfig->pSpiConfig->ePortNum, 0xFF, pucReadByte);

	if(!eRet)
		return e_SD_ERR_DATA_RESP;
	else
		return e_SD_ERR_NO_CARD;
}

__STATIC_INLINE SS_SD_ERROR SS_SDSPIClkToMax(void)
{
	return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \addtogroup SDCard_API SD Card API
//!
//! @{
//
//*****************************************************************************
extern SS_SD_ERROR SS_SDInit(const SS_SD_CFG *pSDConfig);
extern SS_SD_ERROR SS_SDBlockRead(unsigned char *pucDestBuf,
                                 unsigned long ulBlockIndex);
extern SS_SD_ERROR SS_SDBlockWrite(const unsigned char *pucSrcBuf,
                                  unsigned long ulBlockIndex);
#if SS_SD_READ_MULTI_BLOCK_EN
extern SS_SD_ERROR SS_SDMultiBlockRead(unsigned char *pucDestBuf,
                                      unsigned long ulStartBlockIndex,
                                      unsigned long ulRdBlockNumber);
#endif

#if SS_SD_WRITE_MULTI_BLOCK_EN
extern SS_SD_ERROR SS_SDMultiBlockWrite(const unsigned char* pucSrcBuf,
                                       unsigned long ulStartBlockIndex,
                                       unsigned long ulWrBlockNumber);
#endif

#if SS_SD_ERASE_BLOCK_EN
extern SS_SD_ERROR SS_SDBlockErase(unsigned long ulStartBlockIndex,
                                  unsigned long ulBlockNumber);
#endif

extern const SS_SDCardDeviceInfo* SS_SDCardInfoGet(void);

extern bool SS_SDCardIsIn(SS_SD_INFO *pConfig);

#endif /* SS_SDCARD_H_ */
