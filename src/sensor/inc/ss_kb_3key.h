/*
 * ss_kb_3key.h
 *
 *  Created on: 2013-4-13
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_KB_3KEY_H_
#define SS_KB_3KEY_H_

#define SS_KB_3KEY_SHAKE_DELAY			5			// 延时去抖动， 100ms（线程每10ms调用一次）

typedef struct {
	GPIO_TypeDef		*GpioCOMPort;
	uint16_t			u16GpioCOMPin;

	uint16_t			u16GpioK1Pin;
	uint8_t				u8K1PinShift;

	uint16_t			u16GpioK2Pin;
	uint8_t				u8K2PinShift;

	uint16_t			u16GpioK3Pin;
	uint8_t				u8K3PinShift;

	uint32_t			u32APB2Clk;
	uint32_t			u32APB1Clk;
	uint32_t			u32AHBClk;
} SS_KB_3KEY_CFG;

typedef struct{
	const SS_KB_3KEY_CFG	*pSSConfig;

	uint16_t				u16IOMask;		// 掩码，屏蔽不用的引脚及公共引脚

	SS_KB_KEY_VALUE			eKeyValue;

} SS_KB_3KEY_INFO;

ErrorStatus ss_kb_3key_init(const SS_KB_3KEY_CFG *pConfig);

// 按键扫描线程，必须每10ms调用一次
void ss_kb_3key_thread(bool *pbNewKey);

ErrorStatus ss_kb_3key_read_key(SS_KB_KEY_VALUE *peKeyValue);

#endif /* SS_KB_3KEY_H_ */
