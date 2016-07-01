#pragma once
#include "common.h"

class CCFile
{
public:
	//************************************
	// @Method:    Create
	// @Returns:   FileStream*
	// @Parameter: path 文件路径及文件名
	// @Brief:	创建文件，返回文件流
	//************************************
	static FileStream* Create(std::string path);
	//************************************
	// @Method:    Copy
	// @Returns:   intptr_t
	// @Parameter: sourcePath 源文件路径
	// @Parameter: destPath 目标文件路径
	// @Brief:	将文件从源sourcepath拷贝至destpath
	//************************************
	static intptr_t Copy(std::string sourcePath, std::string destPath);
	static void CopyAsync(std::string sourcePath, std::string destPath);
	static intptr_t CopySync(std::string sourcepath, std::string destPath);
	//************************************
	// @Method:    Delete
	// @Returns:   void
	// @Parameter: path
	// @Brief:	删除指定文件
	//************************************
	static void Delete(std::string path);
	//************************************
	// @Method:    Exists
	// @Returns:   bool
	// @Parameter: path
	// @Brief:	判断指定路径文件是否存在
	//************************************
	static bool Exists(std::string path);

	//************************************
	// @Method:    GetAccessControl
	// @Returns:   FileAccess
	// @Parameter: path
	// @Brief:	取得文件访问权限(可读写或者只读)
	//************************************
	static FileAccess GetAccessControl(std::string path);
	//************************************
	// @Method:    GetAttributes
	// @Returns:   FileAttributes
	// @Parameter: path
	// @Brief:	获取文件属性，是目录，归档文件或者普通文件
	//************************************
	static FileAttributes GetAttributes(std::string path);
	//************************************
	// @Method:    GetCreateTime
	// @Returns:   DateTime
	// @Parameter: path
	// @Brief:	取得文件创建时间
	//************************************
	static DateTime GetCreateTime(std::string path);
	//************************************
	// @Method:    GetLastAccessTime
	// @Returns:   DateTime
	// @Parameter: path
	// @Brief:	取得文件上次访问时间
	//************************************
	static DateTime GetLastAccessTime(std::string path);
	//************************************
	// @Method:    GetLastModifyTime
	// @Returns:   DateTime
	// @Parameter: path
	// @Brief:	取得文件上次修改时间
	//************************************
	static DateTime GetLastModifyTime(std::string path);
	//************************************
	// @Method:    Move
	// @Returns:   void
	// @Parameter: sourcePath
	// @Parameter: destPath
	// @Brief:	移动指定源路径文件到目标路径
	//************************************
	static void Move(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    Open
	// @Returns:   FileStream*
	// @Parameter: path
	// @Parameter: 
	// @Brief:	以指定模式打开文件，返回文件流
	//************************************
	static FileStream* Open(std::string path, FileMode);
	//************************************
	// @Method:    OpenRead
	// @Returns:   FileStream*
	// @Parameter: path 要打开的文件
	// @Brief:	以只读模式打开文件，返回文件流
	//************************************
	static FileStream* OpenRead(std::string path);
	//************************************
	// @Method:    OpenWrite
	// @Returns:   FileStream*
	// @Parameter: path	要打开的文件
	// @Brief:	打开一个文件或者创建一个新文件进行写入，打开一个存在的文件进行写入会清空源文件内容
	//************************************
	static FileStream* OpenWrite(std::string path);
	//************************************
	// @Method:    ReadAllBytes
	// @Returns:   intptr_t
	// @Parameter: path	文件路径
	// @Parameter: destBuffer
	// @Parameter: length	用户缓冲总大小
	// @Brief:	打开文件从文件中读出数据写入用户buffer中，
	//************************************
	static intptr_t ReadAllBytes(std::string path, char destBuffer[], size_t length);
	//************************************
	// @Method:    ReadAllLines
	// @Returns:   long
	// @Parameter: path	文件路径
	// @Parameter: destContainer	用户目标数据
	// @Brief:	打开指定文件，从文件中以行为单位读出数据，写入目标位置
	//************************************
	static long ReadAllLines(std::string path, std::vector<std::string>&destContainer);
	//************************************
	// @Method:    ReadAllText
	// @Returns:   std::string	返回字符串
	// @Parameter: path	文件路径
	// @Brief:	打开指定文件读出文件内容并以字符串形式返回它
	//************************************
	static std::string ReadAllText(std::string path);
	//************************************
	// @Method:    SetAccessControl
	// @Returns:   void
	// @Parameter: mode	访问权限
	// @Brief:	设置文件访问权限
	//************************************
	static void SetAccessControl(FileMode mode);

	//************************************
	// @Method:    Replace
	// @Returns:   bool	成功或失败
	// @Parameter: sourcePath 源文件路径
	// @Parameter: replaceFilePath	替换文件路径
	// @Parameter: backPath	备份文件路径
	// @Brief:	用目标文件的文件内容替换源文件内容，并将源文件备份至backPath
	//************************************
	static bool Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath);
	//************************************
	// @Method:    WriteAllBytes
	// @Returns:   intptr_t
	// @Parameter: path	打开文件
	// @Parameter: sourceData
	// @Parameter: len	缓冲区数据长度
	// @Brief:	打开文件，将用户缓冲区的内容全部写入文件。
	//************************************
	static intptr_t WriteAllBytes(std::string path, const char sourceData[], size_t len);
	//************************************
	// @Method:    WriteAllLines
	// @Returns:   long
	// @Parameter: path
	// @Parameter: sourceData
	// @Brief:	打开文件，将用户数据全部写入文件
	//************************************
	static long WriteAllLines(std::string path, const std::vector<std::string>& sourceData);
	//************************************
	// @Method:    WriteAllText
	// @Returns:   intptr_t
	// @Parameter: path 文件路径
	// @Parameter: content	写入内容
	// @Brief:	打开文件，将内容全部写入文件。
	//************************************
	static intptr_t WriteAllText(std::string path, std::string& content);
private:
	//************************************
	// @Method:    CopyFileData
	// @Returns:   long 返回拷贝字节数
	// @Parameter: src	源文件
	// @Parameter: dest	目标文件
	// @Brief:	拷贝文件数据
	//************************************
	static long CopyFileData(FileStream* src, FileStream* dest);
};