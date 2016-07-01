#ifndef _CC_FILE_STRATEGY_H_
#define _CC_FILE_STRATEGY_H_


#include "common.h"
#include <vector>

class CC_file_strategy
{
public:
	static CC_file_strategy* GetInstance();
	virtual ~CC_file_strategy(){}
protected:
	CC_file_strategy(){}
	static CC_file_strategy* f_instance;
public:
	virtual void Create(std::string path);
	virtual intptr_t Copy(std::string sourcePath, std::string destPath);
	virtual void Delete(std::string path);
	virtual bool Exists(std::string path) = 0;

	virtual bool CanRead(std::string path) = 0;
	virtual bool CanWrite(std::string path) = 0;
	virtual bool CanExecute(std::string path) = 0;
	virtual void Move(std::string sourcePath, std::string destPath);
	virtual FileStream* Open(std::string path, FileMode mode);
	virtual FileStream* OpenRead(std::string path);
	virtual FileStream* OpenWrite(std::string path);
	virtual intptr_t ReadAllBytes(std::string path, char destBuffer[], size_t length);
	virtual long ReadAllLines(std::string path, std::vector<std::string>& destContainer);
	virtual std::string ReadAllText(std::string path);
	virtual bool Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath);
	virtual intptr_t WriteAllBytes(std::string path, const char sourceData[], size_t len);
	virtual long WriteAllLines(std::string path, const std::vector<std::string>& sourceData);
	virtual intptr_t WriteAllText(std::string path, std::string& content);

};

#endif