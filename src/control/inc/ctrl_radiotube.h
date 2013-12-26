/*
 * ctrl_radiotube.h
 *
 *  Created on: 2013-4-2
 *      Author: WLD-LuoWenjie
 */

#ifndef CTRL_RADIOTUBE_H_
#define CTRL_RADIOTUBE_H_

#define CTRL_RADIOTUBE_NUM_MAX			SS_RADIOTUBE_NUM_OF_ALL

typedef enum{
	e_radiotube_null		=	0x00000000,
	e_radiotube_1		=	0x00000001,
	e_radiotube_2		=	0x00000002,
	e_radiotube_3		=	0x00000004,
	e_radiotube_4		=	0x00000008,
	e_radiotube_5		=	0x00000010,

	e_radiotube_6		=	0x00000020,
	e_radiotube_7		=	0x00000040,
	e_radiotube_8		=	0x00000080,
	e_radiotube_9		=	0x00000100,
	e_radiotube_10		=	0x00000200,

	e_radiotube_11		=	0x00000400,
	e_radiotube_12		=	0x00000800,
	e_radiotube_13		=	0x00001000,
	e_radiotube_14		=	0x00002000,
	e_radiotube_15		=	0x00004000,

	e_radiotube_16		=	0x00008000,
	e_radiotube_17		=	0x00010000,
	e_radiotube_18		=	0x00020000,
	e_radiotube_19		=	0x00040000,
	e_radiotube_20		=	0x00080000,

	e_radiotube_all		=	0x000FFFFF,
} CTRL_RADIOTUBE_NUM;

#define CTRL_RT_AIR_IN			e_radiotube_3
#define CTRL_RT_AIR_FILTER		e_radiotube_4
#define CTRL_RT_ZERO_AIR_IN		e_radiotube_2
#define CTRL_RT_AIR_OUT			e_radiotube_1

#define CTRL_RT_GROUP1_0		e_radiotube_5
#define CTRL_RT_GROUP1_30		e_radiotube_6

#define CTRL_RT_GROUP2_0		e_radiotube_7
#define CTRL_RT_GROUP2_30		e_radiotube_8

#define CTRL_RT_GROUP3_0		e_radiotube_9
#define CTRL_RT_GROUP3_30		e_radiotube_10

#define CTRL_RT_GROUP4_0		e_radiotube_11
#define CTRL_RT_GROUP4_30		e_radiotube_12

#define CTRL_RT_GROUP5_0		e_radiotube_13
#define CTRL_RT_GROUP5_30		e_radiotube_14

#define CTRL_RT_GROUP6_0		e_radiotube_15
#define CTRL_RT_GROUP6_30		e_radiotube_16

#define CTRL_RT_GROUP7_0		e_radiotube_17
#define CTRL_RT_GROUP7_30		e_radiotube_18

#define CTRL_RT_GROUP8_0		e_radiotube_19
#define CTRL_RT_GROUP8_30		e_radiotube_20

#define CTRL_RT_GROUP(x,y)		CTRL_RT_GROUP##x##_##y

#define CTRL_RT_ALL				e_radiotube_all

#define CTRL_RT_COMBINE(x)		((CTRL_RADIOTUBE_NUM)(x))

#define CTRL_RT_IS_TESTAIR_CH(x)	(((x)==CTRL_RT_GROUP1_0) || \
									((x)==CTRL_RT_GROUP1_30) || \
									((x)==CTRL_RT_GROUP2_0)  ||  \
									((x)==CTRL_RT_GROUP2_30) || \
									((x)==CTRL_RT_GROUP3_0)  ||  \
									((x)==CTRL_RT_GROUP3_30) || \
									((x)==CTRL_RT_GROUP4_0)  ||  \
									((x)==CTRL_RT_GROUP4_30) || \
									((x)==CTRL_RT_GROUP5_0)  ||  \
									((x)==CTRL_RT_GROUP5_30) || \
									((x)==CTRL_RT_GROUP6_0)  || \
									((x)==CTRL_RT_GROUP6_30) || \
									((x)==CTRL_RT_GROUP7_0)  || \
									((x)==CTRL_RT_GROUP7_30) || \
									((x)==CTRL_RT_GROUP8_0)  || \
									((x)==CTRL_RT_GROUP8_30))

ErrorStatus ctrl_radiotube_init(void);

void ctrl_radiotube_on(CTRL_RADIOTUBE_NUM eNum);
void ctrl_radiotube_off(CTRL_RADIOTUBE_NUM eNum);

#endif /* CTRL_RADIOTUBE_H_ */
