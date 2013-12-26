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
	e_sample_uninit		=	0x00,		// ������δ��ʼ��
	e_sample_idle		=	0x01,		// ���������ڿ���״̬
	e_sample_sampling	=	0x02,		// ���������ڲ���
	e_sample_dataok		=	0x03,		// �������Ѿ��������
} CTRL_SAMPLE_STATUE;

typedef struct{
	uint32_t			*pu32UserDataBuff;			// �洢�ɼ����ݵĻ���������Ӧ�ó����ṩ����ͨ���ɼ�ʱ����һ��ͨ�������ݽ�����ǰһ��ͨ�������ݡ�
													// Ӧ�ó���ע���ڸ�������ʱ��ֹ������û�������СӦ���ǣ���������Xͨ������
	uint32_t			u32SamlePoints;				// ��Ҫ�����ĵ��������β���ʱ����
} CTRL_SAMPLE_USER_INFO;

typedef struct{
	uint8_t				u8SampleChannel;					// ������ͨ����

#if(CTRL_SAMPLE_SENSOR == CTRL_SAMPLE_SS_ADC7739)
	SS_ADC_AD7739_CH	aeADCChannels[e_ad7739_ch7 + 1];	// ��Ҫ������ͨ���Ŷ���
#else
	SS_ADC_AD7739_CH	aeADCChannels[e_ad7739_ch7 + 1];	// ��Ҫ������ͨ���Ŷ���
#endif

	uint32_t			u32SamplePeriod;			// �����������λus,���β�����ʱ����
} CTRL_SAMPLE_SETING;

typedef struct{
	CTRL_SAMPLE_STATUE		eStatue;		// ������״̬

	bool					bBlock;			// ֹͣ����

//	PAL_TIMER_NUM			eTimerNum;		// ʹ�õĶ�ʱ���߼����

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
