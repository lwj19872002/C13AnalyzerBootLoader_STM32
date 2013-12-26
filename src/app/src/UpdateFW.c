/*
 * UpdateFW.c
 *
 *  Created on: 2013-9-6
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"
#include "app.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static ST_UPDATEFW_INFO g_sstUpdateFWInfo;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
 * ���ܣ���ʼ���̼����£���Ҫ�ǲ�����Ҫ��̵�FLASH����
 * ���룺
 *     uint32_t u32TotalSector ------- ��Ҫ���µ���������
 *     uint32_t u32SectorSize  ------- ÿһ�������ֽ���
 * �����
 *     ��
 * ���أ�
 *     ErrorStatus ------ SUCCESS �ɹ���ERROR ʧ�ܡ�
 */
ErrorStatus UpdateFW_Init(uint32_t u32TotalSector, uint32_t u32SectorSize)
{
	ErrorStatus eRet;
	uint32_t	u32TotalSize;
	uint32_t	u32SectorToErase;
	uint32_t	u32I;

	if(!u32TotalSector)
		return ERROR;

	if(!u32SectorSize)
		return ERROR;

	eRet = ERROR;

	u32TotalSize = u32SectorSize * u32TotalSector;
	if(u32TotalSize > (BL_APP_END_ADDR - BL_APP_START_ADDR + 1))
	{
		UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("The file is too big!!! Total size: %d Bytes\n\r", u32TotalSize));
		return ERROR;
	}

	u32SectorToErase = u32TotalSize / UPDATEFW_FLASHPAGE_SIZE;
	if(u32TotalSize%UPDATEFW_FLASHPAGE_SIZE)
		u32SectorToErase += 1;
	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Total pages to erase: %d (2KB per page)\n\r", u32SectorToErase));

	__disable_interrupt();

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	for(u32I=0; u32I<u32SectorToErase; u32I++)
	{
		if(FLASH_COMPLETE == FLASH_ErasePage(BL_APP_START_ADDR + (u32I * UPDATEFW_FLASHPAGE_SIZE)))
		{
			UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Pages %d erase successfully!\n\r", u32I+1));
			eRet = SUCCESS;
		}
		else
		{
			UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Pages %d erase failed!\n\r", u32I+1));
			eRet = ERROR;
			break;
		}
	}

	FLASH_Lock();

	__enable_interrupt();

	g_sstUpdateFWInfo.u32WriteAddrOffset = 0;

	return eRet;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*
 * ���ܣ����FLASH
 * ���룺
 *     const uint8_t *pau8SrcData ----- ��̵�FLASH��ԭʼ����
 *     uint32_t u32DataLen ------------ ԭʼ�����ܳ��ȣ��ֽڣ�
 * �����
 *     ��
 * ���أ�
 *     ErrorStatus ------ SUCCESS �ɹ���ERROR ʧ�ܡ�
 */
ErrorStatus UpdateFW_ProgramData(const uint8_t *pau8SrcData, uint32_t u32DataLen)
{
	uint32_t	u32I;
	ErrorStatus	eRet;
	uint32_t	u32StartAddr;

	if((!pau8SrcData) || (!u32DataLen))
		return ERROR;

	// ����������ֽ������ܾ�дFLASH
	if(u32DataLen & 0x00000001)
		return ERROR;

	// ����Ƿ������Ӧ�ó���ĵ�ַ��Χ
	u32StartAddr = BL_APP_START_ADDR + g_sstUpdateFWInfo.u32WriteAddrOffset;
	if((u32StartAddr + u32DataLen) > BL_APP_END_ADDR)
		return ERROR;

	__disable_interrupt();

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	for(u32I=0; u32I<(u32DataLen >> 1); u32I++)
	{
		if(FLASH_COMPLETE == FLASH_ProgramHalfWord(u32StartAddr + (u32I << 1), *((uint16_t *)&pau8SrcData[u32I << 1])))
		{
			eRet = SUCCESS;
		}
		else
		{
			UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Program halfword failed! Add: 0x%X\n\r", u32StartAddr + (u32I << 1)));
			eRet = ERROR;
			break;
		}
	}

	FLASH_Lock();

	__enable_interrupt();

	if(eRet)
	{
		// ���������Ƿ�д��ȷ
		for(u32I=0; u32I<(u32DataLen >> 1); u32I++)
		{
			if(*((uint16_t *)&pau8SrcData[u32I << 1]) != *(uint16_t *)(u32StartAddr + (u32I << 1)))
			{
				UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Verify failed! Add:0x%X Value: 0x%X SrcData: 0x%X\n\r", \
						u32StartAddr + (u32I << 1), *(uint16_t *)(u32StartAddr + (u32I << 1)), *((uint16_t *)&pau8SrcData[u32I << 1])));
				eRet = ERROR;
				break;
			}
		}
	}

	if(eRet)
	{
		UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Program a sector successfully! StartAddr:0x%X DataLen: %dBytes\n\r", u32StartAddr, u32DataLen));
		g_sstUpdateFWInfo.u32WriteAddrOffset += u32DataLen;
	}

	return eRet;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
