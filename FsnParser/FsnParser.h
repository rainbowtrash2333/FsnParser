
#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
 
 
 
 
#define READ_SIZE 100
/**
* @brief 单个图像结构
*/
typedef struct
{
	unsigned int Data[32];		/**< 图像冠字号点阵数据 */
}BOC_BOC_TImageSNoData;
 
 
/**
* @brief 图像冠字号码结构
*/
typedef struct
{
	short Num;				/**< 字符数 */
	short Height, width;	/**< 每个图像字符高度和宽度 */
	short Reserve2;			/**< 保留字2 */
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
* @brief 文件记录结构
*/
typedef struct
{
	USHORT Date;			/**< 验钞启动日期 Date=((Year-1980)<<9)+(Month<<5)+Day*/   //
	USHORT Time;			/**< 验钞启动时间 Time=(Hour<<11)+(Minute<<5)+(Second>>1)*/
	USHORT tfFlag;			/**< 真、假、残和旧币标志 */					 //
	USHORT ErrorCode[3];	/**< 错误码(3个) */
	USHORT MoneyFlag[4];	/**< 货币标志 */								 //
	USHORT Ver;				/**< 版本号 */
	USHORT Valuta;			/**< 币值 */									//				
	USHORT CharNUM;			/**< 冠字号码字符数 */
	USHORT SNo[12];			/**< 冠字号码 */                               //
	USHORT MachineSNo[24];	/**< 机具编号 */
	USHORT Operateor_No;		/**< 保留字1 */
}BOC_TagData_Fromat;
 
 
typedef  struct {   //为变长类型
	UINT  u32_ImgDataLen;  //图片文件长度
	UINT  u32_Image_Width;//图像宽度
	UINT  u32_Image_Height;//图像高度
	UCHAR   u8_Filetype[8];//文件类型.jpg  或者 .bmp  
	UCHAR   Reserve[60];  //空出60个字节
}TKTImageSNo_File;
//u32_ImgDataLen
//图片数据
 
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
	USHORT  u16_Reservel[20];  //空出40个字节
							   //TKTImageSNo_File  ImageSNo_File;
}TKFSN_Record_File;
 
//2015-10号文，保留字的组成
typedef struct
{
	USHORT machineSNo_Length : 4;
	USHORT machine_Length : 4;
	USHORT machine_Type : 4;
	USHORT money_Type : 4;
}FSN_Reserve, *PFSN_Reserve;
 
#pragma pack()
 
enum ImageType
{
	FSN_IMG_Standard = 0xC0,
	FSN_IMG_JPG,
};
 
 
typedef struct
{
	BOC_TagData_Fromat btf;
	USHORT uFileFlag;		//文件索引
	DWORD  dwOffset;		//文件图片索引
	CHAR   bSame;			//是否合格
	INT	   llLen;			//图片文件长度
	BYTE   uFileType;		//文件类型
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
	std::vector<Money_Info> ReadTk(std::string &filename,int ncount);
	Money_Info  InsList(vector<BOC_TagRecord_Data*>& vecRecord, int nStartPos);
	vector<BOC_TagRecord_Data*> m_vecRecordData;
private:
	vector<Money_Info> m_vecMoneyInfo;
};