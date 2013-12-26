/*
 * ssconfig.h
 *
 *  Created on: 2013-3-28
 *      Author: WLD-LuoWenjie
 */

#ifndef SSCONFIG_H_
#define SSCONFIG_H_

#define SSCFG_ADC_ADC7739
#define SSCFG_SDCARD
#define SSCFG_LEDS
#define SSCFG_RADIOTUBE
#define SSCFG_LCD_NH012864
#define SSCFG_KB_3KEY

#ifdef SSCFG_ADC_ADC7739
//extern const SS_ADC_AD7739_CFG g_cstADCAD7739Cfg;
#endif

#ifdef SSCFG_SDCARD
extern const SS_SD_CFG g_cstSDCardCfg;
#endif

#ifdef SSCFG_LEDS
extern const SS_LEDS_CFG g_cstLEDsCfg;
#endif

#ifdef SSCFG_RADIOTUBE
extern const SS_RADIOTUBE_CFG g_cstRadiotubeCfg;
#endif

#ifdef SSCFG_LCD_NH012864
extern const SS_LCD_NH012864_CFG g_cstLCDNH012864Cfg;
#endif

#ifdef SSCFG_KB_3KEY
extern const SS_KB_3KEY_CFG g_cstKB3KeyCfg;
#endif

#endif /* SSCONFIG_H_ */
