/*
 * BootLoader.h
 *
 *  Created on: 2013-9-5
 *      Author: WLD-LuoWenjie
 */

/*   STM32F303VCT6 MCU Flash ��ַ���䡣 ��FLASH
 *
 *   0x0803 8000  ----  0x0803 FFFF       DataBase2   (1KB)
 *   0x0803 F000  ----  0x0803 F7FF       DataBase1   (1KB)
 *   0x0800 7000  ----  0x0803 EFFF       Application (198KB)
 *   0x0800 0000  ----  0x0800 6FFF       BootLoader  (56KB)
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#define BL_MCUFLASH_BASE			0x08000000		// MCU�ڲ�FLASH��ʼ��ַ
#define BL_APP_START_ADDR			0x08007000		// Ӧ�ó�����ʼ��ַ
#define BL_APP_END_ADDR				0x0803EFFF		// Ӧ�ó��������ַ

#define BL_WAITCMD_TIMEOUT				50  // 5s

typedef enum{
	e_BL_Init			=	0x00,		// ��ʼ���׶�
	e_BL_WaitCMD		=	0x01,		// �ȴ���������˽׶εȴ�3�룬���û��������Զ�����Ӧ�ó���
	e_BL_MannulControl	=	0x02,		// �ֶ�����ģʽ
	e_BL_BootApp		=	0x03,		// ����Ӧ�ó���
	e_BL_UpdateApp		=	0x04,		// ����Ӧ�ó���
	e_BL_Error			=	0xFE,		// ����ģʽ������ʱ���˽׶�
} E_BL_THREADSTATUS;

typedef enum{
	e_WaitCMDR_None				=	0x00,
	e_WaitCMDR_MannulControl	=	0x01,
	e_WaitCMDR_UpdateApp		=	0x02,
	e_WaitCMDR_BootApp			=	0x03,
} E_BL_WAITCMD_RETURN;

typedef enum{
	e_WaitCMD_Init			=	0x00,
	e_WaitCMD_DelayInit		=	0x01,
	e_WaitCMD_Delay			=	0x02,
	e_WaitCMD_CheckCMD		=	0x03,
	e_WaitCMD_TimeOut		=	0x04,
} E_BL_WAITCMDSTATUS;

typedef struct{
	E_BL_THREADSTATUS		eThreadStatus;

	E_BL_WAITCMDSTATUS		eWaitCMDStatus;
	uint32_t				u32WaitCMDFlag;
} ST_BL_INFO;

ErrorStatus BL_Initialize(void);
void BL_Thread(void);

#endif /* BOOTLOADER_H_ */
