/*
 * ctrl_graphic.c
 *
 *  Created on: 2013-4-4
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"
#include "controls.h"

#ifdef CTRL_GRAPHIC

static CTRL_GP_SENSOR_INTERFACE g_sstSensorIF;
static CTRL_GP_INFO g_sstGPInfo;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_gp_init(void)
{
	ErrorStatus eRet;
#if(CTRL_GP_FONT_LOCATION == CTRL_GP_SDFLASH_FONT)
	uint8_t u8Ret;
#endif

	memset(&g_sstSensorIF, 0x00, sizeof(CTRL_GP_SENSOR_INTERFACE));
	memset(&g_sstGPInfo, 0x00, sizeof(CTRL_GP_INFO));

	eRet = ERROR;

#if(CTRL_GP_USE_LCD == CTRL_GP_LCD_NHO12864)
	g_sstSensorIF.pLCDInit = ss_lcd_nh012864_init;
	g_sstSensorIF.pLCDFillScreen = ss_lcd_nh012864_fill_screen;
	g_sstSensorIF.pLCDDrawPoint = ss_lcd_nh012864_draw_point;
	g_sstSensorIF.pLCDDrawArea = ss_lcd_nh012864_draw_area;

	eRet = (*g_sstSensorIF.pLCDInit)(&g_cstLCDNH012864Cfg);

	if(eRet)
		ss_lcd_nh012864_get_info(&g_sstGPInfo.u32LCDMaxCol, &g_sstGPInfo.u32LCDMaxRow);

	UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("LCD Type: %s\n\r", UTIL_STRING(CTRL_GP_LCD_NHO12864)));
	UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("Max Column:%d  Max Row:%d\n\r", g_sstGPInfo.u32LCDMaxCol, g_sstGPInfo.u32LCDMaxRow));
#else
	return ERROR;
#endif

#if(CTRL_GP_FONT_LOCATION == CTRL_GP_SDFLASH_FONT)
	u8Ret = znFAT_Open_File(&g_sstGPInfo.stGB2312ZiKu, CTRL_GP_ZIKU_FILE_PATH, 0, 1);
	if(u8Ret != 0)
	{
		g_sstGPInfo.bZiKuOK = false;
		UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("Failed to open the file:%s\n\r", CTRL_GP_ZIKU_FILE_PATH));
		return ERROR;
	}
	else
	{
		g_sstGPInfo.bZiKuOK = true;
	}
#elif(CTRL_GP_FONT_LOCATION == CTRL_GP_LOCALFLASH_FONT)
	g_sstGPInfo.bZiKuOK = true;
#else
	g_sstGPInfo.bZiKuOK = false;
#endif

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_gp_draw_line(uint32_t u32SX, uint32_t u32SY, uint32_t u32EX, uint32_t u32EY, uint8_t u8Constrast)
{
	uint32_t u32PX, u32PY;
	float fK, fB;
	uint32_t u32Dex;
	uint32_t u32Width, u32StartX;

	if((u32SX > g_sstGPInfo.u32LCDMaxCol) || (u32EX > g_sstGPInfo.u32LCDMaxCol))
		return ERROR;
	if((u32SY > g_sstGPInfo.u32LCDMaxRow) || (u32EY > g_sstGPInfo.u32LCDMaxRow))
		return ERROR;

	fK = ((float)u32EY - (float)u32SY) / ((float)u32EX - (float)u32SX);
	fB = (float)u32SY - fK * (float)u32SX;

	if(u32SX < u32EX)
	{
		u32Width = u32EX - u32SX;
		u32StartX = u32SX;
	}
	else
	{
		u32Width = u32SX - u32EX;
		u32StartX = u32EX;
	}

	for(u32Dex=0; u32Dex<u32Width; u32Dex++)
	{
		u32PX = u32StartX + u32Dex;
		u32PY = (uint32_t)(u32PX * fK + fB);
		(*g_sstSensorIF.pLCDDrawPoint)(u32PX, u32PY, u8Constrast);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ctrl_gp_clear_screen(void)
{
	(*g_sstSensorIF.pLCDFillScreen)(0x00);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ctrl_gp_draw_text(uint32_t u32X, uint32_t u32Y, const uint8_t *pText, uint8_t u8Constrast, bool bRev, uint32_t u32Width, uint32_t u32Hight)
{
#if(CTRL_GP_FONT_LOCATION == CTRL_GP_SDFLASH_FONT)
	uint32_t	u32Temp;
	uint32_t	u32ZiKuAddr;
	uint32_t	u32LenToRead;
#endif
	uint32_t	u32Dex;
	uint32_t	u32TextX;
	bool		bRet;

	if(!g_sstGPInfo.bZiKuOK)
		return ERROR;

	u32Dex = 0;
#if(CTRL_GP_FONT_LOCATION == CTRL_GP_SDFLASH_FONT)
	u32LenToRead = (u32Width / 8 * u32Hight);
#endif
	u32TextX = u32X;

//	u8Ret = znFAT_Open_File(&g_sstGPInfo.stGB2312ZiKu, CTRL_GP_ZIKU_FILE_PATH, 0, 1);
//	if(u8Ret != 0)
//	{
//		UTIL_DEBUG_PRINTF(CTRL_GP_DEBUG, ("Failed to open the file:%s\n\r", CTRL_GP_ZIKU_FILE_PATH));
//		return ERROR;
//	}

	while((*(pText+u32Dex) != '\0'))
	{
		if(*(pText+u32Dex) < 0xA1)
		{
			//
#if(CTRL_GP_FONT_LOCATION == CTRL_GP_SDFLASH_FONT)
			bRet = false;
#elif(CTRL_GP_FONT_LOCATION == CTRL_GP_LOCALFLASH_FONT)
			bRet = FontLib_Get8X16FontData(pText+u32Dex, g_sstGPInfo.au8ZiKuBuffer);
#else
			return ERROR;
#endif

			if(bRet)
			{
				(*g_sstSensorIF.pLCDDrawArea)(u32TextX, u32Y, g_sstGPInfo.au8ZiKuBuffer, u32Width >> 1, u32Hight, u8Constrast, bRev);
				u32TextX += (u32Width >> 1);
			}

			u32Dex += 1;
		}
		else if(*(pText+u32Dex+1) != '\0')
		{
#if(CTRL_GP_FONT_LOCATION == CTRL_GP_SDFLASH_FONT)
			u32Temp = (((uint32_t)*(pText+u32Dex)) << 8) | (uint32_t)*(pText+u32Dex+1);
			// ×Ö¿â¹ýÂËµôÁË¿Õ°××Ö·û
//			u32ZiKuAddr = (((u32Temp >> 8) - 0xA1) * 94 + (u32Temp & 0x000000FF) - 0xA1) * u32LenToRead;
			u32ZiKuAddr = (((*(pText+u32Dex)) - 0xA1) * 94 + ((*(pText+u32Dex+1)) & 0x00FF) - 0xA1) * u32LenToRead;

			znFAT_Seek_File(&g_sstGPInfo.stGB2312ZiKu, 0);
			u32Temp = znFAT_Read_File(&g_sstGPInfo.stGB2312ZiKu, u32ZiKuAddr, u32LenToRead, g_sstGPInfo.au8ZiKuBuffer);
			if(u32Temp != u32LenToRead)
			{
				return ERROR;
			}

			bRet = true;
#elif(CTRL_GP_FONT_LOCATION == CTRL_GP_LOCALFLASH_FONT)
			bRet = FontLib_Get16X16FontData(pText+u32Dex, g_sstGPInfo.au8ZiKuBuffer);
#else
			return ERROR;
#endif

			if(bRet)
			{
				(*g_sstSensorIF.pLCDDrawArea)(u32TextX, u32Y, g_sstGPInfo.au8ZiKuBuffer, u32Width, u32Hight, u8Constrast, bRev);
				u32TextX += u32Width;
			}

			u32Dex += 2;
		}
		else
		{
			u32Dex += 1;
		}
	}

	//znFAT_File_Close(&g_sstGPInfo.stGB2312ZiKu);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

#endif
