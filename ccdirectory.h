#pragma once
#include "common.h"


class CCDirectory
{
public:
	//************************************
	// @Method:    CreateDirectory
	// @Returns:   void
	// @Parameter: path	目录路径
	// @Brief:	创建目录，若目录存在则不做更改
	//************************************
	static void CreateDirectory(std::string path);
	//************************************
	// @Method:    Delete
	// @Returns:   void
	// @Parameter: path	目录路径
	// @Brief:	删除目录
	//************************************
	static void Delete(std::string path);
	//************************************
	// @Method:    GetSubDirectoriesCollection
	// @Returns:   std::vector<std::string> 子目录集合
	// @Parameter: path	指定目录
	// @Brief:	取得指定目录下所有子目录
	//************************************
	static std::vector<std::string> GetSubDirectoriesCollection(std::string path);
	//************************************
	// @Method:    GetFiles
	// @Returns:   std::vector<std::string>
	// @Parameter: path	指定目录
	// @Brief:	返回指定目录下所有文件集合
	//************************************
	static std::vector<std::string> GetFiles(std::string path);
	//************************************
	// @Method:    GetFilesAndDirectories
	// @Returns:   std::vector<std::string>
	// @Parameter: path
	// @Brief:	返回指定目录下所有文件及子目录，
	//************************************
	static std::vector<std::string> GetFilesAndDirectories(std::string path);
	//************************************
	// @Method:    Exists
	// @Returns:   bool
	// @Parameter: path 指定目录路径
	// @Brief:	判断指定目录是否存在
	//************************************
	static bool Exists(std::string path);
	//************************************
	// @Method:    GetAccessControl
	// @Returns:   void
	// @Parameter: path
	// @Brief:	获取指定目录访问权限
	//************************************
	static void GetAccessControl(std::string path);
	//************************************
	// @Method:    GetCreateTime
	// @Returns:   DateTime 时间	2016-06-23 16:45:46
	// @Parameter: path	指定目录
	// @Brief:	获取指定目录创建时间
	//************************************
	static DateTime GetCreateTime(std::string path);
	//************************************
	// @Method:    GetLastAccessTime
	// @Returns:   DateTime
	// @Parameter: path 指定目录。
	// @Brief:	获取目录上次访问时间
	//************************************
	static DateTime GetLastAccessTime(std::string path);
	//************************************
	// @Method:    GetLastWriteTime
	// @Returns:   DateTime
	// @Parameter: path
	// @Brief:	获取目录上次修改时间（似乎无意义）
	//************************************
	static DateTime GetLastWriteTime(std::string path);
	//************************************
	// @Method:    GetCurrentDirectory
	// @Returns:   std::string
	// @Brief:	获取当前目录
	//************************************
	static std::string GetCurrentDirectory();
	//************************************
	// @Method:    GetDirectoryRoot
	// @Returns:   std::string
	// @Parameter: path
	// @Brief:	获取指定目录根目录或盘符
	//************************************
	static std::string GetDirectoryRoot(std::string path);
	//************************************
	// @Method:    GetParentDirectory
	// @Returns:   std::string
	// @Parameter: path
	// @Brief:	取得指定目录父目录
	//************************************
	static std::string GetParentDirectory(std::string path);
	//************************************
	// @Method:    MoveDirectory
	// @Returns:   void
	// @Parameter: sourcePath	源目录路径
	// @Parameter: destPath	目标目录路径
	// @Brief:	移动指定源目录及目录下所有文件和子目录到目标目录
	//************************************
	static void MoveDirectory(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    MoveDirectoryFast
	// @Returns:   void
	// @Parameter: sourcePath	源目录
	// @Parameter: destPath	目标目录
	// @Brief:	移动指定源目录及目录下所有文件和子目录到目标目录（目录嵌套较深及文件过多）
	//************************************
	static void MoveDirectoryFast(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    CopyDirectory
	// @Returns:   void
	// @Parameter: sourcePath	源目录
	// @Parameter: destPath	目标目录
	// @Brief:	拷贝指定目录到目标目录
	//************************************
	static void CopyDirectory(std::string sourcePath, std::string destPath);
	//************************************
	// @Method:    SetAccessControl
	// @Returns:   void
	// @Parameter: path
	// @Brief:	设置目录访问权限
	//************************************
	static void SetAccessControl(std::string path);
	static void SetCreateTime(DateTime);
	static void SetLastAccessTime(DateTime);
	static void SetLastWriteTime(DateTime);
	//************************************
	// @Method:    SetCurrentDirectory
	// @Returns:   void
	// @Parameter: path
	// @Brief:	设置目前目录
	//************************************
	static void SetCurrentDirectory(std::string path);
	//************************************
	// @Method:    Search
	// @Returns:   void
	// @Parameter: path 指定目录
	// @Parameter: vec 返回符合的文件列表
	// @Parameter: realSymbol	查找符号
	// @Brief:	查找指定目录之下所有符合条件的文件
	//************************************
	static void Search(std::string path, std::vector<std::string>& vec,std::string realSymbol);
	//************************************
	// @Method:    SetDateTime
	// @Returns:   void
	// @Parameter: 
	// @Parameter: 
	// @Brief:	设置日期的助手函数
	//************************************
	static void SetDateTime(DateTime&, struct tm*);
	//************************************
	// @Method:    DisplayDirectoryInformation
	// @Returns:   void
	// @Parameter: path
	// @Brief:	查看指定目录下总文件大小及子目录信息
	//************************************
	static void DisplayDirectoryInformation(std::string path);
private:
	//************************************
	// @Method:    SearchFile
	// @Returns:   std::vector<std::string>
	// @Parameter: path 指定目录
	// @Parameter: recursive	是否进行递归查找
	// @Brief:	在指定目录下查找指定文件
	//************************************
	static std::vector<std::string>SearchFile(std::string path, bool recursive=false);
	//下面的几个函数当目录嵌套较深，文件较多时比较慢
	//************************************
	// @Method:    GetOldCatalogTree
	// @Returns:   FolderNode 目录节点
	// @Parameter: path 指定目录
	// @Brief:	获取指定目录目录树
	//************************************
	static FolderNode GetOldCatalogTree(std::string path);
	//************************************
	// @Method:    ConstructNewCatalogTree
	// @Returns:   void
	// @Parameter: node	源目录树节点
	// @Parameter: path	目标路径
	// @Parameter: prefix	
	// @Brief:	根据源目录树，构建目标路径下目录树
	//************************************
	static void ConstructNewCatalogTree(FolderNode node, std::string path, std::deque<std::string>& prefix);
	//************************************
	// @Method:    RemoveCatalogTree
	// @Returns:   void
	// @Parameter: node 目录树节点
	// @Brief:	删除指定目录树下所有文件及子目录
	//************************************
	static void RemoveCatalogTree(FolderNode& node);
	//************************************
	// @Method:    GetRootDriveSymbol
	// @Returns:   std::string
	// @Parameter: index
	// @Brief:	获取盘符
	//************************************
	static std::string GetRootDriveSymbol(int index);
	//************************************
	// @Method:    CopyFile
	// @Returns:   void
	// @Parameter: node	源目录树节点
	// @Parameter: sourcePath	源目录
	// @Parameter: destPath	目标目录
	// @Brief:	根据目录树，拷贝整个源目录树文件及子目录至目标目录
	//************************************
	static void CopyFile(FolderNode& node, std::string sourcePath, std::string destPath);

	//快速遍历
	//************************************
	// @Method:    ParseOldCatalogTree
	// @Returns:   void
	// @Parameter: path 源目录
	// @Parameter: treePath	目录树存放位置
	// @Brief:	根据源目录路径解析整个目录树
	//************************************
	static void ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath);
	//************************************
	// @Method:    SplitFilesAndDirectories
	// @Returns:   void
	// @Parameter: originTree	源目录树
	// @Parameter: newPath 目标路径
	// @Parameter: oldPath 源路径
	// @Parameter: splitedOldFiles	原目录所有文件
	// @Parameter: splitedNewDirectories	新目录所有子目录
	// @Parameter: splitedNewFiles	新目录下所有文件
	// @Brief:	根据目录树获取原目录树下所有文件，和所有子目录，以及新目录
	//************************************
	static void ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath, Directory_info& d_info);
	static void SplitFilesAndDirectories(
		std::vector<std::string>& originTree, 
		std::string& newPath, std::string& oldPath,
		std::vector<std::string>&splitedOldFiles, 
		std::vector<std::string>&splitedNewDirectories,
		std::vector<std::string>&splitedNewFiles);
	//************************************
	// @Method:    RebuildCatalogTree
	// @Returns:   void
	// @Parameter: newDirectory
	// @Brief:	根据新目录，构建整个目录树
	//************************************
	static void RebuildCatalogTree(std::vector<std::string>& newDirectory);
	//************************************
	// @Method:    CopyOriginFile
	// @Returns:   void
	// @Parameter: originFiles
	// @Parameter: newFiles
	// @Brief:	拷贝源目录下所有文件到目标目录
	//************************************
	static void CopyOriginFile(std::vector<std::string>& originFiles, std::vector<std::string>& newFiles);
	//************************************
	// @Method:    RemoveOldCatalogTree
	// @Returns:   void
	// @Parameter: originalPathTree
	// @Brief:	删除整个旧目录树
	//************************************
	static void RemoveOldCatalogTree(std::vector<std::string>& originalPathTree);
};
