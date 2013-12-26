/*
 * ss_sdcard.c
 *
 *  Created on: 2013-3-29
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "ss_sd_hw.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_SDCARD

//
// 100ms correspond to SPI clock(unit: 8 clocks)
//
#define SS_SD_READ_TIMEOUT_100MS   100 * SS_SD_SPI_CLOCK / 1000 / 8

//
// 250ms correspond to SPI clock(unit: 8 clocks)
//
#define SS_SD_WRITE_TIMEOUT_250MS  250 * SS_SD_SPI_CLOCK / 1000 / 8

//
// ACMD41 timeout value
//
#define SS_SD_IDLE_WAIT_MAX        5000


SS_SD_ERROR SS_SDRegisterRead(unsigned long ulLen, unsigned char* pucRdBuf);
SS_SD_ERROR SS_SDWaitBusy(unsigned char ucWaitType);
unsigned long
SS_SDExtBits(unsigned char *pucBuf, unsigned long ulBufLen,
          unsigned long ulMSB, unsigned long ulLSB);
SS_SD_ERROR SS_SDBlockDataRead(unsigned char *pucRdBuf, unsigned long ulLen);

static SS_SD_INFO g_sstSDInfo;

//
//! SD Card Device Info
//
SS_SDCardDeviceInfo g_sSDCardInfo =
{
    0,
    0,
    0,
    SS_SD_READ_TIMEOUT_100MS,
    SS_SD_WRITE_TIMEOUT_250MS,
    SS_SD_WRITE_TIMEOUT_250MS,
    {0},
};

//
//! Register CSD TAAC time unit
//! Unit = 0.000000001ns
//
const unsigned long g_pulCSDTAACTimeUnit[8] =
{
//  1ns,        10ns,       100ns       1000ns(1us)
    1000000000, 100000000,  10000000,   1000000,
//  10us        100us,      1ms,        10ms
    100000,     10000,      1000,       100
};


//
//! Register CSD TAAC time value
//
const unsigned char g_pucCSDTAACTimeValue[16] =
{
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

//
// Register CSD R2W factor
//
const unsigned char g_pucCSDR2WFactor[8] =
{
    1, 2, 4, 8, 16, 32, 0, 0
};


//*****************************************************************************
//
// }
//
//*****************************************************************************

//
//! Get some bit-field from CSD register
//
#define SS_SDCSDExtBits(pucCSDBuf, ulMSB, ulLSB)                                 \
        SS_SDExtBits(pucCSDBuf, 16, ulMSB, ulLSB)

#if SS_SD_CRC_EN
//
// CRC Table : X16 + X12 + X5 + 1
//
unsigned short const g_pusCRCTable[256] =
{
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
#endif

#if SS_SD_CRC_EN
//*****************************************************************************
//
//! \brief Get the direction and mode of a pin.
//!
//! \param pucSource is the data to ge CRC16.
//! \param usLen is the data length.
//!
//! \return Returns the CRC16 code.
//
//*****************************************************************************
unsigned short
SS_SDCRC16Get(unsigned char *pucSource, unsigned short usLen)
{
    unsigned short i;
    unsigned short ulResult = 0;

    for(i = 0; i < usLen; i++)
    {
        ulResult = (ulResult << 8) ^
            (unsigned short)g_pusCRCTable[(ulResult >> 8) ^ pucSource[i]];
    }

    return ulResult;
}

//*****************************************************************************
//
//! \brief get the CRC7 of the command of SD card.
//!
//! \param ucCmd is the SD Card command.
//! \param pucParam is the pucParam of command, length is 4 bytes.
//!
//! \return Returns the CRC16 code.
//
//*****************************************************************************
unsigned char
SS_SDCmdByte6Get(unsigned char ucCmd, unsigned char *pucParam)
{
    unsigned char i, j;
    unsigned char ucReg = 0;
    unsigned char pucArray[5];

    pucArray[0] = ucCmd;

    //
    // Re-order the command
    //
    for(i = 1; i < 5; i++)
    {
        pucArray[i] = pucParam[4 - i];
    }

    //
    // Calc the CRC7
    //
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 8; j++)
        {
            ucReg <<= 1;
            ucReg ^= ((((pucArray[i] << j) ^ ucReg) & 0x80) ? 0x9 : 0);
        }
    }

    //
    // CRC7 Result left shift 1, then set the bit 0(E)
    //
    return ((ucReg << 1) & 0x01) ;
}
#endif

//*****************************************************************************
//
//! \brief Initializate the hardware.
//!
//! \param None.
//!
//! This function initializate the hardware, such as the SPI port, re-power the
//! sdcard.
//!
//! \return None.
//
//*****************************************************************************
SS_SD_ERROR SS_SDHardwareInit(const SS_SD_CFG *pSDConfig, const PAL_SPI_CFG *pSPIConfig)
{
	ErrorStatus			eRet;
	GPIO_InitTypeDef	GPIO_InitStructure;

    if((!pSPIConfig)||(!pSDConfig))
    	return e_SD_ERR_USER_PARAM;

    memset(&g_sstSDInfo, 0x00, sizeof(SS_SD_INFO));

    g_sstSDInfo.pSDConfig = pSDConfig;
    g_sstSDInfo.pSpiConfig = pSPIConfig;

    if(pSDConfig->u32APB1Clk)
		RCC_APB1PeriphClockCmd(pSDConfig->u32APB1Clk, ENABLE);
	if(pSDConfig->u32APB2Clk)
		RCC_APB2PeriphClockCmd(pSDConfig->u32APB2Clk, ENABLE);
	if(pSDConfig->u32AHBClk)
		RCC_AHBPeriphClockCmd(pSDConfig->u32AHBClk, ENABLE);

	GPIO_InitStructure.GPIO_Pin = g_sstSDInfo.pSDConfig->u16GpioDecPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(g_sstSDInfo.pSDConfig->pGpioDecPort, &GPIO_InitStructure);
	//
	// Set to 100kHz for initialisation
	//
    eRet = pal_spi_init(pSPIConfig);

    if(!eRet)
    	return e_SD_ERR_HardWare;

    return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \brief Send a command to sdcard and get the response.
//!
//! \param ucCmd is the CMD index.
//! \param pucParam is the CMD parameter, 4 byte
//! \param ucRespType is the response type, such as R1.
//! \param pucResp is the response that command return.
//!
//! \b ucCmd can be SD_CMD1, SD_ACMD41 and so on, more refrence \ref SD_Command.\n
//! The means of \b pucParam is determined by the command, it is 4 bytes length.\n
//! \b ucRespType can be SD_R1, SD_R2 or SD_CMD1_R1, SD_ACMD41_R and so on, more
//! refrence \ref SD_Command and \ref SD_Response_Type. \n
//! \b pucResp is the response values, users can use the resonse to determine if
//! the command execute successful.
//!
//! \return None.
//
//*****************************************************************************
SS_SD_ERROR SS_SDCmdWrite(unsigned char ucCmd, unsigned char *pucParam,
                         unsigned char ucRespType, unsigned char* pucResp)
{
    unsigned char ucTmp;
    int i, iRdLen;

    if((!pucParam)||(!pucResp))
    	return e_SD_ERR_USER_PARAM;

    SS_SDSPICSAssert(&g_sstSDInfo);

    //
    // send command header and word
    //
    SS_SDSPIByteWrite(&g_sstSDInfo, (ucCmd & 0x3F) | 0x40);

    //
    // send parameters
    //
    for(i = 3; i >= 0; i--)
    {
       SS_SDSPIByteWrite(&g_sstSDInfo, pucParam[i]);
    }

    //
    // CRC
    //
#if SS_SD_CRC_EN
    ucTmp = SS_SDCmdByte6Get((ucCmd & 0x3F) | 0x40, pucParam);
    SS_SDSPIByteWrite(&g_sstSDInfo, ucTmp);
#else
    //
    // CRC,only used for the first command
    //
    SS_SDSPIByteWrite(&g_sstSDInfo, 0x95);
#endif

    //
    // Get the response byte length according the response type
    //
    iRdLen = 0;
    switch(ucRespType)
    {
        case SS_SD_R1:
        case SS_SD_R1B:
        {
            iRdLen = 1;
            break;
        }
        case SS_SD_R2:
        {
            iRdLen = 2;
            break;
        }
        case SS_SD_R3:
        case SS_SD_R7:
        {
            iRdLen = 5;
            break;
        }
        default:
        {
            SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
            SS_SDSPICSDeAssert(&g_sstSDInfo);
            return e_SD_ERR_CMD_RESP_TYPE;
            break;
        }
    }

    //
    // Waiting valid response
    //
    for(i = 0; i < SS_SD_CMD_TIMEOUT; i++)
    {
        SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);
        if(!(ucTmp & 0x80))
        {
            break;
        }
    }
    if(i >= SS_SD_CMD_TIMEOUT)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        return e_SD_ERR_CMD_TIMEOUT;
    }

    //
    // Read the response
    //
    for(i = iRdLen - 1; i >= 0; i--)
    {
        pucResp[i] = ucTmp;
        SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);
    }

    SS_SDSPICSDeAssert(&g_sstSDInfo);

    return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \brief Packet the command parameter into a byte buffer.
//!
//! \param ulValue is the parameter value.
//!
//! This function packet the unsigned long parameter value into a 4 byte buffer
//! buffer.
//!
//! \return None.
//
//*****************************************************************************
void SS_SDParamPack(unsigned char *pucParam, unsigned long ulValue)
{
    if(!pucParam)
    	return;

    pucParam[3] = (unsigned char)(ulValue >> 24);
    pucParam[2] = (unsigned char)(ulValue >> 16);
    pucParam[1] = (unsigned char)(ulValue >> 8);
    pucParam[0] = (unsigned char)(ulValue);
}

//*****************************************************************************
//
//! \brief Send a block command to sdcard.
//!
//! \param ucCmd is the block command index.
//! \param ucRespType is the command response type.
//! \param ulParameter is the command parameter.
//!
//! \b ucCmd block command can be SD_CMD24, SD_CMD16. more refrence
//! \ref SD_Command_Block_Read, \ref SD_Command_Block_Write. \n
//! The \b ucRespType is always like this:  SD_CMD24's response type is
//! SD_CMD24_R.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDBlockCommand(unsigned char ucCmd, unsigned char ucRespType,
               unsigned long ulParameter)
{
    unsigned char pucParam[4],ucResp;
    SS_SD_ERROR eRet;

    ulParameter <<= SS_SD_BLOCK_SIZE_NBITS;

    SS_SDParamPack(pucParam, ulParameter);

    eRet = SS_SDCmdWrite(ucCmd, pucParam, ucRespType, &ucResp);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    //
    // Check the R1 response
    //
    if(ucResp != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    return e_SD_NO_ERR;

}

//*****************************************************************************
//
//! \brief Reset the sdcard(CMD0).
//!
//! \param None.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
static SS_SD_ERROR SS_SDCardReset(void)
{
    unsigned char pucParam[4] = {0,0,0,0}, ucResp;

    return (SS_SDCmdWrite(SS_SD_CMD0, pucParam, SS_SD_CMD0_R, &ucResp));
}

//*****************************************************************************
//
//! \brief Determine the SDCard is V1.x or V2.x.(CMD8)
//!
//! \param pbVoltageAccepted is the result of if 3V3 compatible.
//!
//! \return Returns SD_NO_ERR(0) indicate this is an V2.x sdcard, others V1.x or
//! not an sdcard.
//
//*****************************************************************************
static SS_SD_ERROR SS_SDConditionCheck(bool *pbVoltageAccepted)
{
    //
    // 3V3
    //
    unsigned long ulParameter = (0x01 << 8) | 0xAA;
    unsigned char pucParam[4] = {0,0,0,0}, pucResp[5], ucTmp;
    int i;

    if(!pbVoltageAccepted)
    	return e_SD_ERR_USER_PARAM;

    SS_SDParamPack(pucParam, ulParameter);

    SS_SDSPICSAssert(&g_sstSDInfo);

    //
    // Send command header and word
    //
    SS_SDSPIByteWrite(&g_sstSDInfo, (SS_SD_CMD8 & 0x3F) | 0x40);

    //
    // Send parameters
    //
    for(i = 3; i >= 0; i--)
    {
       SS_SDSPIByteWrite(&g_sstSDInfo, pucParam[i]);
    }

    //
    // CMD8 is an SD mode command, need CRC
    //
    SS_SDSPIByteWrite(&g_sstSDInfo, 0x87);

    //
    // Waiting response
    //
    for(i = 0; i < SS_SD_CMD_TIMEOUT; i++)
    {
        SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);
        if(!(ucTmp & 0x80))
        {
            break;
        }
    }
    if(i >= SS_SD_CMD_TIMEOUT)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        return e_SD_ERR_CMD_TIMEOUT;
    }

    //
    // Read the response
    //
    for(i = 5 - 1; i >= 0; i--)
    {
        pucResp[i] = ucTmp;
        SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);
    }

    SS_SDSPICSDeAssert(&g_sstSDInfo);

    //
    // Check the resonse, first byte is R1
    //
    if(pucResp[0] != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    // support 3V3 ?
    if((pucResp[4] & SS_SD_R7_4_VOLTAGE_MASK) == 0x01)
    {
        *pbVoltageAccepted = true;
    }
    else
    {
        *pbVoltageAccepted = false;
    }

    return e_SD_NO_ERR;

}

//*****************************************************************************
//
//! \brief Read the CSD Register.
//!
//! \param ucCSDLen is the CSD register length, the full register is 16 byte.
//! \param pucRdBuf is the destination buffer to store the register value.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDCSDRead(unsigned char ucCSDLen, unsigned char *pucRdBuf)
{
    unsigned char pucParam[4] = {0,0,0,0}, ucResp;
    SS_SD_ERROR eRet;

    eRet = SS_SDCmdWrite(SS_SD_CMD9, pucParam, SS_SD_CMD9_R, &ucResp);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    if(ucResp != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    return (SS_SDRegisterRead(ucCSDLen, pucRdBuf));
}

//*****************************************************************************
//
//! \brief Read the CID Register.
//!
//! \param ucCIDLen is the CID register length, the full register is 16 byte.
//! \param pucRdBuf is the destination buffer to store the register value.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDCIDRead(unsigned char ucCIDLen, unsigned char *pucRdBuf)
{
    unsigned char pucParam[4] = {0,0,0,0}, ucResp;
    SS_SD_ERROR eRet;

    eRet = SS_SDCmdWrite(SS_SD_CMD10, pucParam, SS_SD_CMD10_R, &ucResp);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    if(ucResp != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    return (SS_SDRegisterRead(ucCIDLen, pucRdBuf));
}

//*****************************************************************************
//
//! \brief Forces the card to stop transmission in Multiple Block Read Operation.
//!
//! \param None.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDTransmissionStop(void)
{
    unsigned char pucParam[4] = {0,0,0,0}, ucResp;

    return (SS_SDCmdWrite(SS_SD_CMD12, pucParam, SS_SD_CMD12_R, &ucResp));
}

//*****************************************************************************
//
//! \brief Read the status Register.
//!
//! \param ucLen is the status register length.
//! \param pucRdBuf is the destination buffer to store the register value.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDCardStatusRead(unsigned char ucLen, unsigned char *pucBuffer)
{
    unsigned char pucParam[4] = {0,0,0,0};

    return (SS_SDCmdWrite(SS_SD_CMD13, pucParam, SS_SD_CMD13_R, pucBuffer));
}

//*****************************************************************************
//
//! \brief Set the block size.
//!
//! \param ulLength is the block size, the size is always 512 bytes.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDBlockLenSet(unsigned long ulLength)
{
    unsigned char pucParam[4] = {0,0,0,0}, ucResp;
    SS_SD_ERROR eRet;

    SS_SDParamPack(pucParam, ulLength);
    eRet = SS_SDCmdWrite(SS_SD_CMD16, pucParam, SS_SD_CMD16_R, &ucResp);

    if(eRet !=e_SD_NO_ERR)
    {
        return eRet;
    }
    if(ucResp != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \brief Send request of reading a block.
//!
//! \param ulBlockAddr the block address.
//!
//! The parameter \b ulBlockAddr, SDSC Card uses byte unit address and SDHC and
//! SDXC Cards  use block unit address (512 bytes unit).
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDSingleBlockRead(unsigned long ulBlockAddr)
{
    return (SS_SDBlockCommand(SS_SD_CMD17, SS_SD_CMD17_R, ulBlockAddr));
}

//*****************************************************************************
//
//! \brief Send request of reading multiple blocks.
//!
//! \param ulBlockAddr the start block address.
//!
//! The parameter \b ulBlockAddr, SDSC Card uses byte unit address and SDHC and
//! SDXC Cards  use block unit address (512 bytes unit).
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDMultipleBlockRead(unsigned long ulBlockAddr)
{
    return (SS_SDBlockCommand(SS_SD_CMD18, SS_SD_CMD18_R, ulBlockAddr));
}

//*****************************************************************************
//
//! \brief Send request to write a block.
//!
//! \param ulBlockAddr the block address.
//!
//! The parameter \b ulBlockAddr, SDSC Card uses byte unit address and SDHC and
//! SDXC Cards  use block unit address (512 bytes unit).
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDSingleBlockWrite(unsigned long ulBlockAddr)
{
    return (SS_SDBlockCommand(SS_SD_CMD24, SS_SD_CMD24_R, ulBlockAddr));
}

//*****************************************************************************
//
//! \brief Send request to write multiple blocks.
//!
//! \param ulBlockAddr the start block address.
//!
//! The parameter \b ulBlockAddr, SDSC Card uses byte unit address and SDHC and
//! SDXC Cards  use block unit address (512 bytes unit).
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDMultipleBlockWrite(unsigned long ulBlockAddr)
{
    return (SS_SDBlockCommand(SS_SD_CMD25, SS_SD_CMD25_R, ulBlockAddr));
}

#if SS_SD_ERASE_BLOCK_EN
//*****************************************************************************
//
//! \brief Set the start block to erase.
//!
//! \param ulStartBlock the start block address.
//!
//! The parameter \b ulStartBlock, SDSC Card uses byte unit address and SDHC and
//! SDXC Cards  use block unit address (512 bytes unit).
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDEraseStartBlockSet(unsigned long ulStartBlock)
{
    return (SS_SDBlockCommand(SS_SD_CMD32, SS_SD_CMD32_R, ulStartBlock));
}

//*****************************************************************************
//
//! \brief Set the end block to erase.
//!
//! \param ulEndBlock the start block address.
//!
//! The parameter \b ulEndBlock, SDSC Card uses byte unit address and SDHC and
//! SDXC Cards  use block unit address (512 bytes unit).
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDEraseEndBlockSet(unsigned long ulEndBlock)
{
    return (SS_SDBlockCommand(SS_SD_CMD33, SS_SD_CMD33_R, ulEndBlock));
}

//*****************************************************************************
//
//! \brief Execute to erase the select blocks.
//!
//! \param ulEndBlock the start block address.
//!
//! The start block is set by SDEraseStartBlockSet(), the end block is set by
//! SDEraseEndBlockSet().
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDEraseSelected(void)
{
    unsigned char pucParam[4] = {0,0,0,0}, ucResp;
    SS_SD_ERROR eRet;

    SS_SDParamPack(pucParam, 0);
    eRet = SS_SDCmdWrite(SS_SD_CMD38, pucParam, SS_SD_CMD38_R, &ucResp);

    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    if(SS_SDWaitBusy(SS_SD_WAIT_ERASE) != e_SD_NO_ERR)
    {
        return e_SD_ERR_TIMEOUT_ERASE;
    }
    else
    {
        return e_SD_NO_ERR;
    }
}
#endif

//*****************************************************************************
//
//! \brief Read the OCR Register.
//!
//! \param ucOCRLen is the OCR register length, the full register is 4 byte.
//! \param pucRdBuf is the destination buffer to store the register value.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDOCRRead(unsigned char ucOCRLen, unsigned char *pucRdBuf)
{
    unsigned char pucParam[4] = {0,0,0,0}, pucResp[5], ucTmp;
    SS_SD_ERROR eTmp;

    eTmp = SS_SDCmdWrite(SS_SD_CMD58, pucParam, SS_SD_CMD58_R, pucResp);
    if(eTmp != e_SD_NO_ERR)
    {
        return eTmp;
    }

    if(pucResp[0] != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    for(ucTmp = 0; ucTmp < ucOCRLen; ucTmp++)
    {
        pucRdBuf[ucTmp] = pucResp[ucTmp + 1];
    }

    return e_SD_NO_ERR;
}

#if SS_SD_CRC_EN
//*****************************************************************************
//
//! \brief Enable/Disable the CRC in SPI mode.
//!
//! \param bEnable is CRC enable state. xture means enable, xfalse means disable.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SD_EnableCRC(xtBoolean bEnable)
{
    unsigned char pucParam[4],ucResp;
    SS_SD_ERROR eRet;

    if(bEnable)
    {
        pucParam[0] = 1;
    }
    else
    {
        pucParam[1] = 0;
    }
    eRet = SS_SDCmdWrite(SS_SD_CMD59, pucParam, SS_SD_CMD59_R, &ucResp);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    if(ucResp != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    return e_SD_NO_ERR;

}
#endif

#if SS_SD_WRITE_MULTI_BLOCK_EN
//*****************************************************************************
//
//! \brief Get the blocks number that have been sucessful written with mulit
//! block command.
//!
//! \param pulBlockNum is buffer to store the number.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDWrBlockNumberGet(unsigned long *pulBlockNum)
{
    unsigned char pucTmp[4] = {0,0,0,0},ucResp;
    SS_SD_ERROR eRet;

    eRet = SS_SDCmdWrite(SS_SD_CMD55, pucTmp, SS_SD_CMD55_R, &ucResp);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    if(ucResp != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    eRet = SS_SDCmdWrite(SS_SD_ACMD22, pucTmp, SS_SD_ACMD22_R, &ucResp);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    if(ucResp != 0)
    {
        return e_SD_ERR_CMD_RESP;
    }

    eRet = SS_SDBlockDataRead(pucTmp, 4);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    *pulBlockNum = (pucTmp[0] << 24) + (pucTmp[1] << 16) +
                   (pucTmp[2] << 8) + pucTmp[3];

    return e_SD_NO_ERR;
}
#endif


///////////////////////////////////////////////////////////////////////////////


//*****************************************************************************
//
//! \brief Transfer the register data after sending register read request.
//!
//! \param ulLen is the length to transfer.
//! \param pucRdBuf is the destination buffer to store the value.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDRegisterRead(unsigned long ulLen, unsigned char* pucRdBuf)
{
    unsigned long i = 0;
    unsigned char ucResp;
    unsigned char ucTemp;

    if(!pucRdBuf)
    	return e_SD_ERR_USER_PARAM;

    SS_SDSPICSAssert(&g_sstSDInfo);

    // wait for data start token
    i = 0;
    do
    {
        SS_SDSPIByteRead(&g_sstSDInfo, &ucResp);
        i++;
    }while((ucResp == 0xFF) && (i < SS_SD_READREG_TIMEOUT));

    if(i >= SS_SD_READREG_TIMEOUT)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        return e_SD_ERR_TIMEOUT_READ;
    }

    if(ucResp != SS_SD_TOK_RD_START_BLOCK)
    {
        pucRdBuf[0] = ucResp;
        i = 1;
    }
    else
    {
        i = 0;
    }

    for(; i < ulLen; i++)
    {
        SS_SDSPIByteRead(&g_sstSDInfo, &pucRdBuf[i]);
    }

    // Read CRC16
    SS_SDSPIByteRead(&g_sstSDInfo, &ucTemp);
    i = ucTemp;
    SS_SDSPIByteRead(&g_sstSDInfo, &ucTemp);
    i = (i << 16) | ucTemp;

#if SS_SD_CRC_EN
    if(i != SS_SDCRC16Get(pucRdBuf, ulLen))
    {
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
        return e_SD_ERR_DATA_CRC16;
    }
#endif

    SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
    SS_SDSPICSDeAssert(&g_sstSDInfo);

    return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \brief Transfer the block data after sending block read request.
//!
//! \param pucRdBuf is the destination buffer to store the value.
//! \param ulLen is the length to transfer.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDBlockDataRead(unsigned char *pucRdBuf, unsigned long ulLen)
{
    unsigned char ucTmp;
    unsigned long i = 0;
    unsigned char ucTemp;

    SS_SDSPICSAssert(&g_sstSDInfo);

    do
    {
        SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);
        i++;
    }while((ucTmp == 0xFF) && (i < g_sSDCardInfo.ulReadTimeout));


    if(i >= g_sSDCardInfo.ulReadTimeout)
    {
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        return e_SD_ERR_TIMEOUT_READ;
    }

    if(ucTmp != SS_SD_TOK_RD_START_BLOCK)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        return e_SD_ERR_DATA_START_TOK;
    }

    for(i = 0; i < ulLen; i++)
    {
        SS_SDSPIByteRead(&g_sstSDInfo, &pucRdBuf[i]);
    }

    SS_SDSPIByteRead(&g_sstSDInfo, &ucTemp);
    i = ucTemp;
    SS_SDSPIByteRead(&g_sstSDInfo, &ucTemp);
    i = (i << 8) + ucTemp;

#if SS_SD_CRC_EN
    if(i != SS_SDCRC16Get(pucRdBuf, ulLen))
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        return e_SD_ERR_DATA_CRC16;
    }
#endif

    SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
    SS_SDSPICSDeAssert(&g_sstSDInfo);

    return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \brief Transfer the block data after sending block write request.
//!
//! \param pucWrBuf is the source buffer to transfer.
//! \param ulLen is the length to transfer.
//! \param bMulti indicate if this is an multi block write request.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDBlockDataWrite(unsigned char *pucWrBuf, unsigned long ulLen,
                 unsigned char bMulti)
{
    unsigned short i;
    unsigned char ucTmp;

    SS_SDSPICSAssert(&g_sstSDInfo);

    SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);

    if(bMulti)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, SS_SD_TOK_WR_START_BLOCK_MULTI);
    }
    else
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, SS_SD_TOK_WR_START_BLOCK);
    }

    for(i = 0; i < ulLen; i++)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, pucWrBuf[i]);
    }

#if SS_SD_CRC_EN
    i = SS_SDCRC16Get(pucWrBuf,ulLen);
#endif

    SS_SDSPIByteWrite(&g_sstSDInfo, (i >> 8) & 0xFF);
    SS_SDSPIByteWrite(&g_sstSDInfo, i & 0xFF);

    SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);
    if((ucTmp & SS_SD_RESP_DATA_MASK) != SS_SD_RESP_DATA_ACCETPTED)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
        SS_SDSPICSDeAssert(&g_sstSDInfo);
        return e_SD_ERR_DATA_RESP;
    }

    SS_SDSPICSDeAssert(&g_sstSDInfo);

    if(SS_SDWaitBusy(SS_SD_WAIT_WRITE) != e_SD_NO_ERR)
    {
        return e_SD_ERR_TIMEOUT_WRITE;
    }
    else
    {
        return e_SD_NO_ERR;
    }
 }

//*****************************************************************************
//
//! \brief Send the token to stop multi block transfer.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void
SS_SDStopMultiToken(void)
{
	unsigned char ucTmp;

    SS_SDSPICSAssert(&g_sstSDInfo);

    SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
    SS_SDSPIByteWrite(&g_sstSDInfo, SS_SD_TOK_STOP_TRAN_MULTI);
    SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);

    SS_SDSPICSDeAssert(&g_sstSDInfo);
}

//*****************************************************************************
//
//! \brief Wait the sd card read/write/erase execute complete.
//!
//! \param ucWaitType is the execute type to wait.
//!
//! The parameter \b ucWaitType can be:
//! - \ref SD_WAIT_READ - wait read complete.
//! - \ref SD_WAIT_WRITE - wait write complete.
//! - \ref SD_WAIT_ERASE - wait erase complete.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDWaitBusy(unsigned char ucWaitType)
{
    unsigned long ulTimeout = 0, i = 0;
    unsigned char ucTmp;

    if(ucWaitType == SS_SD_WAIT_WRITE)
    {
        ulTimeout = g_sSDCardInfo.ulWriteTimeout;
    }
    else if(ucWaitType == SS_SD_WAIT_WRITE)
    {
        ulTimeout = g_sSDCardInfo.ulReadTimeout;
    }
    else
    {
        ulTimeout = g_sSDCardInfo.ulEraseTimeout;
    }

    SS_SDSPICSAssert(&g_sstSDInfo);
    do
    {
        SS_SDSPIByteRead(&g_sstSDInfo, &ucTmp);
        i++;
    }while ((ucTmp != 0xFF) && (i < ulTimeout));
    SS_SDSPICSDeAssert(&g_sstSDInfo);

    if(i < ulTimeout)
    {
        return e_SD_NO_ERR;
    }
    else
    {
        return e_SD_ERR_TIMEOUT_WAIT;
    }
}

//*****************************************************************************
//
//! \brief Delay some SPI clocks.
//!
//! \param None.
//!
//! The delay unit is 8 SPi clocks. So the total clocks delay is:
//! ulDelay8Clock * 8
//!
//! \return None.
//
//*****************************************************************************
void
SS_SDSPIDelay(unsigned long ulDelay8Clock)
{
    unsigned long i;

    for(i = 0; i < ulDelay8Clock; i++)
    {
        SS_SDSPIByteWrite(&g_sstSDInfo, 0xFF);
    }
}

//*****************************************************************************
//
//! \brief Calculate sdcard read/write/erase timeout according CSD register.
//!
//! \param pucCSDBuf is the CSD register buffer.(16 byte)
//!
//! \return None.
//
//*****************************************************************************
static void
SS_SDCalTimeout(unsigned char *pucCSDBuf)
{
    unsigned long ulTmp;
    unsigned char ucTimeUnit,ucTimeValue,ucTimeoutFactor;

    if(!pucCSDBuf)
    	return;

    //
    // Set default timeout value
    //
    g_sSDCardInfo.ulReadTimeout = SS_SD_READ_TIMEOUT_100MS;
    g_sSDCardInfo.ulWriteTimeout = SS_SD_WRITE_TIMEOUT_250MS;
    g_sSDCardInfo.ulEraseTimeout = SS_SD_WRITE_TIMEOUT_250MS;

    //
    // TAAC[119:112] (time unit [2:0], time value [6:3])
    // R2W Factor [28:26]
    //
    ucTimeUnit = SS_SDCSDExtBits(pucCSDBuf, 119, 112) & 0x07;
    ucTimeValue = (SS_SDCSDExtBits(pucCSDBuf, 119, 112) >> 3) & 0x0F;
    ucTimeoutFactor = SS_SDCSDExtBits(pucCSDBuf, 28, 26);

    if(ucTimeValue == 0)
    {
        return;
    }
    if(ucTimeoutFactor >= 6)
    {
        return;
    }

    ulTmp = SS_SD_SPI_CLOCK * g_pucCSDTAACTimeValue[ucTimeValue] / 10 /
            g_pulCSDTAACTimeUnit[ucTimeUnit];

    //
    // NSAC [114:104]
    //
    ulTmp += SS_SDCSDExtBits(pucCSDBuf, 114, 104) * 100;

    //
    // 8 SPI Clock is a unit
    //
    g_sSDCardInfo.ulReadTimeout = ulTmp / 8;
    g_sSDCardInfo.ulWriteTimeout = (ulTmp * g_pucCSDR2WFactor[ucTimeoutFactor]) / 8;

    if(g_sSDCardInfo.ulReadTimeout > SS_SD_READ_TIMEOUT_100MS)
    {
        g_sSDCardInfo.ulReadTimeout = SS_SD_READ_TIMEOUT_100MS;
    }

    if(g_sSDCardInfo.ulWriteTimeout > SS_SD_WRITE_TIMEOUT_250MS)
    {
        g_sSDCardInfo.ulWriteTimeout = SS_SD_WRITE_TIMEOUT_250MS;
    }

    g_sSDCardInfo.ulEraseTimeout = g_sSDCardInfo.ulWriteTimeout;
}

//*****************************************************************************
//
//! \brief Parse sdcard info such as page size, block size, block number
//! according the CSD register.
//!
//! \param None.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
static SS_SD_ERROR SS_SDCardInfoParse(void)
{
	SS_SD_ERROR eRet;
    unsigned char pucCSDBuf[16];
    unsigned long ulMaxRdBlockSize = 0;

    //
    // Read CID register
    eRet = SS_SDCSDRead(16, g_sSDCardInfo.pucCID);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    //
    //
    // Read CSD register
    //
    eRet = SS_SDCSDRead(16, pucCSDBuf);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    //
    // Calculate sdcard timeout according CSD
    //
    SS_SDCalTimeout(pucCSDBuf);

    //
    // Calculate the size of a sector
    // READ_BL_LEN [83:80]
    // BlockSize = 2^READ_BL_LEN
    //
    ulMaxRdBlockSize = 1 << (SS_SDCSDExtBits(pucCSDBuf, 83, 80));

    //
    // calculate the sector numbers of the SD Card
    // C_SIZE [73:62], C_SIZE_MULT[49:47]
    // BlockNumber = (C_SIZE + 1) * 2^(C_SIZE_MULT + 2)
    //
    g_sSDCardInfo.ulBlockNumber = (SS_SDCSDExtBits(pucCSDBuf, 73, 62) + 1) *
                                  (1 << (SS_SDCSDExtBits(pucCSDBuf, 49, 47) + 2));

    //
    // Block Unit -> SD_BLOCK_SIZE(512)
    //
    g_sSDCardInfo.ulBlockNumber *=  (ulMaxRdBlockSize / SS_SD_BLOCK_SIZE);

    //
    // calculate the size of sector(erase unit is block number)
    // SECTOR_SIZE[45:39]
    // Erase Unit = SECTOR_SIZE + 1
    //
    g_sSDCardInfo.ulEraseUnit = SS_SDCSDExtBits(pucCSDBuf, 45, 39) + 1;

    //
    // Block Unit -> SD_BLOCK_SIZE(512)
    //
    g_sSDCardInfo.ulEraseUnit *= (ulMaxRdBlockSize / SS_SD_BLOCK_SIZE);

    return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \brief Waiting sdcard internal initialization complete.
//!
//! \param None.
//!
//! Waiting sdcard init OK by repeating sending CMD1.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
static SS_SD_ERROR SS_SDActiveInit(void)
{
    unsigned char pucParam[4] = {0,0,0,0},  ucResp;
    unsigned long i = 0;
    SS_SD_ERROR eRet;
    //
    // Wait unit the SD card initialize complete.
    //
    do
    {
        //
        // The next command is an application specific command
        //
        eRet = SS_SDCmdWrite(SS_SD_CMD55, pucParam, SS_SD_CMD55_R, &ucResp);
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }

        eRet = SS_SDCmdWrite(SS_SD_ACMD41, pucParam, SS_SD_ACMD41_R, &ucResp);
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }
        i++;
    }while (((ucResp & 0x01) == 0x01) && (i < SS_SD_IDLE_WAIT_MAX));

    if(i >= SS_SD_IDLE_WAIT_MAX)
    {
        return e_SD_ERR_TIMEOUT_WAIT_IDLE;
    }

    return e_SD_NO_ERR;
}

//*****************************************************************************
//
//! \brief Initializate the sdcard.
//!
//! \param None.
//!
//! This function initializate the hardware, re-power on the sdcard, do some
//! initialization sequence, then get some sdcard info, such as block size,
//! block number, the CID and so on.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDInit(const SS_SD_CFG *pSDConfig)
{
    unsigned char pucRdBuf[4];
    bool bVoltageAccepted = false;
    SS_SD_ERROR eRet;

    //
    // Hardware Init(SPI, Power)
    //
    eRet = SS_SDHardwareInit(pSDConfig, &g_cstPalSDCardSPI);
//    eRet = SS_SDHardwareInit(pSDConfig, pSPIConfig);
    if(eRet != e_SD_NO_ERR)
    	return eRet;

    //
    // Assert CS at least 74 SPI clocks
    //
    SS_SDSPICSAssert(&g_sstSDInfo);
    SS_SDSPIDelay(10);
    SS_SDSPICSDeAssert(&g_sstSDInfo);
    SS_SDSPIDelay(1);

    //
    // Reset sdcard
    //
    eRet = SS_SDCardReset();
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    eRet = SS_SDConditionCheck(&bVoltageAccepted);
    if(eRet != e_SD_NO_ERR)
    {
        // Ver 1.x
        //
        // Read OCR register to check if 3V3 compatible
        //
        eRet = SS_SDOCRRead(4, pucRdBuf);
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }
        //
        //3.2V - 3.4V [21:20]
        //
        if(!SS_SDExtBits(pucRdBuf, 4, 21, 20))
        {
            //
            // Not support 3.3V
            //
            return e_SD_ERR_VOL_NOT_SUSP;
        }

        //
        // Waiting sdcard internal initialization complete
        //
        eRet = SS_SDActiveInit();
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }

        //
        // Ver1.X Standard Capacity SD Memory Card
        //
        g_sSDCardInfo.ulCardType = SS_SD_TYPE_SDSC_V1;

    }
    else
    {
        // Ver 2.x
        if(!bVoltageAccepted)
        {
            return e_SD_ERR_VOL_NOT_SUSP;
        }
        //
        // Read OCR register to check if 3V3 compatible
        //
        eRet = SS_SDOCRRead(4, pucRdBuf);
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }
        //
        //3.2V - 3.4V [21:20]
        //
        if(!SS_SDExtBits(pucRdBuf, 4, 21, 20))
        {
            //
            // Not support 3.3V
            //
            return e_SD_ERR_VOL_NOT_SUSP;
        }

        //
        // Waiting sdcard internal initialization complete
        //
        eRet = SS_SDActiveInit();
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }
        //
        // Read OCR register to check CCS
        //
        eRet = SS_SDOCRRead(4, pucRdBuf);
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }

        if(SS_SDExtBits(pucRdBuf, 4, 30, 30) == 0)
        {
            //
            // Ver2.X Standard Capacity SD Memory Card
            //
            g_sSDCardInfo.ulCardType = SS_SD_TYPE_SDSC_V2;
        }
        else
        {
            //
            // Ver2.X High Capacity or Extended Capacity SD Memory Card
            //
            g_sSDCardInfo.ulCardType = SS_SD_TYPE_SDSC_V1;
        }
    }

    //
    // Set SPI Clock to Max
    //
    SS_SDSPIClkToMax();

#if SS_SD_CRC_EN
    //
    // Enable CRC
    //
    eRet = SS_SD_EnableCRC();
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }
#endif

    //
    // Set the Block Length to 512 Byte
    //
    eRet = SS_SDBlockLenSet(SS_SD_BLOCK_SIZE);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    //
    // Get CID, CSD register, then Parse
    //
    return (SS_SDCardInfoParse());

}

//*****************************************************************************
//
//! \brief Get some bit-field from a byte buffer.
//!
//! \param None.
//!
//! This function is always used to get a register bit field.
//!
//! \return Returns the bit-field value.
//
//*****************************************************************************
unsigned long
SS_SDExtBits(unsigned char *pucBuf, unsigned long ulBufLen,
          unsigned long ulMSB, unsigned long ulLSB)
{
    unsigned long i;
    unsigned long ulBits = 0, ulPosition, ulByte, ulBit, ulValue;
    unsigned long ulSize = 1 + ulMSB - ulLSB;

    if(!pucBuf)
    	while(1);

    if(!(ulBufLen > 0))
    	while(1);

    if(!(ulMSB >= ulLSB))
    	while(1);

    if(!(ulMSB < (ulBufLen * 8)))
    	while(1);

    if(!(ulMSB - ulLSB < 32))
    	while(1);

    for(i=0; i < ulSize; i++)
    {
        ulPosition = ulLSB + i;
        ulByte = (ulBufLen - 1) - (ulPosition >> 3);
        ulBit = ulPosition & 0x7;
        ulValue = (pucBuf[ulByte] >> ulBit) & 1;
        ulBits |= ulValue << i;
    }
    return ulBits;
}

//*****************************************************************************
//
//! \brief Read a block from the sdcard.
//!
//! \param pucDestBuf is the destination buffer to store the value that read.
//! \param ulBlockIndex is the block index to read. The index start from 0.
//!
//! This function is used to read a block data from the sdcard, a block length
//! is always 512 bytes, defined by SD_BLOCK_SIZE.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDBlockRead(unsigned char *pucDestBuf, unsigned long ulBlockIndex)
{
	SS_SD_ERROR eRet;
    if(!pucDestBuf)
    	return e_SD_ERR_USER_PARAM;

    //
    // Over the card range
    //
    if(ulBlockIndex >= g_sSDCardInfo.ulBlockNumber)
    {
        return e_SD_ERR_OVER_CARD_RANGE;
    }

    //
    // Set Read address
    //
    if(g_sSDCardInfo.ulCardType == SS_SD_TYPE_SDHC_SDXC)
    {
        eRet = SS_SDSingleBlockRead(ulBlockIndex);
    }
    else
    {
//        eRet = SS_SDSingleBlockRead(ulBlockIndex * SS_SD_BLOCK_SIZE);
        eRet = SS_SDSingleBlockRead(ulBlockIndex);
    }

    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    //
    // Read Data
    //
    eRet = SS_SDBlockDataRead(pucDestBuf, SS_SD_BLOCK_SIZE);

    return eRet;
}
#if SS_SD_READ_MULTI_BLOCK_EN
//*****************************************************************************
//
//! \brief Read multi blocks from the sdcard.
//!
//! \param pucDestBuf is the destination buffer to store the value that read.
//! \param ulBlockIndex is the block index to read. The index start from 0.
//! \param ulRdBlockNumber is the number of blocks to read.
//!
//! This function is used to read some blocks data from the sdcard, a block
//! length is always 512 bytes, defined by SD_BLOCK_SIZE.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDMultiBlockRead(unsigned char *pucDestBuf, unsigned long ulStartBlockIndex,
                 unsigned long ulRdBlockNumber)
{
	SS_SD_ERROR eRet;
    unsigned long i;

    if(!pucDestBuf)
    	return e_SD_ERR_USER_PARAM;

    //
    // Over the card range
    //
    if(ulStartBlockIndex + ulRdBlockNumber > g_sSDCardInfo.ulBlockNumber)
    {
        return e_SD_ERR_OVER_CARD_RANGE;
    }

    //
    // Set Read Address
    //
    if(g_sSDCardInfo.ulCardType == SS_SD_TYPE_SDHC_SDXC)
    {
        eRet = SS_SDMultipleBlockRead(ulStartBlockIndex);
    }
    else
    {
        eRet = SS_SDMultipleBlockRead(ulStartBlockIndex * SS_SD_BLOCK_SIZE);
    }
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    for(i = 0; i < ulRdBlockNumber; i++)
    {
        eRet = SS_SDBlockDataRead(&pucDestBuf[i * SS_SD_BLOCK_SIZE], SS_SD_BLOCK_SIZE);
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }
    }

    eRet = SS_SDTransmissionStop();

    //
    // Delay while busy
    //
    SS_SDWaitBusy(SS_SD_WAIT_READ);

    return eRet;


}
#endif

//*****************************************************************************
//
//! \brief Write a block to the sdcard.
//!
//! \param pucSrcBuf is the source buffer to write.
//! \param ulBlockIndex is the block index to write. The index start from 0.
//!
//! This function is used to write a block data from the sdcard, a block length
//! is always 512 bytes, defined by SD_BLOCK_SIZE.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDBlockWrite(const unsigned char *pucSrcBuf, unsigned long ulBlockIndex)
{
    unsigned char pucTmp[2];
    SS_SD_ERROR eRet;
    //
    // Over the card range
    //
    if(ulBlockIndex >= g_sSDCardInfo.ulBlockNumber)
    {
        return e_SD_ERR_OVER_CARD_RANGE;
    }

    //
    // Set Write address
    //
    if(g_sSDCardInfo.ulCardType == SS_SD_TYPE_SDHC_SDXC)
    {
        eRet = SS_SDSingleBlockWrite(ulBlockIndex);
    }
    else
    {
        eRet = SS_SDSingleBlockWrite(ulBlockIndex * SS_SD_BLOCK_SIZE);
    }

    //
    // Write Data
    //
    eRet = SS_SDBlockDataWrite((unsigned char*)pucSrcBuf, SS_SD_BLOCK_SIZE, false);

    if(eRet == e_SD_NO_ERR)
    {
        eRet = SS_SDCardStatusRead(2, pucTmp);
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }

        if((pucTmp[0] != 0) || (pucTmp[1] != 0))
        {
            return e_SD_ERR_WRITE_BLK;
        }
    }

    return eRet;
}

#if SS_SD_WRITE_MULTI_BLOCK_EN
//*****************************************************************************
//
//! \brief Write multi blocks to the sdcard.
//!
//! \param pucSrcBuf is the source buffer to write.
//! \param ulStartBlockIndex is the start block index to write. The index start
//! from 0.
//! \param ulWrBlockNumber is number of blocks to write.
//!
//! This function is used to write some blocks data to the sdcard, a block length
//! is always 512 bytes, defined by SD_BLOCK_SIZE.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDMultiBlockWrite(const unsigned char* pucSrcBuf,
                  unsigned long ulStartBlockIndex,
                  unsigned long ulWrBlockNumber)
{
	SS_SD_ERROR eRet;
    unsigned long i;

    if(!pucSrcBuf)
    	return e_SD_ERR_USER_PARAM;

    //
    // Over the card range
    //
    if(ulStartBlockIndex + ulWrBlockNumber > g_sSDCardInfo.ulBlockNumber)
    {
        return e_SD_ERR_OVER_CARD_RANGE;
    }

    //
    // Set Write address
    //
    if(g_sSDCardInfo.ulCardType == SS_SD_TYPE_SDHC_SDXC)
    {
        eRet = SS_SDMultipleBlockWrite(ulStartBlockIndex);
    }
    else
    {
        eRet = SS_SDMultipleBlockWrite(ulStartBlockIndex * SS_SD_BLOCK_SIZE);
    }

    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    for(i = 0; i < ulWrBlockNumber; i++)
    {
        eRet = SS_SDBlockDataWrite((unsigned char*)&pucSrcBuf[i * SS_SD_BLOCK_SIZE],
                                 SS_SD_BLOCK_SIZE,
                                 true);
        if(eRet != e_SD_NO_ERR)
        {
            SS_SDTransmissionStop();
            SS_SDWaitBusy(SS_SD_WAIT_WRITE);
            return eRet;
        }
    }

    SS_SDStopMultiToken();
    eRet = SS_SDWaitBusy(SS_SD_WAIT_WRITE);
    if(eRet != e_SD_NO_ERR)
    {
        return e_SD_ERR_TIMEOUT_WRITE;
    }

    //
    // Get the block number that write correctly
    //
    eRet = SS_SDWrBlockNumberGet(&i);
    if(eRet != e_SD_NO_ERR)
    {
        return eRet;
    }

    if(ulWrBlockNumber != i)
    {
        return e_SD_ERR_WRITE_BLK_NUMS;
    }

    return e_SD_NO_ERR;

}
#endif

#if SS_SD_ERASE_BLOCK_EN
//*****************************************************************************
//
//! \brief Erase some blocks.
//!
//! \param pucSrcBuf is the source buffer to write.
//! \param ulStartBlockIndex is the start block index to erase. The index start
//! from 0.
//! \param ulBlockNumber is number of blocks to erase.
//!
//! This function is used to write some blocks data to the SS_SDcard, a block length
//! is always 512 bytes, defined by SS_SD_BLOCK_SIZE. \n
//! The erase unit defined by \ref SS_SDCardDeviceInfo.ulEraseUnit, is the block
//! number that sdcard can erase max once.
//!
//! \return Returns SD_NO_ERR indicates everything is OK, others is the error
//! code.
//
//*****************************************************************************
SS_SD_ERROR SS_SDBlockErase(unsigned long ulStartBlockIndex, unsigned long ulBlockNumber)
{
	SS_SD_ERROR eRet;
    unsigned long ulRemain = ulBlockNumber;
    unsigned long ulEraseBlock = ulStartBlockIndex, ulEraseOnce;
    //
    // Over the card range
    //
    if(ulStartBlockIndex + ulBlockNumber > g_sSDCardInfo.ulBlockNumber)
    {
        return e_SD_ERR_OVER_CARD_RANGE;
    }

    while(ulRemain)
    {
        ulEraseOnce = (ulRemain < g_sSDCardInfo.ulEraseUnit) ? ulRemain :
                                         g_sSDCardInfo.ulEraseUnit;
        //
        // Set erase start address
        //
        if(g_sSDCardInfo.ulCardType == SS_SD_TYPE_SDHC_SDXC)
        {
            eRet = SS_SDEraseStartBlockSet(ulEraseBlock);
        }
        else
        {
            eRet = SS_SDEraseStartBlockSet(ulEraseBlock * SS_SD_BLOCK_SIZE);
        }
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }

        //
        // Set erase end address
        //
        if(g_sSDCardInfo.ulCardType == SS_SD_TYPE_SDHC_SDXC)
        {
            eRet = SS_SDEraseEndBlockSet(ulEraseBlock + ulEraseOnce - 1);
        }
        else
        {
            eRet = SS_SDEraseEndBlockSet((ulEraseBlock + ulEraseOnce - 1) *
                                       SS_SD_BLOCK_SIZE);
        }

        //
        // execute erase
        //
        eRet = SS_SDEraseSelected();
        if(eRet != e_SD_NO_ERR)
        {
            return eRet;
        }

        ulEraseBlock += ulEraseOnce;
        ulRemain -= ulEraseOnce;
    }

    return e_SD_NO_ERR;
}
#endif

//*****************************************************************************
//
//! \brief Get the sdcard info, such as total block number, CID and so on.
//!
//! \param None.
//!
//! This function should by called only after SDInit().
//!
//! \return Returns the sdcard info struct point.
//
//*****************************************************************************
const SS_SDCardDeviceInfo* SS_SDCardInfoGet(void)
{
    return &g_sSDCardInfo;
}

bool SS_SDCardIsIn(SS_SD_INFO *pConfig)
{
	return (RESET == GPIO_ReadInputDataBit(pConfig->pSDConfig->pGpioDecPort, pConfig->pSDConfig->u16GpioDecPin));
}

#endif
