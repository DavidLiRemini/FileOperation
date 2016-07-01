#include "ccdirectory.h"
#include <assert.h>

CC_directory_strategy* CC_Directory::instance = nullptr;

CC_Directory::CC_Directory()
{
	if (instance == nullptr)
	{
		instance = CC_directory_strategy::GetInstance();
	}
	assert(instance != nullptr);
}

CC_Directory::~CC_Directory()
{
	if (instance == nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void CC_Directory::CreateDirectory(std::string path)
{
	instance->CreateDirectory(path);
}

void CC_Directory::RemoveDirectory(std::string path)
{
	instance->RemoveDirectory(path);
}

std::vector<std::string>CC_Directory::GetSubDirectoryCollection(std::string path)
{
	return instance->GetSubDirectoriesCollection(path);
}

std::vector<std::string>CC_Directory::GetFiles(std::string path)
{
	return instance->GetFiles(path);
}

std::vector<std::string>CC_Directory::GetFilesAndDirectories(std::string path)
{
	return instance->GetFilesAndDirectories(path);
}

bool CC_Directory::Exists(std::string path)
{
	return instance->Exists(path);
}

std::string CC_Directory::GetCurrentDirectory()
{
	return instance->GetCurrentDirectory();
}

std::string CC_Directory::GetDirectoryRoot(std::string path)
{
	return instance->GetDirectoryRoot(path);
}

std::string CC_Directory::GetParentDirectory(std::string path)
{
	return instance->GetParentDirectory(path);
}

void CC_Directory::MoveDirectory(std::string sourcePath, std::string destPath)
{
	instance->MoveDirectory(sourcePath, destPath);
}

void CC_Directory::CopyDirectory(std::string sourcePath, std::string destPath)
{
	instance->CopyDirectory(sourcePath, destPath);
}

void CC_Directory::Search(std::string path, std::vector<std::string>& vec, std::string realSymbol)
{
	instance->Search(path, vec, realSymbol);
}

void CC_Directory::DisplayDirectoryInformation(std::string path)
{
	instance->DisplayDirectoryInformation(path);
}