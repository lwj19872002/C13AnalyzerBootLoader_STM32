/*
 * ss_sd_hw.h
 *
 *  Created on: 2013-4-1
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_SD_HW_H_
#define SS_SD_HW_H_

//*****************************************************************************
//
//! \addtogroup SD_Command_Basic SD Card Basic Command
//! @{
//
//*****************************************************************************
//
// timeout of command
//
#define SS_SD_CMD_TIMEOUT          100
#define SS_SD_READREG_TIMEOUT      8

//
//! Reset cards to idle state
//
#define SS_SD_CMD0                 0
#define SS_SD_CMD0_R               SS_SD_R1

//
//! Read the OCR (MMC mode, do not use for SD cards)
//
#define SS_SD_CMD1                 1
#define SS_SD_CMD1_R               SS_SD_R1

//
//! Sends SD Memory Card interface condition
//
#define SS_SD_CMD8                 8
#define SS_SD_CMD8_R               SS_SD_R7

//
//! Card sends the CSD
//
#define SS_SD_CMD9                 9
#define SS_SD_CMD9_R               SS_SD_R1

//
//! Card sends CID
//
#define SS_SD_CMD10                10
#define SS_SD_CMD10_R              SS_SD_R1

//
//! Stop a multiple block (stream) read/write operation
//
#define SS_SD_CMD12                12
#define SS_SD_CMD12_R              SS_SD_R1B

//
//! Get the addressed card's status register
//
#define SS_SD_CMD13                13
#define SS_SD_CMD13_R              SS_SD_R2

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Command_Block_Read SD Card Block Read Command
//! @{
//
//*****************************************************************************

//
//! Set the block length
//
#define SS_SD_CMD16                16
#define SS_SD_CMD16_R              SS_SD_R1

//
//! Read a single block
//
#define SS_SD_CMD17                17
#define SS_SD_CMD17_R              SS_SD_R1

//
//! Read multiple blocks until a CMD12
//
#define SS_SD_CMD18                18
#define SS_SD_CMD18_R              SS_SD_R1

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Command_Block_Write SD Card Block Write Command
//! @{
//
//*****************************************************************************

//
//! Write a block of the size selected with CMD16
//
#define SS_SD_CMD24                24
#define SS_SD_CMD24_R              SS_SD_R1

//
//! Multiple block write until a CMD12
//
#define SS_SD_CMD25                25
#define SS_SD_CMD25_R              SS_SD_R1

//
//! Program the programmable bits of the CSD
//
#define SS_SD_CMD27                27
#define SS_SD_CMD27_R              SS_SD_R1

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Command_Write_Protect SD Card Write Protection Command
//! @{
//
//*****************************************************************************

//! Set the write protection bit of the addressed group
#define SS_SD_CMD28                28
#define SS_SD_CMD28_R              SS_SD_R1B

//! Clear the write protection bit of the addressed group
#define SS_SD_CMD29                29
#define SS_SD_CMD29_R              SS_SD_R1B

//! Ask the card for the status of the write protection bits
#define SS_SD_CMD30                30
#define SS_SD_CMD30_R              SS_SD_R1

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Command_Erase SD Card Erase Command
//! @{
//
//*****************************************************************************

//
//! Set the address of the first write block to be erased
//
#define SS_SD_CMD32                32
#define SS_SD_CMD32_R              SS_SD_R1

//
//! Set the address of the last write block to be erased
//
#define SS_SD_CMD33                33
#define SS_SD_CMD33_R              SS_SD_R1

//
//! Erase the selected write blocks
//
#define SS_SD_CMD38                38
#define SS_SD_CMD38_R              SS_SD_R1B

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Command_Lock_Card SD Card Lock Card Command
//! @{
//
//*****************************************************************************

//
//! Set/reset the password or lock/unlock the card
//
#define SS_SD_CMD42                42
#define SS_SD_CMD42_R              SS_SD_R1B

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Command_App SD Card Application-specific Command
//! @{
//
//*****************************************************************************

//
//! Flag that the next command is application-specific
//
#define SS_SD_CMD55                55
#define SS_SD_CMD55_R              SS_SD_R1

//
//! General purpose I/O for application-specific commands
//
#define SS_SD_CMD56                56
#define SS_SD_CMD56_R              SS_SD_R1

//
//! Read the OCR (SPI mode only)
//
#define SS_SD_CMD58                58
#define SS_SD_CMD58_R              SS_SD_R3

//
//! CRC Turn CRC on or off
//
#define SS_SD_CMD59                59
#define SS_SD_CMD59_R              SS_SD_R1

//
//! Get the SD card's status
//
#define SS_SD_ACMD13               13
#define SS_SD_ACMD13_R             SS_SD_R2

//
//! Get the number of written write blocks (Minus errors )
//
#define SS_SD_ACMD22               22
#define SS_SD_ACMD22_R             SS_SD_R1

//
//! Set the number of write blocks to be pre-erased before writing
//
#define SS_SD_ACMD23               23
#define SS_SD_ACMD23_R             SS_SD_R1

//
//! Get the card's OCR (SD mode)
//
#define SS_SD_ACMD41               41
#define SS_SD_ACMD41_R             SS_SD_R1

//
//! Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3]
//
#define SS_SD_ACMD42               42
#define SS_SD_ACMD42_R             SS_SD_R1

//
//! Get the SD configuration register
//
#define SS_SD_ACMD51               51
#define SS_SD_ACMD51_R             SS_SD_R1

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Token SD Card Response & Data Token
//!
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Wait_type SD Card Wait Busy Type
//! @{
//
//*****************************************************************************

#define SS_SD_WAIT_READ            0x00
#define SS_SD_WAIT_WRITE           0x01
#define SS_SD_WAIT_ERASE           0x02

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Response_Type SD Card Response Type
//! \brief SD Card Command Responses Type
//!
//! @{
//
//*****************************************************************************

#define SS_SD_R1                   1
#define SS_SD_R1B                  2
#define SS_SD_R2                   3
#define SS_SD_R3                   4
#define SS_SD_R7                   5

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_R1 SD Card Response R1
//! \brief SD Card Responses Format R1
//!
//! This response token is sent by the card after every command with the
//! exception of SEND_STATUS commands. It is one byte long, and the MSB is
//! always set to zero.
//!
//! The main response format (R1) is a status byte (normally zero). Key flags:
//! - idle - 1 if the card is in an idle state/initialising
//! - cmd  - 1 if an illegal command code was detected
//!
//!
//! \verbatim
//!    +-------------------------------------------------+
//! R1 | 0 | arg | addr | seq | crc | cmd | erase | idle |
//!    +-------------------------------------------------+
//! \endverbatim
//!
//! @{
//
//*****************************************************************************

#define SS_SD_R1_IDLE              0x01
#define SS_SD_R1_ERASE_RST         0x02
#define SS_SD_R1_ILLL_CMD          0x04
#define SS_SD_R1_CRC_ERR           0x08
#define SS_SD_R1_ERASE_SEQ_ERR     0x10
#define SS_SD_R1_ADDR_ERR          0x20
#define SS_SD_R1_PARAM_ERR         0x40

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_R2 SD Card Response R2
//! \brief SD Card Responses Format R2
//!
//! This response token(R2) is two bytes long and sent as a response to the
//! SEND_STATUS command.
//!
//! \verbatim
//! R1 / R2 Byte1
//! +-------------------------------------------------+
//! | 0 | arg | addr | seq | crc | cmd | erase | idle |
//! +-------------------------------------------------+
//! R2 Byte2
//! +------------------------------------------------------------------------+
//! | out of range | erase param | wp | ecc | cc | error | erase skip | lock |
//! +------------------------------------------------------------------------+
//! \endverbatim
//!
//! @{
//
//*****************************************************************************

#define SS_SD_R2_2_LOCK            0x01
#define SS_SD_R2_2_WP_ERASE_SKIP   0x02
#define SS_SD_R2_2_ERR             0x04
#define SS_SD_R2_2_CC_ERR          0x08
#define SS_SD_R2_2_ECC_FAILED      0x10
#define SS_SD_R2_2_WP_VIOLATION    0x20
#define SS_SD_R2_2_ERASE_PARAM     0x40
#define SS_SD_R2_2_OUT_OF_RANGE    0x80

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_R7 SD Card Response R7
//! \brief SD Card Responses Format R7
//!
//! This response token(R7) is 5 bytes long and sent as a response to the
//! SEND_IF_COND command(CMD8).
//!
//! \verbatim
//! R1 / R7 Byte1 [39:32]
//! +-------------------------------------------------+
//! | 0 | arg | addr | seq | crc | cmd | erase | idle |
//! +-------------------------------------------------+
//! R7 Byte2 [31:24]
//! +-------------------------------------------------------+
//! |  31  |  30  |  29  |  28  |  27  |  26  |  25  |  24  |
//! |-------------------------------------------------------|
//! |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
//! |-------------------------------------------------------|
//! |       CMD version         |      reserved             |
//! +-------------------------------------------------------+
//! R7 Byte3 [23:16] (reserved)
//! R7 Byte4 [15:8]
//! +-------------------------------------------------------+
//! |  15  |  14  |  13  |  12  |  11  |  10  |  9   |  8   |
//! |-------------------------------------------------------|
//! |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
//! |-------------------------------------------------------|
//! |       reserved            |      voltage accepted     |
//! +-------------------------------------------------------+
//! R7 Byte5 [7:0] check pattern (do not care)
//! \endverbatim
//!
//! @{
//
//*****************************************************************************

#define SS_SD_R7_2_CMD_VERSION_MASK                                              \
                                0xF0
#define SS_SD_R7_4_VOLTAGE_MASK    0x0F

//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup SD_Data_Token SD Card Data Token
//! \brief SD Card Data Response Token/Start Block Token/Stop tran Token
//!
//! \verbatim
//! Data Response Token
//! +-----------------------------------------------+
//! |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//! |-----------------------------------------------|
//! |  x  |  x  |  x  |  0  |      status     |  1  |
//! +-----------------------------------------------+
//!
//! Single Block Read/Write - Start Block Token(0xFE)
//! +-----------------------------------------------+
//! |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//! |-----------------------------------------------|
//! |  1  |  1  |  1  |  1  |  1  |  1  |  1  |  0  |
//! +-----------------------------------------------+
//!
//! Muti Blcok Read/Write - Start Block Token(0xFC)
//! +-----------------------------------------------+
//! |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//! |-----------------------------------------------|
//! |  1  |  1  |  1  |  1  |  1  |  1  |  0  |  0  |
//! +-----------------------------------------------+
//!
//! Muti Blcok Read/Write - Stop tran Token(0xFD)
//! +-----------------------------------------------+
//! |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//! |-----------------------------------------------|
//! |  1  |  1  |  1  |  1  |  1  |  1  |  0  |  1  |
//! +-----------------------------------------------+
//!
//! Data Error Token
//! +-----------------------------------------------+
//! |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//! |-----------------------------------------------|
//! |  0  |  0  |  0  |  0  |range| ECC |  CC |Error|
//! +-----------------------------------------------+
//! \endverbatim
//!
//! @{
//
//*****************************************************************************

#define SS_SD_RESP_DATA_MASK       0x1F
#define SS_SD_RESP_DATA_ACCETPTED  0x05
#define SS_SD_RESP_DATA_REJECT_CRC 0x0B
#define SS_SD_RESP_DATA_REJECT_WRITE                                             \
                                0x0D

#define SS_SD_TOK_RD_START_BLOCK   0xFE
#define SS_SD_TOK_WR_START_BLOCK   0xFE
#define SS_SD_TOK_RD_START_BLOCK_MULTI                                           \
                                0xFC
#define SS_SD_TOK_WR_START_BLOCK_MULTI                                           \
                                0xFC
#define SS_SD_TOK_STOP_TRAN_MULTI  0xFD

#endif /* SS_SD_HW_H_ */
