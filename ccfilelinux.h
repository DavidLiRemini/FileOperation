#pragma once
#include "ccfilestrategy.h"
#include "platformconfig.h"

#if CCTARGET_PLATFORM == CCPLATFORM_LINUX
class CC_file_linux : public CC_file_strategy
{
public:
	void Create(std::string path)override;
	intptr_t Copy(std::string sourcePath, std::string destPath)override;
	bool Exists(std::string path)override;
	bool CanRead(std::string path)override;
	bool CanWrite(std::string path)override;
	bool CanExecute(std::string path)override;
	
private:
	static long CopyFileData(FILE* src, FILE* dest);
};

#endif

