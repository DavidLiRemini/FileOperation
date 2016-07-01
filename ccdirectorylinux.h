#pragma once

#include "platformconfig.h"
#if CCTARGET_PLATFORM == CCPLATFORM_LINUX
#include "ccdirectorystrategy.h"

class CC_directory_linux : public CC_directory_strategy
{
public:
	void CreateDirectory(std::string path)override;
	void RemoveDirectory(std::string path)override;
	std::vector<std::string> GetSubDirectoriesCollection(std::string path)override;
	std::vector<std::string> GetFiles(std::string path)override;
	std::vector<std::string> GetFilesAndDirectories(std::string path)override;
	bool Exists(std::string path)override;
	std::string GetCurrentDirectory()override;
	void DisplayDirectoryInformation(std::string path)override;
	std::string GetParentDirectory(std::string path)override;
	std::string GetDirectoryRoot(std::string path)override;
	void MoveDirectory(std::string sourcePath, std::string destPath)override;
	void CopyDirectory(std::string sourcePath, std::string destPath)override;
	void Search(std::string path, std::vector<std::string>& vec, std::string realSymbol)override;
	void test(std::string path);
	
private:
	void ParseDirectoryTree(std::string& path, std::vector<std::string>& treePath);
	void ParseOriginDirectoryTree(std::string& path, DirectoryInfo&);
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
