// calldll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include "FSN_Parser.h"
#include <filesystem>

//typedef void (*Test_func)(double*, double*, int, int, int); //
typedef std::vector<Money_Info>(*Parser_func)(const char*);
int main()
{
	HMODULE hModule = LoadLibrary(TEXT("C:\\Users\\Twikura\\source\\repos\\FsnParser\\x64\\Release\\FsnParserDll.dll"));
	if (hModule == NULL) {
		MessageBox(NULL, TEXT("获得句柄失败"), TEXT("警告："), MB_OK);
		return -1;
	}
	Parser_func parser = (Parser_func)GetProcAddress(hModule, "parser");
  //  std::cout << "Hello World!\n";
	std::vector<Money_Info> Vec_MoneyInfo;
	Vec_MoneyInfo = parser("D:\\data\\1.FSN");
	for (int i = 0; i < Vec_MoneyInfo.size(); i++) {
		cout <<"CharNum:  " << Vec_MoneyInfo[i].CharNum << endl;
		cout <<"Id: " << Vec_MoneyInfo[i].Id << endl;
		cout << "IsReal: " << Vec_MoneyInfo[i].IsReal << endl;
		cout << "Kinds: " << Vec_MoneyInfo[i].Kinds << endl;
		cout << "MachineSNo: " << Vec_MoneyInfo[i].MachineSNo << endl;
		cout << "time: " << Vec_MoneyInfo[i].time << endl;
		cout << "Value: " << Vec_MoneyInfo[i].Value << endl;
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
