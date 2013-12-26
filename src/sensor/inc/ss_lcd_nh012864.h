/*
 * ss_lcd_nh012864.h
 *
 *  Created on: 2013-4-2
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_LCD_NH012864_H_
#define SS_LCD_NH012864_H_

typedef struct{
	GPIO_TypeDef		*pCSPort;
	uint16_t			u16CSPin;

	GPIO_TypeDef		*pCDPort;
	uint16_t			u16CDPin;

	GPIO_TypeDef		*pWRPort;
	uint16_t			u16WRPin;

	GPIO_TypeDef		*pRDPort;
	uint16_t			u16RDPin;

	GPIO_TypeDef		*pRESTPort;
	uint16_t			u16RESTPin;

	GPIO_TypeDef		*pDBusPort;
	uint16_t			u16DBusPin;
	uint8_t				u8DBusDex;

	uint32_t			u32APB2Clk;
	uint32_t			u32APB1Clk;
	uint32_t			u32AHBClk;
} SS_LCD_NH012864_CFG;

typedef struct{
	const SS_LCD_NH012864_CFG *pPalConfig;
} SS_LCD_NH012864_INFO;

__STATIC_INLINE void ss_lcd_nh012864_dbus_output(SS_LCD_NH012864_INFO *pLCDInfo)
{
	uint32_t u32Temp;

	u32Temp = pLCDInfo->pPalConfig->pDBusPort->MODER;

	if(0x00005555 == ((u32Temp >> (pLCDInfo->pPalConfig->u8DBusDex * 2)) & 0x0000FFFF))
		return;

	u32Temp = (u32Temp & (~(0x0000FFFF << (pLCDInfo->pPalConfig->u8DBusDex * 2)))) \
			| (0x00005555 << (pLCDInfo->pPalConfig->u8DBusDex * 2));
	pLCDInfo->pPalConfig->pDBusPort->MODER = u32Temp;
}

__STATIC_INLINE void ss_lcd_nh012864_dbus_input(SS_LCD_NH012864_INFO *pLCDInfo)
{
	uint32_t u32Temp;

	u32Temp = pLCDInfo->pPalConfig->pDBusPort->MODER;

	if(0x00000000 == ((u32Temp >> (pLCDInfo->pPalConfig->u8DBusDex * 2)) & 0x0000FFFF))
		return;

	u32Temp = u32Temp & (~(0x0000FFFF << (pLCDInfo->pPalConfig->u8DBusDex * 2)));
	pLCDInfo->pPalConfig->pDBusPort->MODER = u32Temp;
}

__STATIC_INLINE void ss_lcd_nh012864_dbus_read(SS_LCD_NH012864_INFO *pLCDInfo, uint8_t *pu8Data)
{
	uint16_t u16Temp;

	u16Temp = pLCDInfo->pPalConfig->pDBusPort->IDR;
	*pu8Data = (uint8_t)(u16Temp >> pLCDInfo->pPalConfig->u8DBusDex);
}

__STATIC_INLINE void ss_lcd_nh012864_dbus_write(SS_LCD_NH012864_INFO *pLCDInfo, uint8_t u8Data)
{
	uint16_t u16Temp;

	u16Temp = pLCDInfo->pPalConfig->pDBusPort->ODR;
	u16Temp = (u16Temp & (~(0x00FF << pLCDInfo->pPalConfig->u8DBusDex))) | ((uint16_t)u8Data << pLCDInfo->pPalConfig->u8DBusDex);
	pLCDInfo->pPalConfig->pDBusPort->ODR = u16Temp;
}

// Rest Pin
__STATIC_INLINE void ss_lcd_nh012864_rest_low(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_ResetBits(pLCDInfo->pPalConfig->pRESTPort, pLCDInfo->pPalConfig->u16RESTPin);
}

__STATIC_INLINE void ss_lcd_nh012864_rest_high(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_SetBits(pLCDInfo->pPalConfig->pRESTPort, pLCDInfo->pPalConfig->u16RESTPin);
}

// CS Pin
__STATIC_INLINE void ss_lcd_nh012864_cs_low(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_ResetBits(pLCDInfo->pPalConfig->pCSPort, pLCDInfo->pPalConfig->u16CSPin);
}

__STATIC_INLINE void ss_lcd_nh012864_cs_high(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_SetBits(pLCDInfo->pPalConfig->pCSPort, pLCDInfo->pPalConfig->u16CSPin);
}

// CD Pin
__STATIC_INLINE void ss_lcd_nh012864_cd_cmd(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_ResetBits(pLCDInfo->pPalConfig->pCDPort, pLCDInfo->pPalConfig->u16CDPin);
}

__STATIC_INLINE void ss_lcd_nh012864_cd_data(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_SetBits(pLCDInfo->pPalConfig->pCDPort, pLCDInfo->pPalConfig->u16CDPin);
}

// WR Pin
__STATIC_INLINE void ss_lcd_nh012864_wr_low(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_ResetBits(pLCDInfo->pPalConfig->pWRPort, pLCDInfo->pPalConfig->u16WRPin);
}

__STATIC_INLINE void ss_lcd_nh012864_wr_high(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_SetBits(pLCDInfo->pPalConfig->pWRPort, pLCDInfo->pPalConfig->u16WRPin);
}

// RD Pin
__STATIC_INLINE void ss_lcd_nh012864_rd_low(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_ResetBits(pLCDInfo->pPalConfig->pRDPort, pLCDInfo->pPalConfig->u16RDPin);
}

__STATIC_INLINE void ss_lcd_nh012864_rd_high(SS_LCD_NH012864_INFO *pLCDInfo)
{
	GPIO_SetBits(pLCDInfo->pPalConfig->pRDPort, pLCDInfo->pPalConfig->u16RDPin);
}

void ss_lcd_nh012864_fill_screen(uint8_t u8Constrast);

ErrorStatus ss_lcd_nh012864_init(const SS_LCD_NH012864_CFG *pConfig);

void ss_lcd_nh012864_draw_point(uint32_t u8X, uint32_t u8Y, uint8_t u8Constrast);

void ss_lcd_nh012864_draw_area(uint32_t u8X, uint32_t u8Y, uint8_t *pu8Data, uint32_t u8Width, uint32_t u8High, uint8_t u8Constrast, bool bRev);

void ss_lcd_nh012864_get_info(uint32_t *pu32MaxCol, uint32_t *pu32MaxRow);

#endif /* SS_LCD_NH012864_H_ */
