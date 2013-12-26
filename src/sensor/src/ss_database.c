/*
 * ss_database.c
 *
 *  Created on: 2013-4-15
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_DATABASE

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

#define SS_DB_PAGE_SIZE			(0x800)			// 存储页大小，跟CPU的flash页大小一致
#define SS_DB_PAGE1_ADDR		(0x0803F000)	// 数据库存储区1
#define SS_DB_PAGE2_ADDR		(0x0803F800)	// 数据库存储区2

// -------------------------------------------------------------------------
//| byte0 | ...... | byte2043 | ~CRCbyte0 | ~CRCbyte1 | CRCbyte0 | CRCbyte1 |
// -------------------------------------------------------------------------
// 用一个页的最后4个字节存储CRC校验值，其中前两个字节存储CRC的反码，后两个字节存储CRC的原码
#define SS_DB_CRCVALUE_DEX		SS_DB_USERDATA_SIZE
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
typedef enum{
	e_DB_NoError		=	0x00,
	e_DB_P1CRCError		=	0x01,
	e_DB_P2CRCError		=	0x02,
	e_DB_AllCRCError	=	0x03,
} SS_DB_ERROR;

typedef enum{
	e_DB_Page1	=	0x01,
	e_DB_Page2	=	0x02,
} SS_DB_PAGE_NUM;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static SS_DB_ERROR ss_DB_check_data(void);
static ErrorStatus ss_DB_write_a_page(SS_DB_PAGE_NUM ePage, uint8_t *pau8Data);
static ErrorStatus ss_DB_recovery_page(SS_DB_PAGE_NUM ePage);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static SS_DB_ERROR ss_DB_check_data(void)
{
	uint16_t u16P1CRC;
	uint16_t u16P2CRC;
	uint16_t u16P1CRCRev;
	uint16_t u16P1CRCPre;
	uint16_t u16P2CRCRev;
	uint16_t u16P2CRCPre;
	SS_DB_ERROR eRet;

	u16P1CRCPre = *(uint16_t *)(SS_DB_PAGE1_ADDR + SS_DB_CRCVALUE_DEX + 2);
	u16P1CRCRev = *(uint16_t *)(SS_DB_PAGE1_ADDR + SS_DB_CRCVALUE_DEX);
	u16P2CRCPre = *(uint16_t *)(SS_DB_PAGE2_ADDR + SS_DB_CRCVALUE_DEX + 2);
	u16P2CRCRev = *(uint16_t *)(SS_DB_PAGE2_ADDR + SS_DB_CRCVALUE_DEX);

	u16P1CRC = util_CRC16Cal((uint8_t *)SS_DB_PAGE1_ADDR, (SS_DB_PAGE_SIZE - 4));
	u16P2CRC = util_CRC16Cal((uint8_t *)SS_DB_PAGE2_ADDR, (SS_DB_PAGE_SIZE - 4));

	eRet = e_DB_NoError;
	if((u16P1CRCPre != (u16P1CRCRev ^ 0xFFFF)) || (u16P1CRC != u16P1CRCPre))
	{
		eRet = e_DB_P1CRCError;
	}

	if((u16P2CRCPre != (u16P2CRCRev ^ 0xFFFF)) || (u16P2CRC != u16P2CRCPre))
	{
		if(eRet != e_DB_NoError)
			eRet = e_DB_AllCRCError;
		else
			eRet = e_DB_P2CRCError;
	}

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ss_DB_recovery_page(SS_DB_PAGE_NUM ePage)
{
	uint32_t u32Dex;
	uint16_t *pu16DectAddr;
	uint16_t *pu16ScrAddr;
	uint16_t u16Temp;
	ErrorStatus eRet;

	if(e_DB_Page1 == ePage)
	{
		pu16DectAddr = (uint16_t *)SS_DB_PAGE1_ADDR;
		pu16ScrAddr = (uint16_t *)SS_DB_PAGE2_ADDR;
	}
	else if(e_DB_Page2 == ePage)
	{
		pu16DectAddr = (uint16_t *)SS_DB_PAGE2_ADDR;
		pu16ScrAddr = (uint16_t *)SS_DB_PAGE1_ADDR;
	}
	else
	{
		pu16DectAddr = NULL;
		pu16ScrAddr = NULL;
		return ERROR;
	}

	if((!pu16DectAddr) || (!pu16ScrAddr))
	{
		return ERROR;
	}

	eRet = SUCCESS;

	pal_disable_all_interrupt();

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	if(FLASH_COMPLETE == FLASH_ErasePage((uint32_t)pu16DectAddr))
	{
		for(u32Dex=0; u32Dex<SS_DB_PAGE_SIZE;)
		{
			u16Temp = *pu16ScrAddr;
			if(FLASH_COMPLETE != FLASH_ProgramHalfWord((uint32_t)pu16DectAddr, u16Temp))
			{
				eRet = ERROR;
				break;
			}
			pu16DectAddr++;
			pu16ScrAddr++;
			u32Dex += 2;
		}
	}
	else
	{
		eRet = ERROR;
	}

	FLASH_Lock();

	pal_enable_all_interrupt();

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_DB_get_start_addr(uint32_t u32Offset, uint8_t **ppu8StartAddr)
{
	SS_DB_ERROR eError;

	if(!ppu8StartAddr)
		return ERROR;

	if(SS_DB_PAGE_SIZE <= u32Offset)
		return ERROR;

	eError = ss_DB_check_data();

	// 如果原本有数据有被损坏，先恢复数据
	if(e_DB_P1CRCError == eError)
	{
		UTIL_DEBUG_PRINTF(SS_DB_DEBUG, ("DB recovering page 1!\n\r"));
		if(!ss_DB_recovery_page(e_DB_Page1))
			return ERROR;
	}
	else if(e_DB_P2CRCError == eError)
	{
		UTIL_DEBUG_PRINTF(SS_DB_DEBUG, ("DB recovering page 2!\n\r"));
		if(!ss_DB_recovery_page(e_DB_Page2))
			return ERROR;
	}
	else if(e_DB_AllCRCError == eError)
	{
		UTIL_DEBUG_PRINTF(SS_DB_DEBUG, ("DB don't have valid data!\n\r"));
		return ERROR;		// 无数据，或数据全被损坏
	}

	// 恢复数据后再次检查CRC校验，看是否恢复正确
	eError = ss_DB_check_data();
	if(e_DB_NoError != eError)
	{
		UTIL_DEBUG_PRINTF(SS_DB_DEBUG, ("DB CRC checking error!\n\r"));
		return ERROR;
	}

	*ppu8StartAddr = (uint8_t *)(SS_DB_PAGE1_ADDR + u32Offset);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static ErrorStatus ss_DB_write_a_page(SS_DB_PAGE_NUM ePage, uint8_t *pau8Data)
{
	uint32_t u32Dex;
	ErrorStatus eRet;
	uint16_t *pu16DectAddr;
	uint16_t *pu16ScrAddr;
	uint16_t u16Temp;

	if(!pau8Data)
		return ERROR;

	pu16ScrAddr = (uint16_t *)pau8Data;

	switch(ePage)
	{
	case e_DB_Page1:
		pu16DectAddr = (uint16_t *)SS_DB_PAGE1_ADDR;
		break;
	case e_DB_Page2:
		pu16DectAddr = (uint16_t *)SS_DB_PAGE2_ADDR;
		break;
	default:
		return ERROR;
		break;
	}

	eRet = SUCCESS;

	pal_disable_all_interrupt();

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	if(FLASH_COMPLETE == FLASH_ErasePage((uint32_t)pu16DectAddr))
	{
		for(u32Dex=0; u32Dex<SS_DB_USERDATA_SIZE;)
		{
			u16Temp = *pu16ScrAddr;
			if(FLASH_COMPLETE != FLASH_ProgramHalfWord((uint32_t)pu16DectAddr, u16Temp))
			{
				eRet = ERROR;
				goto Out;
			}
			pu16DectAddr++;
			pu16ScrAddr++;
			u32Dex += 2;
		}
	}
	else
	{
		eRet = ERROR;
		goto Out;
	}

	// Save CRC
	u16Temp = util_CRC16Cal(pau8Data, SS_DB_USERDATA_SIZE);
	if(FLASH_COMPLETE != FLASH_ProgramHalfWord((uint32_t)pu16DectAddr, (u16Temp ^ 0xFFFF)))
	{
		eRet = ERROR;
		goto Out;
	}
	pu16DectAddr++;
	if(FLASH_COMPLETE != FLASH_ProgramHalfWord((uint32_t)pu16DectAddr, u16Temp))
	{
		eRet = ERROR;
		goto Out;
	}

Out:
	FLASH_Lock();

	pal_enable_all_interrupt();

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_DB_save_a_page_data(uint8_t *pau8Data)
{
	SS_DB_ERROR eError;
//	uint32_t	u32Dex;
//	uint16_t	*pu16Data;

	// 写数据，先写页1，再写页2
	if(!ss_DB_write_a_page(e_DB_Page1, pau8Data))
	{
		UTIL_DEBUG_PRINTF(SS_DB_DEBUG, ("Write page 1 failed!\n\r"));
		return ERROR;
	}

	if(!ss_DB_recovery_page(e_DB_Page2))
	{
		UTIL_DEBUG_PRINTF(SS_DB_DEBUG, ("Save data: recovery page 2 failed!\n\r"));
		return ERROR;
	}

	// 检查CRC校验，看是否写正确
	eError = ss_DB_check_data();
	if(e_DB_NoError != eError)
	{
		UTIL_DEBUG_PRINTF(SS_DB_DEBUG, ("Check CRC failed!\n\r"));
		return ERROR;
	}

//	if(SS_DB_DEBUG)
//	{
//		UTIL_DEBUG_PRINTF(true, ("\n\r-------------page 1---------------\n\r"));
//		pu16Data = (uint16_t *)SS_DB_PAGE1_ADDR;
//		for(u32Dex=0; u32Dex<(SS_DB_PAGE_SIZE>>1); u32Dex++)
//		{
//			if(u32Dex && ((u32Dex & 0x0F) == 0))
//				UTIL_DEBUG_PRINTF(true, ("\n\r"));
//			UTIL_DEBUG_PRINTF(true, ("0x%04X, ", *pu16Data++));
//		}
//		UTIL_DEBUG_PRINTF(true, ("\n\r-------------page 2---------------\n\r"));
//		pu16Data = (uint16_t *)SS_DB_PAGE2_ADDR;
//		for(u32Dex=0; u32Dex<(SS_DB_PAGE_SIZE>>1); u32Dex++)
//		{
//			if(u32Dex && ((u32Dex & 0x0F) == 0))
//				UTIL_DEBUG_PRINTF(true, ("\n\r"));
//			UTIL_DEBUG_PRINTF(true, ("0x%04X, ", *pu16Data++));
//		}
//	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_DB_get_user_datasize(uint32_t *pu32Size)
{
	if(!pu32Size)
		return ERROR;

	*pu32Size = SS_DB_USERDATA_SIZE;

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
