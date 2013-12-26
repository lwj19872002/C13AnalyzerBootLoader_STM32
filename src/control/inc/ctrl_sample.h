/*
 * ctrl_sample.h
 *
 *  Created on: 2013-4-8
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_SAMPLE_H_
#define CTRL_SAMPLE_H_

#define CTRL_SAMPLE_SS_ADC7739			0x01
#define CTRL_SAMPLE_SS_OTHER			0xFF

#define CTRL_SAMPLE_SENSOR	CTRL_SAMPLE_SS_ADC7739

typedef enum{
	e_sample_uninit		=	0x00,		// 采样器未初始化
	e_sample_idle		=	0x01,		// 采样器处于空闲状态
	e_sample_sampling	=	0x02,		// 采样器正在采样
	e_sample_dataok		=	0x03,		// 采样器已经采样完毕
} CTRL_SAMPLE_STATUE;

typedef struct{
	uint32_t			*pu32UserDataBuff;			// 存储采集数据的缓冲区，由应用程序提供。多通道采集时，后一个通道的数据紧接着前一个通道的数据。
													// 应用程序注意在给缓冲区时防止溢出，该缓冲区大小应该是：采样点数X通道数。
	uint32_t			u32SamlePoints;				// 需要采样的点数，单次采样时忽略
} CTRL_SAMPLE_USER_INFO;

typedef struct{
	uint8_t				u8SampleChannel;					// 采样总通道数

#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
	SS_ADC_AD7739_CH	aeADCChannels[e_ad7739_ch7 + 1];	// 需要采样的通道号队列
#else
	SS_ADC_AD7739_CH	aeADCChannels[e_ad7739_ch7 + 1];	// 需要采样的通道号队列
#endif

	uint32_t			u32SamplePeriod;			// 采样间隔，单位us,单次采样是时忽略
} CTRL_SAMPLE_SETING;

typedef struct{
	CTRL_SAMPLE_STATUE		eStatue;		// 采样器状态

	bool					bBlock;			// 停止采样

//	PAL_TIMER_NUM			eTimerNum;		// 使用的定时器逻辑编号

	CTRL_SAMPLE_USER_INFO	stUserCfg;		//

#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
	uint32_t				au32ADCData[e_ad7739_ch7 + 1];
#endif

	CTRL_SAMPLE_SETING		stSetting;
} CTRL_SAMPLE_INFO;


ErrorStatus ctrl_sample_init(void);

void ctrl_sample_timer_handler(void);

bool ctrl_sample_is_ok(void);

ErrorStatus ctrl_sample_set(CTRL_SAMPLE_SETING *pSampleSet);
ErrorStatus ctrl_sample_start(CTRL_SAMPLE_USER_INFO *pUserCfg);

ErrorStatus ctrl_sample_stop(void);

ErrorStatus ctrl_sample_once(CTRL_SAMPLE_USER_INFO *pUserInfo);

ErrorStatus ctrl_sample_get_adc_value(uint32_t *pau32ADCVal);

void ctrl_sample_block(bool bBlock);
#endif /* CTRL_SAMPLE_H_ */
