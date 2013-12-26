/*
 * ss_adc_ad7739.c
 *
 *  Created on: 2013-3-25
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_ADC_AD7739
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static volatile SS_ADC_ADC7739_INFO g_sstADC7739Info;
static volatile PAL_SPI_NUM g_svePalSPINum;

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_init(void)
{
	ErrorStatus			eRet;
	uint8_t				u8Setup;
	uint8_t				u8SendData;
	uint8_t				u8I;
	uint8_t				u8FW;

	memset((uint8_t *)&g_sstADC7739Info, 0x00, sizeof(SS_ADC_ADC7739_INFO));

	g_sstADC7739Info.pSPIConfig = &g_cstPalAD7739SPI;
	g_svePalSPINum = g_sstADC7739Info.pSPIConfig->ePortNum;


	eRet = pal_spi_init(g_sstADC7739Info.pSPIConfig);
	if(!eRet)
		return ERROR;

	pal_spi_cs_hight(g_sstADC7739Info.pSPIConfig->ePortNum);

	u8Setup	=	((uint8_t)e_ad7739_buff_on \
				|(uint8_t)e_ad7739_diff_off \
				|(uint8_t)e_ad7739_statopt_off \
				|(uint8_t)e_ad7739_ctnenable_off \
				|(uint8_t)e_ad7739_rng_2v5);
	for(u8I=0; u8I<SS_ADC_AD7739_SUNOFCHANNEL; u8I++)
		ss_adc_ad7739_channel_setup((SS_ADC_AD7739_CH)u8I, u8Setup);

	for(u8I=0; u8I<SS_ADC_AD7739_SUNOFCHANNEL; u8I++)
	{
		u8SendData = SS_ADC_AD7739_WRITE_CHREG(e_ad7739_add_Mode, e_ad7739_ch0 + u8I);
		ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);
		u8SendData = ((uint8_t)e_ad7739_mode_Continuous | (uint8_t)e_ad7739_24Bit | (uint8_t)e_ad7739_mclk_output_off);
		ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);
	}

	u8FW = 127;
	ss_adc_ad7739_set_filter_word(e_ad7739_ch0, u8FW);
	ss_adc_ad7739_set_filter_word(e_ad7739_ch1, u8FW);
	ss_adc_ad7739_set_filter_word(e_ad7739_ch2, u8FW);
	ss_adc_ad7739_set_filter_word(e_ad7739_ch3, u8FW);
	ss_adc_ad7739_set_filter_word(e_ad7739_ch4, u8FW);
	ss_adc_ad7739_set_filter_word(e_ad7739_ch5, u8FW);
	ss_adc_ad7739_set_filter_word(e_ad7739_ch6, u8FW);
	ss_adc_ad7739_set_filter_word(e_ad7739_ch7, u8FW);

	ss_adc_ad7739_enable_ch_to_sample(e_ad7739_ch6);
	ss_adc_ad7739_enable_ch_to_sample(e_ad7739_ch7);

	return eRet;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_channel_setup(SS_ADC_AD7739_CH eCh, uint8_t u8ChSteup)
{
	uint8_t u8SendData;

	u8SendData = SS_ADC_AD7739_WRITE_CHREG(e_ad7739_add_ChStatup, eCh);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	u8SendData = u8ChSteup;
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_set_filter_word(SS_ADC_AD7739_CH eCh, uint8_t u8FW)
{
	uint8_t u8SendData;

	u8SendData = SS_ADC_AD7739_WRITE_CHREG(e_ad7739_add_ChConvTime, eCh);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	u8SendData = ((uint8_t)e_ad7739_chop_on | (uint8_t)(0x7F & u8FW));
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_blocksample_once(SS_ADC_AD7739_CH eCh, uint32_t *pu32Result)
{
	uint8_t u8ReadData[3];
	uint8_t u8SendData;
	uint32_t u32Flag;
	ErrorStatus eRet;

	if(!pu32Result)
		return ERROR;

	u8SendData = SS_ADC_AD7739_WRITE_CHREG(e_ad7739_add_Mode, eCh);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	u8SendData = ((uint8_t)e_ad7739_mode_Single | (uint8_t)e_ad7739_24Bit | (uint8_t)e_ad7739_mclk_output_off);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	u32Flag = 10000000;
	eRet = ERROR;
	while(u32Flag--)
	{
		// ¶ÁÈ¡Íê³É×´Ì¬Êä³öÒý½Å£¬ÅÐ¶ÏÊÇ·ñ×ª»»Íê³É
//		if(ss_adc_ad7739_rdy_is_low(&g_sstADC7739Info))
//		{
//			eRet = SUCCESS;
//			break;
//		}

		// ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷£¬ÅÐ¶ÏÊÇ·ñ×ª»»Íê³É
		u8SendData = SS_ADC_AD7739_READ_REG(e_ad7739_add_Status);
		ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

		ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8SendData);
		if(u8SendData & (0x01 << eCh))
		{
			eRet = SUCCESS;
			break;
		}
	}

	if(!eRet)
	{
		return ERROR;
	}

	u8SendData = SS_ADC_AD7739_READ_CHREG(e_ad7739_add_ChData, eCh);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8ReadData[0]);

	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8ReadData[1]);

	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8ReadData[2]);

	*pu32Result = ((uint32_t)u8ReadData[0] << 16) | ((uint32_t)u8ReadData[1] << 8) | ((uint32_t)u8ReadData[2]);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_read_ioport(uint8_t *pu8IOPort)
{
	uint8_t u8Temp;

	if(!pu8IOPort)
		return ERROR;

	u8Temp = SS_ADC_AD7739_READ_REG(e_ad7739_add_IOPort);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8Temp);

	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, pu8IOPort);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_write_ioport(uint8_t u8IOPort)
{
	uint8_t u8Temp;

	u8Temp = SS_ADC_AD7739_WRITE_REG(e_ad7739_add_IOPort);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8Temp);

	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8IOPort);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_read_revision(uint8_t *pu8Revision)
{
	uint8_t u8Temp;

	if(!pu8Revision)
		return ERROR;

	u8Temp = SS_ADC_AD7739_READ_REG(e_ad7739_add_Revision);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8Temp);

	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, pu8Revision);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_sample_onech_once(SS_ADC_AD7739_CH eCh, uint32_t *pu32Result)
{
	uint8_t u8ReadData[3];
	uint8_t u8SendData;

	if(!pu32Result)
		return ERROR;

	// ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷£¬ÅÐ¶ÏÊÇ·ñ×ª»»Íê³É
	u8SendData = SS_ADC_AD7739_READ_REG(e_ad7739_add_Status);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8SendData);
	if(!(u8SendData & (0x01 << eCh)))
	{
		return ERROR;
	}

	u8SendData = SS_ADC_AD7739_READ_CHREG(e_ad7739_add_ChData, eCh);
	ss_adc_ad7739_write_byte(g_svePalSPINum, u8SendData);

	ss_adc_ad7739_read_byte(g_svePalSPINum, &u8ReadData[0]);

	ss_adc_ad7739_read_byte(g_svePalSPINum, &u8ReadData[1]);

	ss_adc_ad7739_read_byte(g_svePalSPINum, &u8ReadData[2]);

	*pu32Result = ((uint32_t)u8ReadData[0] << 16) | ((uint32_t)u8ReadData[1] << 8) | ((uint32_t)u8ReadData[2]);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_enable_ch_to_sample(SS_ADC_AD7739_CH eCh)
{
	uint8_t u8Setup;
	uint8_t u8SendData;

	u8SendData = SS_ADC_AD7739_READ_CHREG(e_ad7739_add_ChStatup, eCh);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);
	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8Setup);
	u8Setup |= (uint8_t)e_ad7739_ctnenable_on;
	ss_adc_ad7739_channel_setup(eCh, u8Setup);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_Disable_ch_to_sample(SS_ADC_AD7739_CH eCh)
{
	uint8_t u8Setup;
	uint8_t u8SendData;

	u8SendData = SS_ADC_AD7739_READ_CHREG(e_ad7739_add_ChStatup, eCh);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);
	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8Setup);
	u8Setup &= (~((uint8_t)e_ad7739_ctnenable_on));
	ss_adc_ad7739_channel_setup(eCh, u8Setup);

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_adc_ad7739_sample_multch_once(SS_ADC_AD7739_CH *peCh, uint32_t u32ChNum, uint32_t *pu32Result)
{
	uint8_t u8ReadData[3];
	uint8_t u8SendData;
	uint32_t u32Dex;

	if(!pu32Result)
		return ERROR;

	// ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷£¬ÅÐ¶ÏÊÇ·ñ×ª»»Íê³É
	u8SendData = SS_ADC_AD7739_READ_REG(e_ad7739_add_Status);
	ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

	ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8SendData);

	for(u32Dex=0; u32Dex<u32ChNum; u32Dex++)
	{
		if(!(u8SendData & (0x01 << peCh[u32Dex])))
		{
			return ERROR;
		}

		u8SendData = SS_ADC_AD7739_READ_CHREG(e_ad7739_add_ChData, peCh[u32Dex]);
		ss_adc_ad7739_write_byte(g_sstADC7739Info.pSPIConfig->ePortNum, u8SendData);

		ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8ReadData[0]);

		ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8ReadData[1]);

		ss_adc_ad7739_read_byte(g_sstADC7739Info.pSPIConfig->ePortNum, &u8ReadData[2]);

		pu32Result[u32Dex] = ((uint32_t)u8ReadData[0] << 16) | ((uint32_t)u8ReadData[1] << 8) | ((uint32_t)u8ReadData[2]);
	}

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
