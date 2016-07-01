#pragma once
#include "ccfilestrategy.h"
#include "platformconfig.h"

#if CCTARGET_PLATFORM == CCPLATFORM_LINUX
class CC_file_linux : public CC_file_strategy
{
public:
	void Create(std::string path)override;
	intptr_t Copy(std::string sourcePath, std::string destPath)override;
	void Delete(std::string path)override;
	bool Exists(std::string path)override;
	bool CanRead(std::string path)override;
	bool CanWrite(std::string path)override;
	bool CanExecute(std::string path)override;
	void Move(std::string sourcePath, std::string destPath)override;
	FileStream* Open(std::string path, FileMode mode)override;
	FileStream* OpenRead(std::string path)override;
	FileStream* OpenWrite(std::string path)override;
	intptr_t ReadAllBytes(std::string path, char destBuffer[], size_t length)override;
	long ReadAllLines(std::string path, std::vector<std::string>& destContainer)override;
	std::string ReadAllText(std::string path)override;
	bool Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath)override;
	intptr_t WriteAllBytes(std::string path, const char sourceData[], size_t len)override;
	long WriteAllLines(std::string path, const std::vector<std::string>& sourceData)override;
	intptr_t WriteAllText(std::string path, std::string& content)override;
private:
	static long CopyFileData(FILE* src, FILE* dest);
};

#endif

