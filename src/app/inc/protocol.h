/*
 * protocol.h
 *
 *  Created on: 2013-4-20
 *      Author: WLD-LuoWenjie
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#if(SYSCFG_DEBUG)
#define PROTOCOL_DEBUG						true
#else
#define PROTOCOL_DEBUG						false
#endif

#define PROTOCOL_PROCESS_BUFF_LEN			512


#define PROTOCOL_DOWN_FRAME_HEAD			0x55

#define PROTOCOL_UP_FRAME_HEAD				0xF0
#define PROTOCOL_UP_FRAME_END				0xFA
#define PROTOCOL_UP_FRAME_TURN				0xF5

#define PROTOCOL_UP_FRAME_HEAD_T			0x50
#define PROTOCOL_UP_FRAME_END_T				0x5A
#define PROTOCOL_UP_FRAME_TURN_T			0x55

#define PROTOCOL_UP_FRAME_LEN_MAX			127


#define PROTOCOL_DOWNKEY_RETURN				0x11
#define PROTOCOL_DOWNKEY_CANCEL				0x12

#define PROTOCOL_UP_READTIME_FLAG			0xA5

#define PROTOCOL_UPDATECFW_DATA_SECTOR		256

typedef enum{
	e_CMD_ReadStatus	=	0x30,
	e_CMD_TestOneCH		=	0x31,
	e_CMD_SetTime		=	0x32,
	e_CMD_GetRandom		=	0x33,
	e_CMD_DownLic		=	0x34,
	e_CMD_ReadSN		=	0x35,
	e_CMD_C12Corr		=	0x36,
	e_CMD_C13Corr		=	0x37,
	e_CMD_CheckFilter	=	0x38,
	e_CMD_ReadADVal		=	0x3A,
	e_CMD_ReadHistory	=	0x3B,
	e_CMD_TestStop		=	0x3C,
	e_CMD_SelfTest		=	0x3D,
	e_CMD_CheckStable	=	0x3E,

	e_CMD_OpenRTOrPump	=	0x40,
	e_CMD_closeRTOrPump	=	0x41,
	e_CMD_ReadTime		=	0x48,
	e_CMD_ReadRemainLic	=	0x49,
	e_CMD_ReadLastResult=	0x4A,
	e_CMD_ReadCurveCoeff=	0x4B,
	e_CMD_SetIdle		=	0x4C,
	e_CMD_DOBCorr		=	0x4D,
	e_CMD_SyncCH		=	0x4E,

	e_CMD_StartUpdateFW	=	0x50,
	e_CMD_UpdateFWData	=	0x51,

	e_CMD_ErrorCMD		=	0xE8,
} E_PROTOCOL_CMD;

typedef enum{
	e_UpdateFW_Error	=	0x30,
	e_UpdateFW_OK		=	0x31,
} E_PROTOCOL_UPDATEFW_BACK;

typedef enum{
	e_UDStatus_None			=	0x00,
	e_UDStatus_Start		=	0x01,
	e_UDStatus_Programming	=	0x02,
	e_UDStatus_End			=	0x03,
} E_PROTOCOL_UPDATEFW_STATUS;

typedef struct{
	uint8_t	u8FrameHead;
	uint8_t	u8SN;
	uint8_t	u8CMD;
	uint8_t	u8Data[22];
	uint8_t	u8CHKSUM;
} PROTOCOL_DOWN_FRAME;

typedef struct{
	uint8_t	u8FrameHead;
	uint8_t	u8SN;
	uint8_t	u8CMD;
	uint8_t	u8Flag;
	uint8_t	u8SUM3;
	uint8_t	u8Data[PROTOCOL_UPDATECFW_DATA_SECTOR];
	uint8_t	u8CHKSUM;
} PROTOCOL_DOWN_UPDATEFWDATA_FRAME;

typedef struct{
	uint8_t						u8Len;
	uint8_t						u8DownCMD;
	uint8_t						u8SN;

	E_PROTOCOL_UPDATEFW_BACK	eStatus;

	uint8_t						u8CHKSUM;
} PROTOCOL_UP_UPDATEFW_FRAME;

typedef struct{
	CTRL_LINKER_PORT			eLinker;

	uint16_t					u16ReadDex;
	uint16_t					u16SendDex;
	uint8_t						au8RevBuffer[PROTOCOL_PROCESS_BUFF_LEN];
	uint8_t						au8SendBuffer[PROTOCOL_PROCESS_BUFF_LEN];

	E_PROTOCOL_UPDATEFW_STATUS	eUpdateFWStatus;
} PROTOCOL_INFO;

ErrorStatus protocol_init(void);

void protocol_thread(void);

E_PROTOCOL_UPDATEFW_STATUS protocol_GetUpdateFWStatus(void);
void protocol_ResetUpdateFWStatus(void);

#endif /* PROTOCOL_H_ */
