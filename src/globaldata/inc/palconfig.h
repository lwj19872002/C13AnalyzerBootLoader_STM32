/*
 * palconfig.h
 *
 *  Created on: 2013-2-27
 *      Author: WLD-LuoWenjie
 */

#ifndef PALCONFIG_H_
#define PALCONFIG_H_

#include "pal.h"
#include "sensor.h"
#include "controls.h"

#define PALCFG_INDICATIVELED_GPIO
#define PALCFG_STDIO_UART
#define PALCFG_AD7739_SPI
//#define PALCFG_AD7739_RDYPIN
#define PALCFG_SDCARD_SPI
#define PALCFG_SAMPLE_TIMER
#define PALCFG_BUZZER_PWM
#define PALCFG_LIGHTSOURCE_PWM
#define PALCFG_PUMP_GPIO
#define PALCFG_LINKERP1_UART
#define PALCFG_LINKERP2_UART
#define PALCFG_RTC


#define PALCFG_LS_FREQ				(4)
#define PALCFG_LS_RATIO				(0.5)


#ifdef PALCFG_INDICATIVELED_GPIO
extern const PAL_GPIO_INIT_ST g_cstPalIndicateLEDGPIOcfg;
#endif

#ifdef PALCFG_STDIO_UART
extern const PAL_COM_CFG g_cstPalStdioUart;
#endif

#ifdef PALCFG_AD7739_SPI
extern const PAL_SPI_CFG g_cstPalAD7739SPI;
#endif

#ifdef PALCFG_AD7739_RDYPIN
extern const PAL_GPIOEXTI_CFG g_cstPalAD7739RdyPinExti;
#endif

#ifdef PALCFG_SDCARD_SPI
extern const PAL_SPI_CFG g_cstPalSDCardSPI;
#endif

#ifdef PALCFG_SAMPLE_TIMER
extern const PAL_TIMER_CFG g_cstSampleTimer;
#endif

#ifdef PALCFG_BUZZER_PWM
extern const PAL_PWM_CFG g_cstPalBuzzerPWM;
#endif

#ifdef PALCFG_LIGHTSOURCE_PWM
extern const PAL_PWM_CFG g_cstPalLightSourcePWM;
#endif

#ifdef PALCFG_PUMP_GPIO
extern const PAL_GPIO_INIT_ST g_cstPalPumpGPIOcfg;
extern const PAL_PWM_CFG g_cstPalPumpPWMcfg;
#endif

#ifdef PALCFG_LINKERP1_UART
extern const PAL_COM_CFG g_cstPalLinkerP1Uart;
#endif

#ifdef PALCFG_LINKERP2_UART
extern const PAL_COM_CFG g_cstPalLinkerP2Uart;
#endif

#ifdef PALCFG_RTC
extern const PAL_RTC_CFG g_cstPalRTCCfg;
#endif

#endif /* PALCONFIG_H_ */
