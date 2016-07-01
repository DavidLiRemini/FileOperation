#ifndef _CC_FILE_STRATEGY_H_
#define _CC_FILE_STRATEGY_H_


#include "common.h"
#include <vector>

class CC_file_strategy
{
public:
	static CC_file_strategy* GetInstance();
protected:
	CC_file_strategy(){}
	static CC_file_strategy* f_instance;
public:
	virtual void Create(std::string path);
	virtual intptr_t Copy(std::string sourcePath, std::string destPath);
	virtual void Delete(std::string path) = 0;
	virtual bool Exists(std::string path) = 0;

	virtual bool CanRead(std::string path) = 0;
	virtual bool CanWrite(std::string path) = 0;
	virtual bool CanExecute(std::string path) = 0;
	virtual void Move(std::string sourcePath, std::string destPath)=0;
	virtual FileStream* Open(std::string path, FileMode mode)=0;
	virtual FileStream* OpenRead(std::string path)=0;
	virtual FileStream* OpenWrite(std::string path)=0;
	virtual intptr_t ReadAllBytes(std::string path, char destBuffer[], size_t length)=0;
	virtual long ReadAllLines(std::string path, std::vector<std::string>& destContainer)=0;
	virtual std::string ReadAllText(std::string path)=0;
	virtual bool Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath)=0;
	virtual intptr_t WriteAllBytes(std::string path, const char sourceData[], size_t len)=0;
	virtual long WriteAllLines(std::string path, const std::vector<std::string>& sourceData)=0;
	virtual intptr_t WriteAllText(std::string path, std::string& content)=0;

};

#endif