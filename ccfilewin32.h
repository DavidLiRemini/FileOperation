#pragma once
#include "ccfilestrategy.h"
#include "platformconfig.h"
#if CCTARGET_PLATFORM == CCPLATFORM_WIN32
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



class CC_file_win32 : public CC_file_strategy
{
public:
	void Create(std::string path)override;
	intptr_t Copy(std::string sourcePath, std::string destPath)override;
	void Delete(std::string path)override;
	bool Exists(std::string path)override;
	bool CanRead(std::string path)override;
	bool CanWrite(std::string path)override;
	bool CanExecute(std::string path)override;

	//************************************
	// @Method:    GetAccessControl
	// @Returns:   FileAccess
	// @Parameter: path
	// @Brief:	取得指定文件访问权限
	//************************************
	FileAccess GetAccessControl(std::string path);
	//************************************
	// @Method:    GetAttributes
	// @Returns:   FileAttributes
	// @Parameter: path
	// @Brief:	获取指定路径文件的属性
	//************************************
	FileAttributes GetAttributes(std::string path);
	//************************************
	// @Method:    GetCreateTime
	// @Returns:   DateTime
	// @Parameter: path
	// @Brief:	获取指定文件创建时间。
	//************************************
	DateTime GetCreateTime(std::string path);
	//************************************
	// @Method:    GetLastAccessTime
	// @Returns:   DateTime
	// @Parameter: path
	// @Brief:	获取指定文件最后一次访问时间。
	//************************************
	DateTime GetLastAccessTime(std::string path);
	//************************************
	// @Method:    GetLastModifyTime
	// @Returns:   DateTime
	// @Parameter: path
	// @Brief:	获取文件最后一次修改时间。
	//************************************
	DateTime GetLastModifyTime(std::string path);
private:
	static long CopyFileData(FileStream* src, FileStream* dest);
};

#endif