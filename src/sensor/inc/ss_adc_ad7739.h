/*
 * ss_adc_ad7739.h
 *
 *  Created on: 2013-3-25
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_ADC_AD7739_H_
#define SS_ADC_AD7739_H_

#define SS_ADC_AD7739_READ_REG(add)		(0x40 | (add))
#define SS_ADC_AD7739_WRITE_REG(add)	(0x00 | (add))

#define SS_ADC_AD7739_READ_CHREG(add, ch)	(0x40 | (add) | (ch))
#define SS_ADC_AD7739_WRITE_CHREG(add, ch)	(0x00 | (add) | (ch))

#define SS_ADC_AD7739_SUNOFCHANNEL		8

typedef enum {
	e_ad7739_ch0 = 0x00,
	e_ad7739_ch1 = 0x01,
	e_ad7739_ch2 = 0x02,
	e_ad7739_ch3 = 0x03,
	e_ad7739_ch4 = 0x04,
	e_ad7739_ch5 = 0x05,
	e_ad7739_ch6 = 0x06,
	e_ad7739_ch7 = 0x07,
} SS_ADC_AD7739_CH;

typedef enum{
	e_ad7739_add_Commu			=	0x00,
	e_ad7739_add_IOPort			=	0x01,
	e_ad7739_add_Revision		=	0x02,
	e_ad7739_add_Test			=	0x03,
	e_ad7739_add_Status			=	0x04,
	e_ad7739_add_CheckSum		=	0x05,
	e_ad7739_add_ADZeroScale	=	0x06,
	e_ad7739_add_ADFullScale	=	0x07,
	e_ad7739_add_ChData			=	0x08,
	e_ad7739_add_ChZeroScale	=	0x10,
	e_ad7739_add_ChFullScale	=	0x18,
	e_ad7739_add_ChStatus		=	0x20,
	e_ad7739_add_ChStatup		=	0x28,
	e_ad7739_add_ChConvTime		=	0x30,
	e_ad7739_add_Mode			=	0x38,
} SS_ADC_AD7739_REGADD;

typedef enum{
	e_ad7739_mode_Idle			=	0x00,
	e_ad7739_mode_Continuous	=	0x20,
	e_ad7739_mode_Single		=	0x40,
	e_ad7739_mode_PowerDown		=	0x60,
	e_ad7739_mode_ADCZeroCal	=	0x80,
	e_ad7739_mode_ADCFullCal	=	0xA0,
	e_ad7739_mode_ChZeroCal		=	0xC0,
	e_ad7739_mode_ChFullCal		=	0xE0,
} SS_ADC_AD7739_MODE_MD;

typedef enum{
	e_ad7739_mclk_output_on		=	0x00,
	e_ad7739_mclk_output_off	=	0x10,
} SS_ADC_AD7739_MODE_CLKDIS;

typedef enum{
	e_ad7739_DUMP_off	=	0x00,
	e_ad7739_DUMP_on	=	0x08,
} SS_ADC_AD7739_MODE_DUMP;

typedef enum{
	e_ad7739_ContRD_off	=	0x00,
	e_ad7739_ContRD_on	=	0x04,
} SS_ADC_AD7739_MODE_CONTRD;

typedef enum{
	e_ad7739_16Bit	=	0x00,
	e_ad7739_24Bit	=	0x02,
} SS_ADC_AD7739_MODE_24OR16;

typedef enum{
	e_ad7739_CLAMP_off	=	0x00,
	e_ad7739_CLAMP_on	=	0x01,
} SS_ADC_AD7739_MODE_CLAMP;

typedef enum{
	e_ad7739_chop_off	=	0x00,
	e_ad7739_chop_on	=	0x80,
}SS_ADC_AD7739_CHCONVTIME_CHOP;

typedef enum{
	e_ad7739_buff_on	=	0x00,
	e_ad7739_buff_off	=	0x80,
}SS_ADC_AD7739_SETUP_BUFF;

typedef enum{
	e_ad7739_diff_off	=	0x00,
	e_ad7739_diff_on	=	0x60,
}SS_ADC_AD7739_SETUP_COM;

typedef enum{
	e_ad7739_statopt_off	=	0x00,
	e_ad7739_statopt_on		=	0x10,
}SS_ADC_AD7739_SETUP_STATOPT;

typedef enum{
	e_ad7739_ctnenable_off	=	0x00,
	e_ad7739_ctnenable_on	=	0x08,
}SS_ADC_AD7739_SETUP_ENABLE;

typedef enum{
	e_ad7739_rng_d2v5	=	0x04,
	e_ad7739_rng_2v5	=	0x05,
	e_ad7739_rng_d1v25	=	0x00,
	e_ad7739_rng_1v25	=	0x01,
	e_ad7739_rng_dv625	=	0x02,
	e_ad7739_rng_v625	=	0x03,
}SS_ADC_AD7739_SETUP_RNG;

//typedef struct{
//	GPIO_TypeDef		*pGpioRDYPort;
//	uint16_t			u16GpioRDYPin;
//
//	uint32_t			u32APB2Clk;
//	uint32_t			u32APB1Clk;
//	uint32_t			u32AHBClk;
//}SS_ADC_AD7739_CFG;

typedef struct{
	const PAL_SPI_CFG		*pSPIConfig;
}SS_ADC_ADC7739_INFO;

ErrorStatus ss_adc_ad7739_init(void);
ErrorStatus ss_adc_ad7739_channel_setup(SS_ADC_AD7739_CH eCh, uint8_t u8ChSteup);
ErrorStatus ss_adc_ad7739_set_filter_word(SS_ADC_AD7739_CH eCh, uint8_t u8FW);

ErrorStatus ss_adc_ad7739_blocksample_once(SS_ADC_AD7739_CH eCh, uint32_t *pu32Result);

ErrorStatus ss_adc_ad7739_sample_onech_once(SS_ADC_AD7739_CH eCh, uint32_t *pu32Result);
ErrorStatus ss_adc_ad7739_sample_multch_once(SS_ADC_AD7739_CH *peCh, uint32_t u32ChNum, uint32_t *pu32Result);

ErrorStatus ss_adc_ad7739_read_ioport(uint8_t *pu8IOPort);
ErrorStatus ss_adc_ad7739_read_revision(uint8_t *pu8Revision);

ErrorStatus ss_adc_ad7739_enable_ch_to_sample(SS_ADC_AD7739_CH eCh);
ErrorStatus ss_adc_ad7739_Disable_ch_to_sample(SS_ADC_AD7739_CH eCh);


//__STATIC_INLINE bool ss_adc_ad7739_rdy_is_low(SS_ADC_ADC7739_INFO *pAD7739Info)
//{
//	return (Bit_RESET == GPIO_ReadInputDataBit(pAD7739Info->pADCConfig->pGpioRDYPort, pAD7739Info->pADCConfig->u16GpioRDYPin));
//}
//
//__STATIC_INLINE bool ss_adc_ad7739_rdy_is_high(SS_ADC_ADC7739_INFO *pAD7739Info)
//{
//	return (Bit_SET == GPIO_ReadInputDataBit(pAD7739Info->pADCConfig->pGpioRDYPort, pAD7739Info->pADCConfig->u16GpioRDYPin));
//}

__STATIC_INLINE void ss_adc_ad7739_transmit_byte(PAL_SPI_NUM ePortNum, uint8_t u8SendData, uint8_t *pReadData)
{
	pal_spi_cs_low(ePortNum);
	pal_spi_transmit_byte(ePortNum, u8SendData, pReadData);
	pal_spi_cs_hight(ePortNum);
}

__STATIC_INLINE void ss_adc_ad7739_read_byte(PAL_SPI_NUM ePortNum, uint8_t *pReadData)
{
	pal_spi_cs_low(ePortNum);
	pal_spi_transmit_byte(ePortNum, 0x00, pReadData);
	pal_spi_cs_hight(ePortNum);

//	*pReadData = 0xAA;
}

__STATIC_INLINE void ss_adc_ad7739_write_byte(PAL_SPI_NUM ePortNum, uint8_t u8SendData)
{
	uint8_t u8ReadData;

	pal_spi_cs_low(ePortNum);
	pal_spi_transmit_byte(ePortNum, u8SendData, &u8ReadData);
	pal_spi_cs_hight(ePortNum);
}

#endif /* SS_ADC_AD7739_H_ */
