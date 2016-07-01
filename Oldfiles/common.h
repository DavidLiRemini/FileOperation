#pragma once

#include <vector>
#include <errno.h>
#include <io.h>
#include <direct.h>
#include <deque>
#include <stack>
enum FileAccess
{
	//文件存在
	EXISTENCEONLY = 0x00,
	//文件可写
	WRITEPERMISSION = 0x02,
	//文件只读
	READPERMISSION = 0x04,
	//文件可读写
	READANDWRITEPERMISSION = 0x06,
	//下面两项主要是为了设置文件权限, 所有文件皆有可读权限，因此S_CANWRITE 等于 S_CANREAD|S_CANWRITE
	S_CANREAD = 0x0100,
	S_CANWRITE = 0x0080
};
enum FileAttributes
{
	UNKNOWN = -1,
	//普通文件，无访问限制
	NORMAL=0x00,
	//只读文件
	RDONLY=0x01,
	//隐藏文件
	HIDDEN=0x02,
	//系统文件
	SYSTEM=0x04,
	//目录
	DIRECTORY=0x10,
	//归档文件
	ARCH=0x20
};

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

struct FolderNode
{
	std::string name;
	FileAttributes nodeAttr;
	bool hasDirectory = false;
	std::vector<FolderNode>subChildren;
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
		sprintf_s(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, sec);
		return buf;
	}
};