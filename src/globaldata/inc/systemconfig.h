/*
 * systemconfig.h
 *
 *  Created on: 2013-2-26
 *      Author: WLD-LuoWenjie
 */
/*
 *       ʱ��                         ������                 ��ǰ�汾                       ����
 *   2013-09-06     ���Ľ�          W13.01.01B    1����д��ʼ�汾
 *
 */

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

#define SYSCFG_DEBUG			true

#define SYSCFG_SW_VERSION "W13.01.1B"

#define SYSCFG_HW_V0102 0x0102
#define SYSCFG_HW_V0200 0x0200
#define SYSCFG_HW_CURVER SYSCFG_HW_V0200
//#define SYSCFG_HW_CURVER SYSCFG_HW_V0102

#if(SYSCFG_HW_CURVER == SYSCFG_HW_V0102)
#define SYSCFG_HW_VERSION "V01.02.00R"
#elif(SYSCFG_HW_CURVER == SYSCFG_HW_V0200)
#define SYSCFG_HW_VERSION "V02.00.00R"
#endif

extern const uint32_t g_stc_u32SystemTickTime;

#endif /* SYSTEMCONFIG_H_ */
