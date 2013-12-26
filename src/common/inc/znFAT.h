/*
 * znFAT.h
 *
 *  Created on: 2013-4-7
 *      Author: WLD-LuoWenjie
 */

#ifndef ZNFAT_H_
#define ZNFAT_H_

/*******************************************************

        +-----------------------------------------+
        |振南电子 原创程序模块 znFAT文件系统 5.18 |
        +-----------------------------------------+

  此源码版权属 振南 全权享有，如欲引用，敬请署名并告知
        严禁随意用于商业目的，违者必究，后果自负
         振南电子
             ->产品网站 http://www.znmcu.cn/
             ->产品论坛 http://bbs.znmcu.cn/
             ->产品网店 http://shop.znmcu.cn/
             ->产品咨询 QQ:987582714 MSN:yzn07@126.com
	                WW:yzn07
说明：znFAT经多方测试，确保其正确性与稳定性，请放心使用，
      如有bug敬请告知，谢谢！！
********************************************************/

//znFAT的裁减宏---------------------------------------------------------
#define  ZNFAT_ENTER_DIR	 //有此宏，函数 znFAT_Enter_Dir() 参与编译
#define  ZNFAT_OPEN_FILE	 //有此宏，函数 znFAT_Open_File() 参与编译
#define  ZNFAT_SEEK_FILE	 //有此宏，函数 znFAT_Seek_File() 参与编译
#define  ZNFAT_READ_FILE	 //有此宏，函数 znFAT_Read_File() 参与编译
#define  ZNFAT_READ_FILEX	 //有此宏，函数 znFAT_Read_FileX() 参与编译
#define  ZNFAT_ADD_DAT	     //有此宏，函数 znFAT_Add_Dat() 参与编译
#define  ZNFAT_CREATE_DIR	 //有此宏，函数 znFAT_Create_Dir() 参与编译
#define  ZNFAT_CREATE_FILE	 //有此宏，函数 znFAT_Create_File() 参与编译
#define  ZNFAT_DEL_FILE	     //有此宏，函数 znFAT_Del_File() 参与编译
#define  ZNFAT_XCOPY_FILE	 //有此宏，函数 znFAT_XCopy_File() 参与编译
#define  ZNFAT_RENAME_FILE	 //有此宏，函数 znFAT_Rename_File() 参与编译

#define ZNFAT_GET_TOTAL_SIZE  //有此宏，函数 znFAT_Get_Total_Size() 参与编译
#define znFAT_GET_REMAIN_CAP  //有此宏，函数 znFAT_Get_Remain_Cap() 参与编译

#define ZNFAT_FILE_CLOSE

#ifdef ZNFAT_DEL_FILE
#define ZNFAT_OPEN_FILE
#define SEARCH_LAST_USABLE_CLUSTER
#define ZNFAT_UPDATE_FSINFO_LAST_CLU
#define ZNFAT_UPDATE_FSINFO_FREE_CLU
#define ZNFAT_GETNEXTCLUSTER
#define ZNFAT_MODIFY_FAT
#endif

#ifdef ZNFAT_OPEN_FILE
#define ZNFAT_ENTER_DIR
#define ZNFAT_TOFILENAME
#define FILENAMEMATCH
#define ZNFAT_GETNEXTCLUSTER
#define STR2UP
#endif

#ifdef  ZNFAT_ENTER_DIR
#define COMPARE_DIR_NAME
#define ZNFAT_GETNEXTCLUSTER
#endif

#ifdef ZNFAT_READ_FILE
#define  ZNFAT_SEEK_FILE
#define ZNFAT_GETNEXTCLUSTER
#endif

#ifdef ZNFAT_READ_FILEX
#define  ZNFAT_SEEK_FILE
#define ZNFAT_GETNEXTCLUSTER
#endif

#ifdef ZNFAT_ADD_DAT
#define  ZNFAT_SEEK_FILE
#define ZNFAT_MODIFY_FAT
#define ZNFAT_FIND_FREE_CLUST
#endif

#ifdef ZNFAT_SEEK_FILE
#define ZNFAT_GETNEXTCLUSTER
#endif

#ifdef ZNFAT_CREATE_DIR
#define ZNFAT_ENTER_DIR
#define ZNFAT_CREATE_REC
#endif

#ifdef ZNFAT_CREATE_FILE
#define  ZNFAT_OPEN_FILE
#define  ZNFAT_CREATE_DIR
#define ZNFAT_CREATE_REC
#endif

#ifdef ZNFAT_XCOPY_FILE
#define  ZNFAT_CREATE_FILE
#define  ZNFAT_OPEN_FILE
#define  ZNFAT_ADD_DAT
#define  ZNFAT_READ_FILE
#endif

#ifdef ZNFAT_RENAME_FILE
#define  ZNFAT_OPEN_FILE
#endif

#ifdef ZNFAT_CREATE_REC
#define FILL_REC_INF
#define ZNFAT_FIND_FREE_CLUST
#define ZNFAT_MODIFY_FAT
#define ZNFAT_GETNEXTCLUSTER
#define ZNFAT_EMPTY_CLUSTER
#endif

#ifdef ZNFAT_FIND_FREE_CLUST
#define ZNFAT_UPDATE_FSINFO_FREE_CLU
#define ZNFAT_EMPTY_CLUSTER
#define ZNFAT_UPDATE_FSINFO_LAST_CLU
#endif

#ifdef ZNFAT_UPDATE_FSINFO_FREE_CLU
#define znFAT_GET_REMAIN_CAP
#endif
//----------------------------------------------------------------------

#define ZNFAT_SOC(c) (((c-pArg->FirstDirClust)*(pArg->SectorsPerClust))+pArg->FirstDirSector) // 用于计算簇的开始扇区
#define CONST const

//设备表
#define ZNFAT_SDCARD 0 //SD卡
#define ZNFAT_UDISK  1 //U盘
#define ZNFAT_CFCARD 2 //CF卡
#define ZNFAT_OTHER  3 //其它
				 //这里的存储设备表，可以灵活扩充，以实现对更多存储设备的支持
//-------------------------------------------

#define ZNFAT_MAKE_FILE_TIME(h,m,s)	((((unsigned int)h)<<11)+(((unsigned int)m)<<5)+(((unsigned int)s)>>1))	/* 生成指定时分秒的文件时间数据 */
#define ZNFAT_MAKE_FILE_DATE(y,m,d)	(((((unsigned int)y)+20)<<9)+(((unsigned int)m)<<5)+((unsigned int)d))	/* 生成指定年月日的文件日期数据 */

//DPT:分区记录结构如下
struct znFAT_PartRecord
{
	uint8_t Active;         //0x80表示此分区有效
	uint8_t StartHead;      //分区的开始磁头
	uint8_t StartCylSect[2];//开始柱面与扇区
	uint8_t PartType;       //分区类型
	uint8_t EndHead;        //分区的结束头
	uint8_t EndCylSect[2];  //结束柱面与扇区
	uint8_t StartLBA[4];    //分区的第一个扇区
	uint8_t Size[4];        //分区的大小
};

//MBR:分区扇区（绝对0扇区）定义如下
struct znFAT_PartSector
{
	uint8_t	PartCode[446]; //MBR的引导程序
	struct	znFAT_PartRecord Part[4];   //4个分区记录
	uint8_t	BootSectSig0;  //55
	uint8_t	BootSectSig1;  //AA
};


//znFAT中对BPB的定义如下  一共占用90个字节
struct znFAT_BPB
{
	uint8_t BS_jmpBoot[3];     //跳转指令            offset: 0
	uint8_t BS_OEMName[8];     //                    offset: 3
	uint8_t BPB_BytesPerSec[2];//每扇区字节数        offset:11
	uint8_t BPB_SecPerClus[1]; //每簇扇区数          offset:13
	uint8_t BPB_RsvdSecCnt[2]; //保留扇区数目        offset:14
	uint8_t BPB_NumFATs[1];    //此卷中FAT表数       offset:16
	uint8_t BPB_RootEntCnt[2]; //znFAT为0            offset:17
	uint8_t BPB_TotSec16[2];   //znFAT为0            offset:19
	uint8_t BPB_Media[1];      //存储介质            offset:21
	uint8_t BPB_FATSz16[2];    //znFAT为0            offset:22
	uint8_t BPB_SecPerTrk[2];  //磁道扇区数          offset:24
	uint8_t BPB_NumHeads[2];   //磁头数              offset:26
	uint8_t BPB_HiddSec[4];    //FAT区前隐扇区数     offset:28
	uint8_t BPB_TotSec32[4];   //该卷总扇区数        offset:32
	uint8_t BPB_FATSz32[4];    //一个FAT表扇区数     offset:36
	uint8_t BPB_ExtFlags[2];   //znFAT特有           offset:40
	uint8_t BPB_FSVer[2];      //znFAT特有           offset:42
	uint8_t BPB_RootClus[4];   //根目录簇号          offset:44
	uint8_t FSInfo[2];         //保留扇区FSINFO扇区数offset:48
	uint8_t BPB_BkBootSec[2];  //通常为6             offset:50
	uint8_t BPB_Reserved[12];  //扩展用              offset:52
	uint8_t BS_DrvNum[1];      //                    offset:64
	uint8_t BS_Reserved1[1];   //                    offset:65
	uint8_t BS_BootSig[1];     //                    offset:66
	uint8_t BS_VolID[4];       //                    offset:67
	uint8_t BS_FilSysType[11]; //	                offset:71
	uint8_t BS_FilSysType1[8]; //"znFAT    "         offset:82
};

struct znFAT_FAT_Item
{
	uint8_t Item[4];
};

struct znFAT_FAT
{
	struct znFAT_FAT_Item Items[128];
};

struct znFAT_direntry
{
	uint8_t deName[8];			// 文件名，不足部分以空格补充
	uint8_t deExtension[3]; 	// 扩展名，不足部分以空格补充
	uint8_t deAttributes;   	// 文件属性
	uint8_t deLowerCase;    	// 0
	uint8_t deCHundredth;   	// 世纪
	uint8_t deCTime[2];     	// 创建时间
	uint8_t deCDate[2];     	// 创建日期
	uint8_t deADate[2];     	// 访问日期
	uint8_t deHighClust[2];		// 开始簇的高字
	uint8_t deMTime[2];     	// 最近的修改时间
	uint8_t deMDate[2];     	// 最近的修改日期
	uint8_t deLowCluster[2]; 	// 开始簇的低字
	uint8_t deFileSize[4];      // 文件大小
};

//znFAT初始化时初始参数装入如下结构体中
struct znFAT_Init_Arg
{
	uint8_t		DEV_No;

	uint8_t		BPB_Sector_No;		//BPB所在扇区号
	uint32_t	Total_Size;			//磁盘的总容量

	uint32_t	FirstDirClust;		//根目录的开始簇
	uint32_t	BytesPerSector;		//每个扇区的字节数
	uint32_t	FATsectors;			//FAT表所占扇区数
	uint32_t	SectorsPerClust;	//每簇的扇区数
	uint32_t	FirstFATSector;		//第一个FAT表所在扇区
	uint32_t	FirstDirSector;		//第一个目录所在扇区
};

struct znFAT_Date
{
	uint16_t	year;
	uint8_t		month;
	uint8_t		day;
};

struct znFAT_Time
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
};

// Stuctures
struct znFAT_FileInfoStruct
{
	uint8_t		FileName[15];       //文件名
	uint32_t	FileStartCluster;   //文件的开始簇

	uint32_t	FileCurCluster;		//文件的当前簇
	uint32_t	FileSize;	    	//文件大小
	uint32_t	FileCurSector;	    //文件的当前扇区
	uint16_t	FileCurPos;         //文件在当前扇区中的位置
	uint32_t	FileCurOffset;	    //文件的当前偏移量
	uint32_t	Rec_Sec;            //文件的文件/目录项所在的扇区
	uint16_t	nRec;               //文件的文件/目录项所在扇区中的位置

	uint8_t		FileAttr;			//文件属性
	struct znFAT_Time	FileCreateTime;	    //文件的创建时间
	struct znFAT_Date	FileCreateDate;	    //文件的创建日期
	struct znFAT_Time	FileMTime;          //文件的修改时间
	struct znFAT_Date	FileMDate;          //文件的修改日期
	struct znFAT_Date	FileADate;          //文件的访问日期
};

struct znFAT_FSInfo //znFAT的文件系统信息结构
{
	uint8_t Head[4];
	uint8_t Resv1[480];
	uint8_t Sign[4];
	uint8_t Free_Cluster[4];
	uint8_t Last_Cluster[4];
	uint8_t Resv2[14];
	uint8_t Tail[2];
};

//函数声明
void znFAT_Device_Init();

uint32_t znFAT_Get_Total_Size(void); //获取总容量
uint32_t znFAT_Get_Remain_Cap(void); //获取剩余容量

//文件系统初始化
void znFAT_Init(void);

//进入目录
uint32_t znFAT_Enter_Dir(CONST int8_t *path);

//打开文件，支持通配
uint8_t znFAT_Open_File(struct znFAT_FileInfoStruct *pfi,CONST int8_t *filepath,uint32_t item,uint8_t is_file);

//文件定位
uint8_t znFAT_Seek_File(struct znFAT_FileInfoStruct *pfi,uint32_t offset);

//读取文件数据
uint32_t znFAT_Read_File(struct znFAT_FileInfoStruct *pfi,uint32_t offset,uint32_t len,uint8_t *pbuf);

//读取文件数据并进行数据处理
uint32_t znFAT_Read_FileX(struct znFAT_FileInfoStruct *pfi,uint32_t offset,uint32_t len,void (*pfun)(uint8_t));

//向文件追加数据
uint32_t znFAT_Add_Dat(struct znFAT_FileInfoStruct *pfi,uint32_t len,uint8_t *pbuf);

//创建目录，支持创建时间
uint8_t znFAT_Create_Dir(CONST int8_t *dirpath,uint8_t *ptd);

//创建文件，支持创建时间
uint8_t znFAT_Create_File(struct znFAT_FileInfoStruct *pfi,CONST int8_t *filepath,uint8_t *ptd);

//删除文件
uint8_t znFAT_Del_File(CONST int8_t *filepath);

//文件拷贝，支持多设备中文件互拷
uint8_t znFAT_XCopy_File(struct znFAT_Init_Arg *pArg1,struct znFAT_Init_Arg *pArg2,CONST int8_t *sfilename,CONST int8_t *tfilename,uint8_t *file_buf,uint32_t buf_size,uint8_t *pt);

//文件关闭
uint8_t znFAT_File_Close(struct znFAT_FileInfoStruct *pfi);

//文件重命名
uint8_t znFAT_Rename_File(CONST int8_t *filename,CONST int8_t *newfilename);

#endif /* ZNFAT_H_ */
