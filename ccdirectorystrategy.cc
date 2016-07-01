#include "ccdirectorystrategy.h"

CC_directory_strategy* CC_directory_strategy::d_instance = nullptr;
std::vector<std::string> CC_directory_strategy::GetFilesAndDirectories(std::string path)
{
	std::vector<std::string>directory = GetSubDirectoriesCollection(path);
	std::vector<std::string>files = GetFiles(path);
	files.insert(files.end(), directory.begin(), directory.end());
	return files;
}

std::string CC_directory_strategy::GetParentDirectory(std::string path)
{
	if (!Exists(path))
	{
		return "";
	}

	size_t index = path.rfind('/');
	std::string parent = path.substr(0, index);
	return parent;
}