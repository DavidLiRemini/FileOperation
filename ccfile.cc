#include "ccfile.h"
#include <assert.h>


CC_file_strategy* CC_File::instance = nullptr;

CC_File::CC_File()
{
	if (instance == nullptr)
	{
		instance = CC_file_strategy::GetInstance();
	}
	assert(instance != nullptr);
}

void CC_File::Create(std::string path)
{
	instance->Create(path);
}

intptr_t CC_File::Copy(std::string sourcePath, std::string destPath)
{
	return instance->Copy(sourcePath, destPath);
}

void CC_File::Delete(std::string path)
{
	instance->Delete(path);
}

bool CC_File::Exists(std::string path)
{
	return instance->Exists(path);
}

bool CC_File::CanRead(std::string path)
{
	return instance->CanRead(path);
}

bool CC_File::CanExecute(std::string path)
{
	return instance->CanExecute(path);
}

void CC_File::Move(std::string sourcePath, std::string destPath)
{
	instance->Move(sourcePath, destPath);
}

FileStream* CC_File::Open(std::string path, FileMode mode)
{
	return instance->Open(path, mode);
}

FileStream* CC_File::OpenRead(std::string path)
{
	return instance->OpenRead(path);
}

FileStream* CC_File::OpenWrite(std::string path)
{
	return instance->OpenWrite(path);
}

intptr_t CC_File::ReadAllBytes(std::string path, char destBuffer[], size_t length)
{
	return instance->ReadAllBytes(path, destBuffer, length);
}

long CC_File::ReadAllLines(std::string path, std::vector<std::string>& destContainer)
{
	return instance->ReadAllLines(path, destContainer);
}

std::string CC_File::ReadAllText(std::string path)
{
	return instance->ReadAllText(path);
}

bool CC_File::Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath)
{
	return instance->Replace(sourcePath, replaceFilePath, backPath);
}

intptr_t CC_File::WriteAllBytes(std::string path, const char sourceData[], size_t len)
{
	return instance->WriteAllBytes(path, sourceData, len);
}

long CC_File::WriteAllLines(std::string path, const std::vector<std::string>& sourceData)
{
	return instance->WriteAllLines(path, sourceData);
}

intptr_t CC_File::WriteAllText(std::string path, std::string& content)
{
	return instance->WriteAllText(path, content);
}

CC_File::~CC_File()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}