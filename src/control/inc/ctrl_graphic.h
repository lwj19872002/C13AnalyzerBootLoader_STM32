/*
 * ctrl_graphic.h
 *
 *  Created on: 2013-4-4
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_GRAPHIC_H_
#define CTRL_GRAPHIC_H_

#if(SYSCFG_DEBUG)
#define CTRL_GP_DEBUG				true
#else
#define CTRL_GP_DEBUG				false
#endif
//---------------------------------------------------------------------
#define CTRL_GP_SDFLASH_FONT		1			// 使用SD卡存放的字库
#define CTRL_GP_LOCALFLASH_FONT		2			// 使用本地FLASH存放的字库

#define CTRL_GP_FONT_LOCATION		CTRL_GP_LOCALFLASH_FONT
//---------------------------------------------------------------------

#define CTRL_GP_ZIKU_FILE_PATH		"\\ziku\\songti.hzk"

#define CTRL_GP_LCD_NHO12864		1
#define CTRL_GP_LCD_LKX				2

#define CTRL_GP_USE_LCD			CTRL_GP_LCD_NHO12864

#if(CTRL_GP_USE_LCD == CTRL_GP_LCD_NHO12864)

#include "ss_lcd_nh012864.h"

typedef SS_LCD_NH012864_CFG CTRL_SS_LCD_CFG;

#elif(CTRL_GP_USE_LCD == CTRL_GP_LCD_LKX)

#endif
typedef void (* P_LCD_DRAW_AREA)(uint32_t u32X, uint32_t u32Y, uint8_t *pu8Data, uint32_t u32Width, uint32_t u32High, uint8_t u8Constrast, bool bRev);
typedef void (* P_LCD_DRAW_POINT)(uint32_t u32X, uint32_t u32Y, uint8_t u8Constrast);
typedef void (* P_LCD_FILL_SCREEN)(uint8_t u8Constrast);
typedef ErrorStatus (* P_LCD_INIT)(const CTRL_SS_LCD_CFG *pConfig);

typedef struct{
	P_LCD_INIT			pLCDInit;
	P_LCD_FILL_SCREEN	pLCDFillScreen;
	P_LCD_DRAW_POINT	pLCDDrawPoint;
	P_LCD_DRAW_AREA		pLCDDrawArea;
} CTRL_GP_SENSOR_INTERFACE;

typedef struct{
	uint32_t					u32LCDMaxCol;
	uint32_t 					u32LCDMaxRow;

#if(CTRL_GP_FONT_LOCATION == CTRL_GP_SDFLASH_FONT)
	struct znFAT_FileInfoStruct stGB2312ZiKu;
#endif
	bool						bZiKuOK;

	uint8_t 					au8ZiKuBuffer[256];
} CTRL_GP_INFO;

ErrorStatus ctrl_gp_init(void);
ErrorStatus ctrl_gp_draw_line(uint32_t u32SX, uint32_t u32SY, uint32_t u32EX, uint32_t u32EY, uint8_t u8Constrast);
ErrorStatus ctrl_gp_draw_text(uint32_t u32X, uint32_t u32Y, const uint8_t *pText, uint8_t u8Constrast, bool bRev, uint32_t u32Width, uint32_t u32Hight);
void ctrl_gp_clear_screen(void);

#endif /* CTRL_GRAPHIC_H_ */
