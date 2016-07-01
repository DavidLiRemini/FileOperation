#ifndef _CC_DIRECTORY_H_
#define _CC_DIRECTORY_H_

#include "ccdirectorystrategy.h"
#include "common.h"
#include <vector>

class CC_Directory
{
public:
	CC_Directory();
	~CC_Directory();

	//************************************
	// @Method:    CreateDirectory
	// @Returns:   void
	// @Parameter: path
	// @Brief:	创建目录
	//************************************
	void CreateDirectory(std::string path);
	//************************************
	// @Method:    RemoveDirectory
	// @Returns:   void
	// @Parameter: path
	// @Brief:	删除目录
	//************************************
	void RemoveDirectory(std::string path);
	//************************************
	// @Method:    GetSubDirectoryCollection
	// @Returns:   std::vector<std::string>
	// @Parameter: path
	// @Brief:	取得路径下子目录集合，不进行递归查找
	//************************************
	std::vector<std::string>GetSubDirectoryCollection(std::string path);
	//************************************
	// @Method:    GetFiles
	// @Returns:   std::vector<std::string>
	// @Parameter: path
	// @Brief:	取得路径下文件集合，不递归查找
	//************************************
	std::vector<std::string> GetFiles(std::string path);
	//************************************
	// @Method:    GetFilesAndDirectories
	// @Returns:   std::vector<std::string>
	// @Parameter: path
	// @Brief:	取得路径下文件和子文件夹集合，文件在前文件夹在后
	//************************************
	std::vector<std::string> GetFilesAndDirectories(std::string path);
	//************************************
	// @Method:    Exists
	// @Returns:   bool
	// @Parameter: path
	// @Brief:	判断指定路径文件夹是否存在
	//************************************
	bool Exists(std::string path);
	//************************************
	// @Method:    GetCurrentDirectory
	// @Returns:   std::string
	// @Brief:	取得当前工作目录
	//************************************
	std::string GetCurrentDirectory();
	//************************************
	// @Method:    GetDirectoryRoot
	// @Returns:   std::string
	// @Parameter: path
	// @Brief:	取得指定路径根目录
	//************************************
	std::string GetDirectoryRoot(std::string path);
	//************************************
	// @Method:    GetParentDirectory
	// @Returns:   std::string
	// @Parameter: path
	// @Brief:	取得指定目录的上级目录
	//************************************
	std::string GetParentDirectory(std::string path);
	//************************************
	// @Method:    MoveDirectory
	// @Returns:   void
	// @Parameter: sourcePath 目标文件夹
	// @Parameter: destPath	
	// @Brief:	移动源路径目标文件夹到目标路径下
	//************************************
	void MoveDirectory(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    CopyDirectory
	// @Returns:   void
	// @Parameter: sourcePath	源目标文件夹
	// @Parameter: destPath
	// @Brief:	拷贝源路径目标文件夹到目标路径之下
	//************************************
	void CopyDirectory(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    Search
	// @Returns:   void
	// @Parameter: path			路径
	// @Parameter: vec	@out	查找结果
	// @Parameter: realSymbol	查找符号
	// @Brief:	查找指定路径下符合指定符号的文件集合
	//************************************
	void Search(std::string path, std::vector<std::string>& vec, std::string realSymbol);
	//************************************
	// @Method:    DisplayDirectoryInformation
	// @Returns:   void
	// @Parameter: path
	// @Brief:	获得指定目录详细信息，目录占用大小，子目录数，总文件数。
	//************************************
	void DisplayDirectoryInformation(std::string path);
private:
	static CC_directory_strategy* instance;
};

#endif