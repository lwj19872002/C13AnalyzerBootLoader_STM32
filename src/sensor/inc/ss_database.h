/*
 * ss_database.h
 *
 *  Created on: 2013-4-15
 *      Author: WLD-LuoWenjie
 */

#ifndef SS_DATABASE_H_
#define SS_DATABASE_H_

#if(SYSCFG_DEBUG)
#define SS_DB_DEBUG			true
#else
#define SS_DB_DEBUG			false
#endif

#define SS_DB_USERDATA_SIZE		(2044)

ErrorStatus ss_DB_get_start_addr(uint32_t u32Offset, uint8_t **ppu8StartAddr);
ErrorStatus ss_DB_save_a_page_data(uint8_t *pau8Data);
ErrorStatus ss_DB_get_user_datasize(uint32_t *pu32Size);

#endif /* SS_DATABASE_H_ */
