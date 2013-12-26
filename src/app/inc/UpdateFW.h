/*
 * UpdateFW.h
 *
 *  Created on: 2013-9-6
 *      Author: WLD-LuoWenjie
 */

#ifndef UPDATEFW_H_
#define UPDATEFW_H_

#define UPDATEFW_FLASHPAGE_SIZE			(0x800)			// 存储页大小，跟CPU的flash页大小一致

typedef struct{
	uint32_t	u32WriteAddrOffset;
} ST_UPDATEFW_INFO;

ErrorStatus UpdateFW_Init(uint32_t u32TotalSector, uint32_t u32SectorSize);
ErrorStatus UpdateFW_ProgramData(const uint8_t *pau8SrcData, uint32_t u32DataLen);

#endif /* UPDATEFW_H_ */
