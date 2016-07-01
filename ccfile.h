#ifndef _CC_FILE_H_
#define _CC_FILE_H_


#include "ccfilestrategy.h"
#include "common.h"
#include <vector>

class CC_File 
{
public:
	CC_File();
	~CC_File();
	//************************************
	// @Method:    Create
	// @Returns:   void
	// @Parameter: path
	// @Brief:	在指定目录下创建指定名字文件，如该文件已存在则会覆盖。
	//************************************
	void Create(std::string path) ;
	//************************************
	// @Method:    Copy
	// @Returns:   intptr_t
	// @Parameter: sourcePath
	// @Parameter: destPath
	// @Brief:	拷贝指定目录文件到目标路径下
	//************************************
	intptr_t Copy(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    Delete
	// @Returns:   void
	// @Parameter: path
	// @Brief:	删除指定文件
	//************************************
	void Delete(std::string path) ;
	//************************************
	// @Method:    Exists
	// @Returns:   bool
	// @Parameter: path
	// @Brief:	判断指定文件是否存在
	//************************************
	bool Exists(std::string path) ;

	//************************************
	// @Method:    CanRead
	// @Returns:   bool
	// @Parameter: path
	// @Brief:	判断指定文件是否可读
	//************************************
	bool CanRead(std::string path);
	//************************************
	// @Method:    CanWrite
	// @Returns:   bool
	// @Parameter: path
	// @Brief:	判断指定文件是否可写
	//************************************
	bool CanWrite(std::string path) ;
	//************************************
	// @Method:    CanExecute
	// @Returns:   bool
	// @Parameter: path
	// @Brief:	判断指定文件是否可执行
	//************************************
	bool CanExecute(std::string path) ;
	//************************************
	// @Method:    Move
	// @Returns:   void
	// @Parameter: sourcePath
	// @Parameter: destPath
	// @Brief:	移动指定文件到目标文件夹下
	//************************************
	void Move(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    Open
	// @Returns:   FileStream*
	// @Parameter: path
	// @Parameter: mode
	// @Brief:	以相应的模式打开指定的文件，返回文件流
	//************************************
	FileStream* Open(std::string path, FileMode mode) ;
	//************************************
	// @Method:    OpenRead
	// @Returns:   FileStream*
	// @Parameter: path
	// @Brief:	以只读模式打开指定文件，返回文件流
	//************************************
	FileStream* OpenRead(std::string path);
	//************************************
	// @Method:    OpenWrite
	// @Returns:   FileStream*
	// @Parameter: path
	// @Brief:	以只写模式打开文件，返回文件流
	//************************************
	FileStream* OpenWrite(std::string path) ;
	//************************************
	// @Method:    ReadAllBytes
	// @Returns:   intptr_t
	// @Parameter: path
	// @Parameter: destBuffer
	// @Parameter: length
	// @Brief:	打开指定文件，读取长度为length的内容到缓冲区，若缓冲区长度小于文件长度，则出错。
	//************************************
	intptr_t ReadAllBytes(std::string path, char destBuffer[], size_t length) ;
	//************************************
	// @Method:    ReadAllLines
	// @Returns:   long
	// @Parameter: path
	// @Parameter: destContainer	用户缓冲
	// @Brief:	打开文件，以行为单位读取文件内容到用户缓冲
	//************************************
	long ReadAllLines(std::string path, std::vector<std::string>& destContainer) ;
	//************************************
	// @Method:    ReadAllText
	// @Returns:   std::string
	// @Parameter: path
	// @Brief:	打开指定文件，读取全部文件内容到字符串，返回该字符串
	//************************************
	std::string ReadAllText(std::string path);
	//************************************
	// @Method:    Replace
	// @Returns:   bool
	// @Parameter: sourcePath
	// @Parameter: replaceFilePath
	// @Parameter: backPath
	// @Brief:	用目标路径文件内容替换源路径目标文件内容，并将源文件备份至backpath
	//************************************
	bool Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath) ;
	//************************************
	// @Method:    WriteAllBytes
	// @Returns:   intptr_t
	// @Parameter: path
	// @Parameter: sourceData
	// @Parameter: len
	// @Brief:	打开文件，将目标缓冲区的内容全部写入文件。 返回写入字节数。
	//************************************
	intptr_t WriteAllBytes(std::string path, const char sourceData[], size_t len);
	//************************************
	// @Method:    WriteAllLines
	// @Returns:   long
	// @Parameter: path
	// @Parameter: sourceData
	// @Brief:	打开指定文件，将指定缓冲区数据全部写入文件，返回写入数据行数。
	//************************************
	long WriteAllLines(std::string path, const std::vector<std::string>& sourceData);
	//************************************
	// @Method:    WriteAllText
	// @Returns:   intptr_t
	// @Parameter: path
	// @Parameter: content
	// @Brief:	打开指定文件，将字符串内容全部写入该文件，返回写入的字节数。
	//************************************
	intptr_t WriteAllText(std::string path, std::string& content);
private:
	static CC_file_strategy* instance;
};

#endif