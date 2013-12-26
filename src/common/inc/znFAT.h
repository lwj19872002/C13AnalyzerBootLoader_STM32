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
        |���ϵ��� ԭ������ģ�� znFAT�ļ�ϵͳ 5.18 |
        +-----------------------------------------+

  ��Դ���Ȩ�� ���� ȫȨ���У��������ã�������������֪
        �Ͻ�����������ҵĿ�ģ�Υ�߱ؾ�������Ը�
         ���ϵ���
             ->��Ʒ��վ http://www.znmcu.cn/
             ->��Ʒ��̳ http://bbs.znmcu.cn/
             ->��Ʒ���� http://shop.znmcu.cn/
             ->��Ʒ��ѯ QQ:987582714 MSN:yzn07@126.com
	                WW:yzn07
˵����znFAT���෽���ԣ�ȷ������ȷ�����ȶ��ԣ������ʹ�ã�
      ����bug�����֪��лл����
********************************************************/

//znFAT�Ĳü���---------------------------------------------------------
#define  ZNFAT_ENTER_DIR	 //�д˺꣬���� znFAT_Enter_Dir() �������
#define  ZNFAT_OPEN_FILE	 //�д˺꣬���� znFAT_Open_File() �������
#define  ZNFAT_SEEK_FILE	 //�д˺꣬���� znFAT_Seek_File() �������
#define  ZNFAT_READ_FILE	 //�д˺꣬���� znFAT_Read_File() �������
#define  ZNFAT_READ_FILEX	 //�д˺꣬���� znFAT_Read_FileX() �������
#define  ZNFAT_ADD_DAT	     //�д˺꣬���� znFAT_Add_Dat() �������
#define  ZNFAT_CREATE_DIR	 //�д˺꣬���� znFAT_Create_Dir() �������
#define  ZNFAT_CREATE_FILE	 //�д˺꣬���� znFAT_Create_File() �������
#define  ZNFAT_DEL_FILE	     //�д˺꣬���� znFAT_Del_File() �������
#define  ZNFAT_XCOPY_FILE	 //�д˺꣬���� znFAT_XCopy_File() �������
#define  ZNFAT_RENAME_FILE	 //�д˺꣬���� znFAT_Rename_File() �������

#define ZNFAT_GET_TOTAL_SIZE  //�д˺꣬���� znFAT_Get_Total_Size() �������
#define znFAT_GET_REMAIN_CAP  //�д˺꣬���� znFAT_Get_Remain_Cap() �������

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

#define ZNFAT_SOC(c) (((c-pArg->FirstDirClust)*(pArg->SectorsPerClust))+pArg->FirstDirSector) // ���ڼ���صĿ�ʼ����
#define CONST const

//�豸��
#define ZNFAT_SDCARD 0 //SD��
#define ZNFAT_UDISK  1 //U��
#define ZNFAT_CFCARD 2 //CF��
#define ZNFAT_OTHER  3 //����
				 //����Ĵ洢�豸������������䣬��ʵ�ֶԸ���洢�豸��֧��
//-------------------------------------------

#define ZNFAT_MAKE_FILE_TIME(h,m,s)	((((unsigned int)h)<<11)+(((unsigned int)m)<<5)+(((unsigned int)s)>>1))	/* ����ָ��ʱ������ļ�ʱ������ */
#define ZNFAT_MAKE_FILE_DATE(y,m,d)	(((((unsigned int)y)+20)<<9)+(((unsigned int)m)<<5)+((unsigned int)d))	/* ����ָ�������յ��ļ��������� */

//DPT:������¼�ṹ����
struct znFAT_PartRecord
{
	uint8_t Active;         //0x80��ʾ�˷�����Ч
	uint8_t StartHead;      //�����Ŀ�ʼ��ͷ
	uint8_t StartCylSect[2];//��ʼ����������
	uint8_t PartType;       //��������
	uint8_t EndHead;        //�����Ľ���ͷ
	uint8_t EndCylSect[2];  //��������������
	uint8_t StartLBA[4];    //�����ĵ�һ������
	uint8_t Size[4];        //�����Ĵ�С
};

//MBR:��������������0��������������
struct znFAT_PartSector
{
	uint8_t	PartCode[446]; //MBR����������
	struct	znFAT_PartRecord Part[4];   //4��������¼
	uint8_t	BootSectSig0;  //55
	uint8_t	BootSectSig1;  //AA
};


//znFAT�ж�BPB�Ķ�������  һ��ռ��90���ֽ�
struct znFAT_BPB
{
	uint8_t BS_jmpBoot[3];     //��תָ��            offset: 0
	uint8_t BS_OEMName[8];     //                    offset: 3
	uint8_t BPB_BytesPerSec[2];//ÿ�����ֽ���        offset:11
	uint8_t BPB_SecPerClus[1]; //ÿ��������          offset:13
	uint8_t BPB_RsvdSecCnt[2]; //����������Ŀ        offset:14
	uint8_t BPB_NumFATs[1];    //�˾���FAT����       offset:16
	uint8_t BPB_RootEntCnt[2]; //znFATΪ0            offset:17
	uint8_t BPB_TotSec16[2];   //znFATΪ0            offset:19
	uint8_t BPB_Media[1];      //�洢����            offset:21
	uint8_t BPB_FATSz16[2];    //znFATΪ0            offset:22
	uint8_t BPB_SecPerTrk[2];  //�ŵ�������          offset:24
	uint8_t BPB_NumHeads[2];   //��ͷ��              offset:26
	uint8_t BPB_HiddSec[4];    //FAT��ǰ��������     offset:28
	uint8_t BPB_TotSec32[4];   //�þ���������        offset:32
	uint8_t BPB_FATSz32[4];    //һ��FAT��������     offset:36
	uint8_t BPB_ExtFlags[2];   //znFAT����           offset:40
	uint8_t BPB_FSVer[2];      //znFAT����           offset:42
	uint8_t BPB_RootClus[4];   //��Ŀ¼�غ�          offset:44
	uint8_t FSInfo[2];         //��������FSINFO������offset:48
	uint8_t BPB_BkBootSec[2];  //ͨ��Ϊ6             offset:50
	uint8_t BPB_Reserved[12];  //��չ��              offset:52
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
	uint8_t deName[8];			// �ļ��������㲿���Կո񲹳�
	uint8_t deExtension[3]; 	// ��չ�������㲿���Կո񲹳�
	uint8_t deAttributes;   	// �ļ�����
	uint8_t deLowerCase;    	// 0
	uint8_t deCHundredth;   	// ����
	uint8_t deCTime[2];     	// ����ʱ��
	uint8_t deCDate[2];     	// ��������
	uint8_t deADate[2];     	// ��������
	uint8_t deHighClust[2];		// ��ʼ�صĸ���
	uint8_t deMTime[2];     	// ������޸�ʱ��
	uint8_t deMDate[2];     	// ������޸�����
	uint8_t deLowCluster[2]; 	// ��ʼ�صĵ���
	uint8_t deFileSize[4];      // �ļ���С
};

//znFAT��ʼ��ʱ��ʼ����װ�����½ṹ����
struct znFAT_Init_Arg
{
	uint8_t		DEV_No;

	uint8_t		BPB_Sector_No;		//BPB����������
	uint32_t	Total_Size;			//���̵�������

	uint32_t	FirstDirClust;		//��Ŀ¼�Ŀ�ʼ��
	uint32_t	BytesPerSector;		//ÿ���������ֽ���
	uint32_t	FATsectors;			//FAT����ռ������
	uint32_t	SectorsPerClust;	//ÿ�ص�������
	uint32_t	FirstFATSector;		//��һ��FAT����������
	uint32_t	FirstDirSector;		//��һ��Ŀ¼��������
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
	uint8_t		FileName[15];       //�ļ���
	uint32_t	FileStartCluster;   //�ļ��Ŀ�ʼ��

	uint32_t	FileCurCluster;		//�ļ��ĵ�ǰ��
	uint32_t	FileSize;	    	//�ļ���С
	uint32_t	FileCurSector;	    //�ļ��ĵ�ǰ����
	uint16_t	FileCurPos;         //�ļ��ڵ�ǰ�����е�λ��
	uint32_t	FileCurOffset;	    //�ļ��ĵ�ǰƫ����
	uint32_t	Rec_Sec;            //�ļ����ļ�/Ŀ¼�����ڵ�����
	uint16_t	nRec;               //�ļ����ļ�/Ŀ¼�����������е�λ��

	uint8_t		FileAttr;			//�ļ�����
	struct znFAT_Time	FileCreateTime;	    //�ļ��Ĵ���ʱ��
	struct znFAT_Date	FileCreateDate;	    //�ļ��Ĵ�������
	struct znFAT_Time	FileMTime;          //�ļ����޸�ʱ��
	struct znFAT_Date	FileMDate;          //�ļ����޸�����
	struct znFAT_Date	FileADate;          //�ļ��ķ�������
};

struct znFAT_FSInfo //znFAT���ļ�ϵͳ��Ϣ�ṹ
{
	uint8_t Head[4];
	uint8_t Resv1[480];
	uint8_t Sign[4];
	uint8_t Free_Cluster[4];
	uint8_t Last_Cluster[4];
	uint8_t Resv2[14];
	uint8_t Tail[2];
};

//��������
void znFAT_Device_Init();

uint32_t znFAT_Get_Total_Size(void); //��ȡ������
uint32_t znFAT_Get_Remain_Cap(void); //��ȡʣ������

//�ļ�ϵͳ��ʼ��
void znFAT_Init(void);

//����Ŀ¼
uint32_t znFAT_Enter_Dir(CONST int8_t *path);

//���ļ���֧��ͨ��
uint8_t znFAT_Open_File(struct znFAT_FileInfoStruct *pfi,CONST int8_t *filepath,uint32_t item,uint8_t is_file);

//�ļ���λ
uint8_t znFAT_Seek_File(struct znFAT_FileInfoStruct *pfi,uint32_t offset);

//��ȡ�ļ�����
uint32_t znFAT_Read_File(struct znFAT_FileInfoStruct *pfi,uint32_t offset,uint32_t len,uint8_t *pbuf);

//��ȡ�ļ����ݲ��������ݴ���
uint32_t znFAT_Read_FileX(struct znFAT_FileInfoStruct *pfi,uint32_t offset,uint32_t len,void (*pfun)(uint8_t));

//���ļ�׷������
uint32_t znFAT_Add_Dat(struct znFAT_FileInfoStruct *pfi,uint32_t len,uint8_t *pbuf);

//����Ŀ¼��֧�ִ���ʱ��
uint8_t znFAT_Create_Dir(CONST int8_t *dirpath,uint8_t *ptd);

//�����ļ���֧�ִ���ʱ��
uint8_t znFAT_Create_File(struct znFAT_FileInfoStruct *pfi,CONST int8_t *filepath,uint8_t *ptd);

//ɾ���ļ�
uint8_t znFAT_Del_File(CONST int8_t *filepath);

//�ļ�������֧�ֶ��豸���ļ�����
uint8_t znFAT_XCopy_File(struct znFAT_Init_Arg *pArg1,struct znFAT_Init_Arg *pArg2,CONST int8_t *sfilename,CONST int8_t *tfilename,uint8_t *file_buf,uint32_t buf_size,uint8_t *pt);

//�ļ��ر�
uint8_t znFAT_File_Close(struct znFAT_FileInfoStruct *pfi);

//�ļ�������
uint8_t znFAT_Rename_File(CONST int8_t *filename,CONST int8_t *newfilename);

#endif /* ZNFAT_H_ */
