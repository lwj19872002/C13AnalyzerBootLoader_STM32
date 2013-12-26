/*
 * protocol.c
 *
 *  Created on: 2013-4-20
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"
#include "app.h"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

static PROTOCOL_INFO g_sstProtocolInfo;
static uint8_t g_sau8ProtocolBuff[PROTOCOL_PROCESS_BUFF_LEN/2];
static uint8_t g_sau8PackageBuff[PROTOCOL_PROCESS_BUFF_LEN];

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void protocol_send_data_thread(void);
static ErrorStatus protocol_add_send_data_to_buff(uint8_t *pau8Data, uint16_t u16Len);
static void protocol_send_package(uint8_t *pu8Frame, uint32_t u32Len);

static void protocol_analyze_frame(void);
static void protocol_process_frame(void *pFrame);

static ErrorStatus protocol_package_up_frame(uint8_t *pau8DecBuff, const uint8_t *pau8SrcBuff, uint32_t u32LenToPackage, uint32_t *pu32PackageLen);

static void protocol_process_StartUpdateFW(PROTOCOL_DOWN_FRAME *pFrame);
static void protocol_process_UpdateFWData(PROTOCOL_DOWN_UPDATEFWDATA_FRAME *pFrame);
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
ErrorStatus protocol_init(void)
{
	memset(&g_sstProtocolInfo, 0x00, sizeof(PROTOCOL_INFO));
	g_sstProtocolInfo.eLinker	=	e_linker_P1;
	g_sstProtocolInfo.u16ReadDex = 0;
	g_sstProtocolInfo.u16SendDex = 0;

	g_sstProtocolInfo.eUpdateFWStatus = e_UDStatus_None;

	ctrl_event_add_10ms_thread(protocol_thread);

	return SUCCESS;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void protocol_send_data_thread(void)
{
	uint16_t	u16SendLen;

	if(!g_sstProtocolInfo.u16SendDex)
		return;

	u16SendLen = (PAL_COM_BUFFER_LEN > g_sstProtocolInfo.u16SendDex)?g_sstProtocolInfo.u16SendDex:PAL_COM_BUFFER_LEN;

	if(ctrl_linker_send(g_sstProtocolInfo.eLinker, g_sstProtocolInfo.au8SendBuffer, u16SendLen))
	{
		if(g_sstProtocolInfo.u16SendDex > u16SendLen)
			g_sstProtocolInfo.u16SendDex -= u16SendLen;
		else
			g_sstProtocolInfo.u16SendDex = 0;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static ErrorStatus protocol_add_send_data_to_buff(uint8_t *pau8Data, uint16_t u16Len)
{
	if((!pau8Data) || (!u16Len))
		return ERROR;

	if((PROTOCOL_PROCESS_BUFF_LEN - g_sstProtocolInfo.u16SendDex) > u16Len)
	{
		memcpy(&g_sstProtocolInfo.au8SendBuffer[g_sstProtocolInfo.u16SendDex], pau8Data, u16Len);
		g_sstProtocolInfo.u16SendDex += u16Len;
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void protocol_send_package(uint8_t *pu8Frame, uint32_t u32Len)
{
	uint8_t		u8CHKSUM;
	uint32_t	u32PackLen;

	if(!util_calculate_checksum(pu8Frame, u32Len-1, &u8CHKSUM))
	{
		return;
	}
	pu8Frame[u32Len-1] = u8CHKSUM;

	if(!protocol_package_up_frame(g_sau8PackageBuff, (uint8_t *)pu8Frame, u32Len - 1, &u32PackLen))
		return;

	if(u32PackLen)
		protocol_add_send_data_to_buff(g_sau8PackageBuff, u32PackLen);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void protocol_thread(void)
{
	uint16_t	u16ReadLen;

	u16ReadLen = 0;
	if(ctrl_linker_read(g_sstProtocolInfo.eLinker, \
						&g_sstProtocolInfo.au8RevBuffer[g_sstProtocolInfo.u16ReadDex], \
						PROTOCOL_PROCESS_BUFF_LEN - g_sstProtocolInfo.u16ReadDex, \
						&u16ReadLen))
	{
		if(u16ReadLen)
		{
			g_sstProtocolInfo.u16ReadDex += u16ReadLen;
		}
	}

	if(g_sstProtocolInfo.u16ReadDex)
	{
		protocol_analyze_frame();
	}

	protocol_send_data_thread();
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void protocol_analyze_frame(void)
{
	uint16_t u16Dex;
	uint16_t u16I;
	uint8_t u8CHKSum;

	u16Dex = 0;

	if(g_sstProtocolInfo.u16ReadDex >= sizeof(PROTOCOL_DOWN_FRAME))
	{
		while(1)
		{
			if(PROTOCOL_DOWN_FRAME_HEAD == g_sstProtocolInfo.au8RevBuffer[u16Dex])
			{
				break;
			}

			if(u16Dex >= g_sstProtocolInfo.u16ReadDex)
			{
				break;
			}

			u16Dex++;
		}

		if((g_sstProtocolInfo.u16ReadDex - u16Dex) >= sizeof(PROTOCOL_DOWN_FRAME))
		{
			if(((PROTOCOL_DOWN_FRAME *)&g_sstProtocolInfo.au8RevBuffer[u16Dex])->u8CMD != e_CMD_UpdateFWData)
			{
				if(util_calculate_checksum(&g_sstProtocolInfo.au8RevBuffer[u16Dex+1], (sizeof(PROTOCOL_DOWN_FRAME)-2), &u8CHKSum))
				{
					if(((PROTOCOL_DOWN_FRAME *)&g_sstProtocolInfo.au8RevBuffer[u16Dex])->u8CHKSUM == u8CHKSum)
					{
						protocol_process_frame(&g_sstProtocolInfo.au8RevBuffer[u16Dex]);
					}
				}

				u16Dex += sizeof(PROTOCOL_DOWN_FRAME);
			}
			else
			{
				if((g_sstProtocolInfo.u16ReadDex - u16Dex) >= sizeof(PROTOCOL_DOWN_UPDATEFWDATA_FRAME))
				{

					protocol_process_frame(&g_sstProtocolInfo.au8RevBuffer[u16Dex]);

					u16Dex += sizeof(PROTOCOL_DOWN_UPDATEFWDATA_FRAME);
				}
			}
		}

		if(u16Dex)
		{
			if(u16Dex >= g_sstProtocolInfo.u16ReadDex)
			{
				g_sstProtocolInfo.u16ReadDex = 0;
			}
			else
			{
				for(u16I=0; u16I<(g_sstProtocolInfo.u16ReadDex - u16Dex); u16I++)
				{
					g_sstProtocolInfo.au8RevBuffer[u16I] = g_sstProtocolInfo.au8RevBuffer[u16Dex + u16I];
				}

				g_sstProtocolInfo.u16ReadDex -= u16Dex;
			}
		}
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void protocol_process_frame(void *pFrame)
{
	PROTOCOL_DOWN_FRAME *pstDownFrame;

	if(!pFrame)
		return;

	pstDownFrame = (PROTOCOL_DOWN_FRAME *)pFrame;

	switch(pstDownFrame->u8CMD)
	{
	case e_CMD_StartUpdateFW:
		protocol_process_StartUpdateFW(pstDownFrame);
		break;
	case e_CMD_UpdateFWData:
		protocol_process_UpdateFWData((PROTOCOL_DOWN_UPDATEFWDATA_FRAME *)pstDownFrame);
		break;
	default:
		break;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static ErrorStatus protocol_package_up_frame(uint8_t *pau8DecBuff, const uint8_t *pau8SrcBuff, uint32_t u32LenToPackage, uint32_t *pu32PackageLen)
{
	uint32_t u32Dex;
	uint32_t u32I;

	if((!pau8DecBuff) || (!pau8SrcBuff) || (!pu32PackageLen))
		return ERROR;

	if((!u32LenToPackage) || (PROTOCOL_UP_FRAME_LEN_MAX<u32LenToPackage))
		return ERROR;

	u32Dex=0;

	pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_HEAD;
	pau8DecBuff[u32Dex++] = u32LenToPackage;
	for(u32I=1; u32I<u32LenToPackage; u32I++)
	{
		switch(pau8SrcBuff[u32I])
		{
		case PROTOCOL_UP_FRAME_HEAD:
			pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_TURN;
			pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_HEAD_T;
			break;
		case PROTOCOL_UP_FRAME_END:
			pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_TURN;
			pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_END_T;
			break;
		case PROTOCOL_UP_FRAME_TURN:
			pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_TURN;
			pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_TURN_T;
			break;
		default:
			pau8DecBuff[u32Dex++] = pau8SrcBuff[u32I];
			break;
		}
	}
	pau8DecBuff[u32Dex++] = PROTOCOL_UP_FRAME_END;
	*pu32PackageLen = u32Dex;

	return SUCCESS;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void protocol_process_StartUpdateFW(PROTOCOL_DOWN_FRAME *pFrame)
{
	uint16_t					u16TotalSector;
	PROTOCOL_UP_UPDATEFW_FRAME	*pstFrame;

	if(!pFrame)
		return;

	u16TotalSector = *((uint16_t *)&pFrame->u8Data[0]);
	UTIL_DEBUG_PRINTF(SYSCFG_DEBUG, ("Start update software. Total sectors:%d Sector size(Bytes): %d\n\r",u16TotalSector, PROTOCOL_UPDATECFW_DATA_SECTOR));

	pstFrame = (PROTOCOL_UP_UPDATEFW_FRAME *)g_sau8ProtocolBuff;
	pstFrame->u8Len		=	sizeof(PROTOCOL_UP_UPDATEFW_FRAME)-1;
	pstFrame->u8DownCMD	=	pFrame->u8CMD;
	pstFrame->u8SN		=	pFrame->u8SN;

	if(UpdateFW_Init(u16TotalSector, PROTOCOL_UPDATECFW_DATA_SECTOR))
	{
		g_sstProtocolInfo.eUpdateFWStatus = e_UDStatus_Start;
		pstFrame->eStatus	=	e_UpdateFW_OK;
	}
	else
	{
		pstFrame->eStatus	=	e_UpdateFW_Error;
	}

	protocol_send_package((uint8_t *)pstFrame, sizeof(PROTOCOL_UP_UPDATEFW_FRAME));
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
static void protocol_process_UpdateFWData(PROTOCOL_DOWN_UPDATEFWDATA_FRAME *pFrame)
{
	PROTOCOL_UP_UPDATEFW_FRAME			*pstFrame;
	uint8_t								u8CHKSum;

	if(!pFrame)
		return;

	pstFrame = (PROTOCOL_UP_UPDATEFW_FRAME *)g_sau8ProtocolBuff;
	pstFrame->u8Len		=	sizeof(PROTOCOL_UP_UPDATEFW_FRAME)-1;
	pstFrame->u8DownCMD	=	pFrame->u8CMD;
	pstFrame->u8SN		=	pFrame->u8SN;

	if(util_calculate_checksum(&pFrame->u8SN, (sizeof(PROTOCOL_DOWN_UPDATEFWDATA_FRAME)-2), &u8CHKSum))
	{
		if(pFrame->u8CHKSUM == u8CHKSum)
		{
			g_sstProtocolInfo.eUpdateFWStatus = e_UDStatus_Programming;

			if(SUCCESS == UpdateFW_ProgramData(pFrame->u8Data, PROTOCOL_UPDATECFW_DATA_SECTOR))
			{
				pstFrame->eStatus	=	e_UpdateFW_OK;
			}
			else
			{
				pstFrame->eStatus	=	e_UpdateFW_Error;
			}
		}
		else
		{
			pstFrame->eStatus	=	e_UpdateFW_Error;
		}
	}

	if(pFrame->u8Flag == 0x5A)
	{
		g_sstProtocolInfo.eUpdateFWStatus = e_UDStatus_End;
	}

	protocol_send_package((uint8_t *)pstFrame, sizeof(PROTOCOL_UP_UPDATEFW_FRAME));
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
E_PROTOCOL_UPDATEFW_STATUS protocol_GetUpdateFWStatus(void)
{
	E_PROTOCOL_UPDATEFW_STATUS eRet;

	eRet = g_sstProtocolInfo.eUpdateFWStatus;

	return eRet;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void protocol_ResetUpdateFWStatus(void)
{
	g_sstProtocolInfo.eUpdateFWStatus = e_UDStatus_None;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
