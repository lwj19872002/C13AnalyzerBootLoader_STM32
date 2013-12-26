/*
 * util.h
 *
 *  Created on: 2013-3-1
 *      Author: WLD-LuoWenjie
 */

#ifndef UTIL_H_
#define UTIL_H_

#define UTIL_DEBUG_PRINTF(bF,x) do{(bF)? printf##x:0;}while(0)

#define UTIL_STRING(x) #x

#define UTIL_FIFO_DEX_INC(dex, fifolen)	do{\
											(dex) = (((dex) + 1) & ((fifolen) -1));\
										}while(0)

#define UTIL_FIFO_CLEAN(Idex, Odex)	do{\
											(Idex) = (Odex);\
										}while(0)

#define UTIL_FIFO_IS_EMPTY(Idex, Odex) ((Idex) == (Odex))

#define UTIL_FIFO_COUNT(Odex, Idex, fifolen) (((Idex) - (Odex)) & ((fifolen) -1))

#define UTIL_FIFO_REMAIN(Odex, Idex, fifolen) (((Odex) - (Idex + 1)) & ((fifolen) - 1))

#define UTIL_IS_BETWEEN(val, low, high) (((val) >= (low))? (((val) <= (high))? 1:0):0)


#define UTIL_ADC_TO_VOL(x) (((float)(x) / 0xFFFFFF) * 2.5)


#define UTIL_IF(Test, Value, x, new) if(Test == Value)\
										x = new

#define UTIL_ELSE_IF(Test, Value, x, new) else if(Test == Value)\
											x = new

#define UTIL_ELSE(x, new) else\
							x = new


uint16_t util_CRC16Cal(uint8_t *pu8Source, uint16_t u16Len);

ErrorStatus util_calculate_checksum(uint8_t *pau8SrcData, uint32_t u32DataLen, uint8_t *pu8CheckSum);
ErrorStatus util_average(const uint32_t *pau32SrcData, uint32_t u32DataLen, uint32_t *pu32Average);
ErrorStatus util_average_float(const float *pfSrcData, uint32_t u32DataLen, float *pfAverage);
void util_float_change_endtype(float *pfData);

#endif /* UTIL_H_ */
