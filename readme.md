# FsnParser

解码验钞机冠字号文件（.FSN）

## FsnParser

为窗口应用，标准输入文件目录，会解析目录下所有FNS文件，在软件同位置生成csv文件。

已编译了release版，可下载直接使用。

## FsnParserDLL

生成dll，暴露函数为`std::vector<Money_Info>parser(char*)`, 其中Money_Info为结构体，结构如下：
```c
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
```
