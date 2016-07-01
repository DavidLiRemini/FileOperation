#ifndef _CC_DIRECTORY_STRATEGY_H_
#define _CC_DIRECTORY_STRATEGY_H_

#include "common.h"
#include <vector>

class CC_directory_strategy
{
public:
	static CC_directory_strategy* GetInstance();
protected:
	CC_directory_strategy(){}
	static CC_directory_strategy* d_instance;
public:
	virtual void CreateDirectory(std::string path) = 0;
	virtual void RemoveDirectory(std::string path) = 0;
	virtual std::vector<std::string>GetSubDirectoriesCollection(std::string path) = 0;
	virtual std::vector<std::string> GetFiles(std::string path) = 0;
	virtual std::vector<std::string> GetFilesAndDirectories(std::string path) = 0;
	virtual bool Exists(std::string path) = 0;
	virtual std::string GetCurrentDirectory() = 0;
	virtual std::string GetDirectoryRoot(std::string path) = 0;
	virtual std::string GetParentDirectory(std::string path) = 0;
	virtual void MoveDirectory(std::string sourcePath, std::string destPath) = 0;
	virtual void CopyDirectory(std::string sourcePath, std::string destPath) = 0;
	virtual void Search(std::string path, std::vector<std::string>& vec, std::string realSymbol) = 0;
	virtual void DisplayDirectoryInformation(std::string path) = 0;
};

#endif