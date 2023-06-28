#define _CRT_SECURE_NO_WARNINGS
#include "FSN_Parser.h"
#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void* __cdecl memcpy1(void* dst, const void* src, size_t count)
{
	void* ret = dst;
#if defined(_M_MRX000)||defined(_M_ALPHA)||defined(_M_PPC)
	{
		extern void RtlMoveMemory(void*, const void*, size_t count);
		RtlMoveMemory(dst, src, count);
	}
#else
	while (count--) {
		*(char*)dst = *(char*)src;
		dst = (char*)dst + 1;
		src = (char*)src + 1;
	}
#endif 
	return (ret);
}

//解析FSN文件
//BOC_TagData_Fromat *btf = (BOC_TagData_Fromat*)malloc(sizeof(BOC_TagData_Fromat*));
//int len = sizeof(BOC_TagData_Fromat*);
std::vector<Money_Info> FsnParser::ReadTk(const char* filename, int ncount)
{
	FILE* myf;
	errno_t err = fopen_s(&myf, filename, "rb");
	if (myf == nullptr)
	{
		cout << "Error opening file";
		exit(1);
	}
	char buffer[3072] = { 0 };
	int iread_offset = 0;
	iread_offset += sizeof(BOC_Header_Fromat);
	int read_size = READ_SIZE;							//文件记录
	int position = 0;
	std::vector<Money_Info> Vec_MoneyInfo;
	fseek(myf, 0, 2);
	int fileSize = ftell(myf);                              //求文件的长度
	while (!feof(myf))
	{
		fseek(myf, iread_offset, SEEK_SET);
		position = ftell(myf);
		fread(buffer, read_size, 1, myf);
		iread_offset += READ_SIZE;
		TKFSN_Record_File* pdata = (TKFSN_Record_File*)buffer;
		BOC_TagRecord_Data* bctd = new BOC_TagRecord_Data;				//数据标记记录
		memset(bctd, 0, sizeof(BOC_TagRecord_Data));
		bctd->btf.Date = pdata->u16_Date;
		bctd->btf.Time = pdata->u16_Time;
		bctd->btf.tfFlag = pdata->u16_tfFlag;
		memcpy1(bctd->btf.ErrorCode, pdata->u16_ErrorCode, sizeof(bctd->btf.ErrorCode));
		memcpy1(bctd->btf.MoneyFlag, pdata->u16_MoneyFlag, sizeof(bctd->btf.MoneyFlag));
		bctd->btf.CharNUM = pdata->u16_CharNum;
		memcpy1(bctd->btf.SNo, pdata->u16_SNo, sizeof(bctd->btf.SNo));
		memcpy1(bctd->btf.MachineSNo, pdata->u16_MachineSNo, sizeof(bctd->btf.MachineSNo));
		iread_offset += sizeof(BOC_TImageSNo);
		m_vecRecordData.push_back(bctd);
		ncount++;
		Money_Info m_info = InsList(m_vecRecordData, ncount - 1);
		Vec_MoneyInfo.push_back(m_info);
	}
	fclose(myf);
	return Vec_MoneyInfo;
}
Money_Info  FsnParser::InsList(vector<BOC_TagRecord_Data*>& vecRecord, int nStartPos)
{
	int j = 0;
	unsigned char idLevel = 0;
	int nEndPos = vecRecord.size();
	Money_Info m_info;												//保存Money信息的数据结构
	for (int i = nStartPos; i < nEndPos; i++)
	{
		BOC_TagData_Fromat* pdata = &vecRecord[i]->btf;
		char tmpbuf[26];
		//SNo
		memset(tmpbuf, 0, 26);
		for (j = 0; j < 12; j++)
		{
			tmpbuf[j] = pdata->SNo[j] & 0xFF;
	//		std::cout << tmpbuf[j] << "    ";
		}
		m_info.Id = tmpbuf;//
		//CharNum
		memset(tmpbuf, 0, 26);
		sprintf_s(tmpbuf, "%d", pdata->CharNUM);
		m_info.CharNum = tmpbuf;//
		//面值
		memset(tmpbuf, 0, 26);
		sprintf_s(tmpbuf, "%d", pdata->Valuta);
		m_info.Value = tmpbuf; //
		//MomeyId 币种
		memset(tmpbuf, 0, 26);
		for (j = 0; j < 4; j++)
		{
			tmpbuf[j] = pdata->MoneyFlag[j] & 0xFF;
		}
		m_info.Kinds = tmpbuf; //
		//真伪
		memset(tmpbuf, 0, 26);
		sprintf_s(tmpbuf, "%d", pdata->tfFlag);
		m_info.IsReal = tmpbuf;
		//错误码
		memset(tmpbuf, 0, 26);
		idLevel = (pdata->ErrorCode[1] >> 8);
		//机具编号
		memset(tmpbuf, 0, 26);
		for (j = 0; j < 24; j++)
		{
			tmpbuf[j] = pdata->MachineSNo[j] & 0xFF;
	//		std::cout << tmpbuf[j];
		}
		m_info.MachineSNo = tmpbuf;//
		//时间戳
		memset(tmpbuf, 0, 26);
		sprintf_s(tmpbuf, "%04d-%02d-%02d %02d:%02d:%02d", (pdata->Date >> 9) + 1980, (pdata->Date >> 5) & 0x0F, pdata->Date & 0x1F,
			(pdata->Time >> 11), (pdata->Time >> 5) & 0x3F, (pdata->Time << 1) & 0x3F);
		m_info.time = tmpbuf;//
		return m_info;
	}
}

char* stringToChar(const std::string& str) {
	char* charStr = new char[str.length() + 1];
	std::strcpy(charStr, str.c_str());
	return charStr;
}

std::vector<Money_Info>  parser(const char* path) {
	std::vector<Money_Info> Vec_MoneyInfo;
	FsnParser ftd;
	Vec_MoneyInfo = ftd.ReadTk(path, 0);
	return Vec_MoneyInfo;
}

int parserFile(const char* directoryPath) {
	std::vector<Money_Info> Vec_MoneyInfo;
	std::string outputFilePath = "./data.csv";
	std::ofstream outputFile(outputFilePath);
	if (!outputFile) {
		std::cerr << "无法打开文件" << std::endl;
		return 1;
	}
	outputFile << "金额,冠字号,是否真币,币种,设备编码,点钞时间,Value" << endl;
	try {
		for (const auto& entry : fs::directory_iterator(directoryPath)) {
			if (fs::is_regular_file(entry.path())) {
				Vec_MoneyInfo = parser(stringToChar(entry.path().string()));
				for (int i = 0; i < Vec_MoneyInfo.size(); i++) {
					outputFile << Vec_MoneyInfo[i].CharNum << ",";
					outputFile << Vec_MoneyInfo[i].Id << ",";
					outputFile << Vec_MoneyInfo[i].IsReal << ",";
					outputFile << Vec_MoneyInfo[i].Kinds << ",";
					outputFile << Vec_MoneyInfo[i].MachineSNo << ",";
					outputFile << Vec_MoneyInfo[i].time << ",";
					outputFile << Vec_MoneyInfo[i].Value << ",";
					outputFile << endl;
				}
			}
		}
	}
	catch (const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}
	outputFile.close();
	return 0;
}

int main()
{
	char path[1024];

	printf("输入文件目录 :");
	scanf("%s", path);

	parserFile(path);
	/* int rtn = foo(10);
	std::vector<Money_Info> Vec_MoneyInfo;
	Vec_MoneyInfo = parser("D:\\data\\1.FSN");
	for (int i = 0; i < Vec_MoneyInfo.size(); i++) {
		cout << Vec_MoneyInfo[i].CharNum << endl;
		cout << Vec_MoneyInfo[i].Id << endl;
		cout << Vec_MoneyInfo[i].IsReal << endl;
		cout << Vec_MoneyInfo[i].Kinds << endl;
		cout << Vec_MoneyInfo[i].MachineSNo << endl;
		cout << Vec_MoneyInfo[i].time << endl;
		cout << Vec_MoneyInfo[i].Value << endl;
	}*/
	return 0;
}
