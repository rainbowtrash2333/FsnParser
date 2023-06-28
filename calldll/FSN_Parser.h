#pragma once

#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>




#define READ_SIZE 100
/**
* @brief ����ͼ��ṹ
*/
typedef struct
{
	unsigned int Data[32];		/**< ͼ����ֺŵ������� */
}BOC_BOC_TImageSNoData;


/**
* @brief ͼ����ֺ���ṹ
*/
typedef struct
{
	short Num;				/**< �ַ��� */
	short Height, width;	/**< ÿ��ͼ���ַ��߶ȺͿ�� */
	short Reserve2;			/**< ������2 */
	BOC_BOC_TImageSNoData SNo[12];
}BOC_TImageSNo;

typedef struct
{
	USHORT HeadStart[4];
	USHORT HeadString[6];
	UINT Counter;
	USHORT HeadEnd[4];
}BOC_Header_Fromat;

/**
* @brief �ļ���¼�ṹ
*/
typedef struct
{
	USHORT Date;			/**< �鳮�������� Date=((Year-1980)<<9)+(Month<<5)+Day*/   //
	USHORT Time;			/**< �鳮����ʱ�� Time=(Hour<<11)+(Minute<<5)+(Second>>1)*/
	USHORT tfFlag;			/**< �桢�١��к;ɱұ�־ */					 //
	USHORT ErrorCode[3];	/**< ������(3��) */
	USHORT MoneyFlag[4];	/**< ���ұ�־ */								 //
	USHORT Ver;				/**< �汾�� */
	USHORT Valuta;			/**< ��ֵ */									//				
	USHORT CharNUM;			/**< ���ֺ����ַ��� */
	USHORT SNo[12];			/**< ���ֺ��� */                               //
	USHORT MachineSNo[24];	/**< ���߱�� */
	USHORT Operateor_No;		/**< ������1 */
}BOC_TagData_Fromat;


typedef  struct {   //Ϊ�䳤����
	UINT  u32_ImgDataLen;  //ͼƬ�ļ�����
	UINT  u32_Image_Width;//ͼ����
	UINT  u32_Image_Height;//ͼ��߶�
	UCHAR   u8_Filetype[8];//�ļ�����.jpg  ���� .bmp  
	UCHAR   Reserve[60];  //�ճ�60���ֽ�
}TKTImageSNo_File;
//u32_ImgDataLen
//ͼƬ����

typedef  struct {
	USHORT  u16_Date;
	USHORT  u16_Time;
	USHORT  u16_tfFlag;
	USHORT  u16_ErrorCode[3];
	USHORT  u16_MoneyFlag[4];
	USHORT  u16_Ver;
	USHORT  u16_Valuta;
	USHORT  u16_CharNum;
	USHORT  u16_SNo[12];
	USHORT  u16_MachineSNo[24];
	USHORT  u16_Reservel[20];  //�ճ�40���ֽ�
	//TKTImageSNo_File  ImageSNo_File;
}TKFSN_Record_File;

//2015-10���ģ������ֵ����
typedef struct
{
	USHORT machineSNo_Length : 4;
	USHORT machine_Length : 4;
	USHORT machine_Type : 4;
	USHORT money_Type : 4;
}FSN_Reserve, * PFSN_Reserve;

#pragma pack()

enum ImageType
{
	FSN_IMG_Standard = 0xC0,
	FSN_IMG_JPG,
};


typedef struct
{
	BOC_TagData_Fromat btf;
	USHORT uFileFlag;		//�ļ�����
	DWORD  dwOffset;		//�ļ�ͼƬ����
	CHAR   bSame;			//�Ƿ�ϸ�
	INT	   llLen;			//ͼƬ�ļ�����
	BYTE   uFileType;		//�ļ�����
	int    nImgWidth;
	int    nImgHeight;
}BOC_TagRecord_Data;

struct Money_Info
{
	std::string Id;
	std::string Value;
	std::string Kinds;
	std::string IsReal;
	std::string time;
	std::string CharNum;
	std::string MachineSNo;
};
using namespace std;
class FsnParser
{
public:
	std::vector<Money_Info> ReadTk(const char* filename, int ncount);
	Money_Info  InsList(vector<BOC_TagRecord_Data*>& vecRecord, int nStartPos);
	vector<BOC_TagRecord_Data*> m_vecRecordData;
private:
	vector<Money_Info> m_vecMoneyInfo;
};