/*
 * ctrl_keyboard.h
 *
 *  Created on: 2013-4-15
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_KEYBOARD_H_
#define CTRL_KEYBOARD_H_

#if(SYSCFG_DEBUG)
#define CTRL_KB_DEBUG			true
#else
#define CTRL_KB_DEBUG			false
#endif

#define CTRL_KB_SS_3KEY			0x01
#define CTRL_KB_SS_64KEY		0x02

#define CTRL_KB_SS_LAYER		CTRL_KB_SS_3KEY


#define CTRL_KB_KEY_NULL		0x00		// 键值空
#define CTRL_KB_KEY_YES			0x01		// 确认键
#define CTRL_KB_KEY_NO			0x02		// 取消键
#define CTRL_KB_KEY_FUNC		0x03		// 功能键

#define CTRL_KB_BUFFER_LENGTH	(1 << 4)


typedef void (* CTRL_KB_SCAN_THREAD)(bool *pbNewKey);
typedef ErrorStatus (* CTRL_KB_READ_KEYVALUE)(SS_KB_KEY_VALUE *pKeyValue);

typedef struct{
	bool					bInitOK;
	bool					bOpenVoice;

	uint8_t					au8KeyBuffer[CTRL_KB_BUFFER_LENGTH];
	uint8_t					u8BufferInDex;
	uint8_t					u8BufferOutDex;

	CTRL_KB_SCAN_THREAD		pScanThread;
	CTRL_KB_READ_KEYVALUE	pReadKeyValue;
} CTRL_KB_INFO;

ErrorStatus ctrl_kb_init(void);
void ctrl_kb_scan_thread(void);
uint8_t ctrl_kb_read_key(void);
ErrorStatus ctrl_kb_AddKeyManul(uint8_t u8Key);
void ctrl_kb_CleanBuffer(void);

#endif /* CTRL_KEYBOARD_H_ */
