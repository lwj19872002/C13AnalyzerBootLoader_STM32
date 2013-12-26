/*
 * ctrl_voice.h
 *
 *  Created on: 2013-4-15
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_VOICE_H_
#define CTRL_VOICE_H_

#define CTRL_VOICE_KEYDOWN_VOICE_DELAY			1		// 100ms

typedef struct{
	bool	bInitOK;
	bool	bKeyDown;

} CTRL_VOICE_INFO;

ErrorStatus ctrl_voice_init(void);
void ctrl_voice_key_down(void);

#endif /* CTRL_VOICE_H_ */
