#pragma once

#include <string>
#include <stdio.h>
enum FileMode
{
	//只读模式打开
	READDONLY,
	//读写方式
	RDWRITE,
	//只写方式 不存在文件将建立，存在将清空
	WRITEONLY,
	//读写方式，不存在将建立，存在将被清空
	WRITE,
	//追加方式打开，不存在将建立，存在追加
	APPEND,
	//追加且可读
	APPENDANDRD,
	//二进制方式，以二进制方式打开文件，而非以普通文本方式
	BINARY
};

class FileStream
{
private:
	FILE*fileDescriptor;
public:
	FileStream()
		:fileDescriptor(nullptr){

	}
	FILE*& Getfd()
	{
		return fileDescriptor;
	}
	~FileStream()
	{
		if (fileDescriptor)
		{
			::fclose(fileDescriptor);
			fileDescriptor = nullptr;
		}
	}

};

class DateTime
{
public:
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int sec;
	DateTime()
		:year(0), month(0), day(0),
		hour(0), minute(0), sec(0){

	}
	std::string FormatTime()
	{
		char buf[30] = { 0 };
#if defined(_WIN32)
		sprintf_s(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, sec);
#elif defined(__GNUC__)
		snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, sec);
#endif
		return buf;
	}
};

enum FileAttributes
{
	UNKNOWN = -1,
	//普通文件，无访问限制
	NORMAL = 0x00,
	//只读文件
	RDONLY = 0x01,
	//隐藏文件
	HIDDEN = 0x02,
	//系统文件
	SYSTEM = 0x04,
	//目录
	DIRECTORY = 0x10,
	//归档文件
	ARCH = 0x20
};

struct DirectoryInfo
{
public:
	unsigned long totalFileNumber;
	unsigned long totalDirectoryNumber;
	double totalDirectorySize;
	DirectoryInfo() :
		totalFileNumber(0),
		totalDirectoryNumber(0),
		totalDirectorySize(0){

	}
};