#pragma once
#include "platformconfig.h"

#if CCTARGET_PLATFORM == CCPLATFORM_WIN32
#include "ccdirectorystrategy.h"
#include <errno.h>
#include <io.h>
#include <direct.h>
#include <deque>
#include <stack>

struct FolderNode
{
	std::string name;
	FileAttributes nodeAttr;
	bool hasDirectory = false;
	std::vector<FolderNode>subChildren;
};

class CC_directory_win32 : public CC_directory_strategy
{
public:
	void CreateDirectory(std::string path)override;
	void RemoveDirectory(std::string path)override;
	std::vector<std::string>GetSubDirectoriesCollection(std::string path)override;
	std::vector<std::string> GetFiles(std::string path)override;
	std::vector<std::string> GetFilesAndDirectories(std::string path)override;
	bool Exists(std::string path)override;
	std::string GetCurrentDirectory()override;
	std::string GetDirectoryRoot(std::string path)override;
	std::string GetParentDirectory(std::string path)override;
	void MoveDirectory(std::string sourcePath, std::string destPath)override;
	void CopyDirectory(std::string sourcePath, std::string destPath)override;
	void Search(std::string path, std::vector<std::string>& vec, std::string realSymbol)override;
	void DisplayDirectoryInformation(std::string path)override;

	void GetAccessControl(std::string path);
	DateTime GetCreateTime(std::string path);
	DateTime GetLastAccessTime(std::string path);
	DateTime GetLastWriteTime(std::string path);
	//************************************
	// @Method:    MoveDirectoryFast
	// @Returns:   void
	// @Parameter: sourcePath
	// @Parameter: destPath
	// @Brief:	快速移动文件夹
	//************************************
	void MoveDirectoryFast(std::string sourcePath, std::string destPath);
	void SetAccessControl(std::string path);
	void SetCreateTime(DateTime);
	void SetLastAccessTime(DateTime);
	void SetLastWriteTime(DateTime);
	void SetCurrentDirectory(std::string path);
	void SetDateTime(DateTime&, struct tm*);
private:
	std::vector<std::string>SearchFile(std::string path, bool recursive = false);
	FolderNode GetOldCatalogTree(std::string path);
	void ConstructNewCatalogTree(FolderNode node, std::string path, std::deque<std::string>& prefix);
	void RemoveCatalogTree(FolderNode& node);
	std::string GetRootDriveSymbol(int index);
	void CopyFile(FolderNode& node, std::string sourcePath, std::string destPath);
	void ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath);
	void ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath, DirectoryInfo& d_info);
	void SplitFilesAndDirectories(
		std::vector<std::string>& originTree,
		std::string& newPath, std::string& oldPath,
		std::vector<std::string>&splitedOldFiles,
		std::vector<std::string>&splitedNewDirectories,
		std::vector<std::string>&splitedNewFiles);
	void RebuildCatalogTree(std::vector<std::string>& newDirectory);
	void CopyOriginFile(std::vector<std::string>& originFiles, std::vector<std::string>& newFiles);
	void RemoveOldCatalogTree(std::vector<std::string>& originalPathTree);
};

#endif