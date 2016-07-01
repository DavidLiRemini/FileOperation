#include "platformconfig.h"
#if CCTARGET_PLATFORM == CCPLATFORM_LINUX
#include "ccfilelinux.h"
#include "ccdirectorylinux.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <algorithm>

CC_directory_strategy* CC_directory_strategy::GetInstance()
{
	if (d_instance == nullptr)
	{
		d_instance = new CC_directory_linux();
	}
	return d_instance;
}

bool CC_directory_linux::Exists(std::string path)
{
	if (path.back() != '/')
		path += "/";
	int stat = access(path.c_str(), F_OK);
	if (stat)
	{
		return false;
	}
	return true;
}

void CC_directory_linux::CreateDirectory(std::string path)
{
	if (path.back() != '/')
	{
		path += "/";
	}
	if (Exists(path))
		return;
	int stat = mkdir(path.c_str(), 0755);
	if (stat != 0)
	{
		int err = errno;
		if (err == EACCES)
		{
			fprintf(stderr, "%s, %d ->mkdir %s failed code %s\n", __FUNCTION__, __LINE__, path.c_str(), strerror(err));
			return;
		}
	}
}

void CC_directory_linux::RemoveDirectory(std::string path)
{
	if (path.back() != '/')
		path += "/";
	if (!Exists(path))
	{
		fprintf(stderr, "No such a directory\n");
		return;
	}
	int stat = rmdir(path.c_str());
	if (stat != 0)
	{
		int err = errno;
		fprintf(stderr, "%s, %d ->rmdir %s failed error Code: %s\n", __FUNCTION__, __LINE__, path.c_str(), strerror(err));
		return;
	}
}

std::vector<std::string> CC_directory_linux::GetSubDirectoriesCollection(std::string path)
{
	if (path.back() != '/')
		path += "/";
	if (!Exists(path))
		return std::vector<std::string>();
	std::vector<std::string>resultSet;
	DIR* d = nullptr;
	struct dirent* handle = nullptr;
	struct stat stab;
	bzero(&stab, sizeof(struct stat));
	if (!(d = opendir(path.c_str())))
	{
		fprintf(stderr, "%s, %d ->Open directory %s failed error code: %s\n", __FUNCTION__, __LINE__, path.c_str(), strerror(errno));
		return resultSet;
	}
	while((handle = readdir(d)) != nullptr)
	{
		std::string name = "";
		if(handle != nullptr)
			name = handle->d_name;
		if (name == "." || name == "..")
			continue;
		std::string pathName = path + name;
		int state = stat(pathName.c_str(), &stab);
		if (state >= 0 && S_ISDIR(stab.st_mode))
		{
			resultSet.push_back(pathName + "/");
		}
	}
	return resultSet;
}

std::vector<std::string> CC_directory_linux::GetFiles(std::string path)
{
	if (path.back() != '/')
		path += "/";
	if (!Exists(path))
		return std::vector<std::string>();
	std::vector<std::string>resultSet;
	DIR* d = nullptr;
	struct dirent* handle = nullptr;
	struct stat stab;
	bzero(&stab, sizeof(struct stat));
	if (!(d = opendir(path.c_str())))
	{
		fprintf(stderr, "%s, %d ->Open directory %s failed error code: %s\n", __FUNCTION__, __LINE__, path.c_str(), strerror(errno));
		return resultSet;
	}
	while((handle = readdir(d)) != nullptr)
	{
		std::string name = "";
		if(handle != nullptr)
			name = handle->d_name;
		if (name == "." || name == "..")
			continue;
		std::string pathName = path + name;
		int state = stat(pathName.c_str(), &stab);
		if (state >= 0 && S_ISREG(stab.st_mode))
		{
			resultSet.push_back(pathName);
		}
	}
	return resultSet;
}


std::string CC_directory_linux::GetCurrentDirectory()
{
	char buf[150] = { 0 };
	char* state = getcwd(buf, sizeof(buf));
	if (state == nullptr)
	{
		fprintf(stderr, "%s, %d ->GetCurrentdirctory error, code %s\n", __FUNCTION__, __LINE__, strerror(errno));
		return "";
	}
	return std::string(buf);
}

void CC_directory_linux::DisplayDirectoryInformation(std::string path)
{
	if (path.back() != '/')
		path += "/";
	DirectoryInfo info;
	ParseOriginDirectoryTree(path, info);
	double sz = info.totalDirectorySize;
	if (sz >= 1024 * 1024 * 1024)
	{
		printf("Directory Total size %.3fG\n", sz / 1024 / 1024 / 1024);
	}
	else if (sz >= 1024 * 1024)
	{
		printf("Directory Total size %.3fM\n", sz / 1024 / 1024);
	}
	else
		printf("Directory Total size %.3fK\n", sz / 1024);
	printf("Directory total folders: %lu\n", info.totalDirectoryNumber);
	printf("Directory total files: %lu\n", info.totalFileNumber);
}

void CC_directory_linux::ParseOriginDirectoryTree(std::string& path, DirectoryInfo& info)
{
	DIR *d = nullptr;
	struct dirent *file = nullptr;
	struct stat sb;
	bzero(&sb, sizeof(struct stat));
	if (!(d = opendir(path.c_str())))
	{
		printf("%s, %d -> Opendir %s failed, errorcode: %s\n", __FUNCTION__, __LINE__, path.c_str(), strerror(errno));
		return;
	}
	while ((file = readdir(d)) != NULL)
	{
		if (strncmp(file->d_name, ".", 1) == 0)
			continue;
		std::string pa = path + "/" + file->d_name;
		int state = stat(pa.c_str(), &sb);
		int err = errno;
		if (state >= 0 && S_ISDIR(sb.st_mode))
		{
			++info.totalDirectoryNumber;
			ParseOriginDirectoryTree(pa, info);
		}
		else if (state >= 0 && S_ISREG(sb.st_mode))
		{
			info.totalDirectorySize += sb.st_size;
			++info.totalFileNumber;
		}
	}
	closedir(d);
}

void CC_directory_linux::ParseDirectoryTree(std::string& path, std::vector<std::string>& treePath)
{
	assert(path.back() == '/');
	DIR *d = nullptr;
	struct dirent *handle = nullptr;
	struct stat stab;
	bzero(&stab, sizeof(struct stat));
	if (!(d = opendir(path.c_str())))
	{
		printf("%s, %d -> Opendir %s failed, errorcode: %s", __FUNCTION__, __LINE__, path.c_str(), strerror(errno));
		return;
	}
	while ((handle = readdir(d)) != nullptr)
	{
		std::string name = handle->d_name;
		if (name == "." || name == "..")
			continue;
		std::string pa = path + name;
		int state = stat(pa.c_str(), &stab);
		int err = errno;
		if (state >= 0 && S_ISDIR(stab.st_mode))
		{
			std::string s = pa + "/";
			treePath.push_back(s);
			ParseDirectoryTree(s, treePath);
		}
		else if (state >= 0 && S_ISREG(stab.st_mode))
		{
			treePath.push_back(pa);
		}
	}
	closedir(d);
}

std::string CC_directory_linux::GetDirectoryRoot(std::string path)
{
	assert(path.length() > 0);
	auto index = path.find_first_of('/', 1);
	if (index != std::string::npos)
	{
		return path.substr(0, index);
	}
	return "";
}

void CC_directory_linux::test(std::string path)
{
	if (path.back() != '/')
		path += "/";
	std::vector<std::string> originTree;
	ParseDirectoryTree(path, originTree);
	std::sort(originTree.begin(), originTree.end(), [](const std::string& lhs, const std::string& rhs){
		return strcmp(lhs.c_str(), rhs.c_str()) < 0;
	});
	for (auto it : originTree)
	{
		printf("->%s\n", it.c_str());
	}
}

void CC_directory_linux::SplitFilesAndDirectories(std::vector<std::string>&originTree,
	std::string& newPath, std::string& oldPath,
	std::vector<std::string>&splitedOldFiles,
	std::vector<std::string>&splitedNewDirectories,
	std::vector<std::string>&splitedNewFiles)
{
	assert(originTree.size() > 0);
	std::sort(originTree.begin(), originTree.end(), [](const std::string& lhs, const std::string& rhs){
		return strcmp(lhs.c_str(), rhs.c_str()) < 0;
	});
	for (auto& item : originTree)
	{
		if (item.back() != '/')
		{
			splitedOldFiles.push_back(item);
		}
		item.replace(0, oldPath.length(), newPath);
		if (item.back() == '/')
		{
			splitedNewDirectories.push_back(item);
		}
		else
			splitedNewFiles.push_back(item);
	}
}

void CC_directory_linux::RebuildCatalogTree(std::vector<std::string>& newDirectory)
{
	for (auto& it : newDirectory)
	{
		if (!CC_directory_linux::Exists(it))
		{
			CC_directory_linux::CreateDirectory(it);
		}
	}
}

void CC_directory_linux::CopyOriginFile(std::vector<std::string>& originFiles, std::vector<std::string>& newFiles)
{
	CC_file_linux file;
	assert(originFiles.size() == newFiles.size());
	for (size_t i = 0; i < originFiles.size(); i++)
	{
		file.Copy(originFiles.at(i), newFiles.at(i));
	}
}
void CC_directory_linux::RemoveOldCatalogTree(std::vector<std::string>& originalPathTree)
{
	CC_file_linux file;
	assert(originalPathTree.size() > 0);
	std::sort(originalPathTree.begin(), originalPathTree.end(), [](const std::string&lhs, const std::string& rhs){
		return strcmp(lhs.c_str(), rhs.c_str()) < 0;
	});
	for (auto iter = originalPathTree.rbegin(); iter != originalPathTree.rend(); iter++)
	{
		std::string s = *iter;
		if ((*iter).back() == '/')
		{
			(*iter).erase(iter->end() - 1);
			CC_directory_linux::RemoveDirectory(*iter);
		}
		else
		{
			file.Delete(*iter);
		}
	}
}

void CC_directory_linux::CopyDirectory(std::string sourcePath, std::string destPath)
{
	if (sourcePath.back() != '/')
	{
		sourcePath += '/';
	}
	auto index = std::string(sourcePath.begin(), sourcePath.end() - 1).rfind('/');
	std::string folderName = sourcePath.substr(index + 1, sourcePath.length() - index - 2);
	if (destPath.back() != '/')
	{
		destPath += "/";
	}
	CC_directory_linux::CreateDirectory(destPath + folderName);
	destPath = destPath + folderName + "/";
	std::vector<std::string>oldPathTree;
	std::vector<std::string>oldFileCollection;
	std::vector<std::string>newDirectoryCollection;
	std::vector<std::string>newFileCollection;
	ParseDirectoryTree(sourcePath, oldPathTree);
	SplitFilesAndDirectories(oldPathTree, destPath,
		sourcePath, oldFileCollection,
		newDirectoryCollection, newFileCollection);
	assert(!newFileCollection.empty());
	RebuildCatalogTree(newDirectoryCollection);
	CopyOriginFile(oldFileCollection, newFileCollection);
}


void CC_directory_linux::Search(std::string path, std::vector<std::string>& vec, std::string realSymbol)
{
	if (path.back() != '/')
	{
		path += "/";
	}

	DIR *d = nullptr;
	struct dirent *handle = nullptr;
	struct stat stab;
	bzero(&stab, sizeof(struct stat));
	if (!(d = opendir(path.c_str())))
	{
		printf("%s, %d -> Opendir %s failed, errorcode: %s", __FUNCTION__, __LINE__, path.c_str(), strerror(errno));
		return;
	}
	while ((handle = readdir(d)) != nullptr)
	{
		std::string name = handle->d_name;
		if (name == "." || name == "..")
			continue;
		std::string pa = path + name;
		int state = stat(pa.c_str(), &stab);
		int err = errno;
		if (state >= 0 && S_ISDIR(stab.st_mode))
		{
			std::string s = pa + "/";
			Search(s, vec, realSymbol);
		}
		else if (state >= 0 && S_ISREG(stab.st_mode))
		{
			if(name == realSymbol)
				vec.push_back(pa);
		}
	}
	closedir(d);

}

void CC_directory_linux::MoveDirectory(std::string sourcePath, std::string destPath)
{
	if (sourcePath.back() != '/')
	{
		sourcePath += '/';
	}
	auto index = std::string(sourcePath.begin(), sourcePath.end() - 1).rfind('/');
	std::string folderName = sourcePath.substr(index + 1, sourcePath.length() - index - 2);
	if (destPath.back() != '/')
	{
		destPath += "/";
	}
	CreateDirectory(destPath + folderName);
	destPath = destPath + folderName + "/";
	std::vector<std::string>oldPathTree;
	std::vector<std::string>oldFileCollection;
	std::vector<std::string>oldPathTreeBak;
	std::vector<std::string>newDirectoryCollection;
	std::vector<std::string>newFileCollection;
	ParseDirectoryTree(sourcePath, oldPathTree);
	std::sort(oldPathTree.begin(), oldPathTree.end(), [](const std::string&lhs, const std::string& rhs){
		return strcmp(lhs.c_str(), rhs.c_str()) < 0;
	});
	oldPathTreeBak = oldPathTree;
	SplitFilesAndDirectories(oldPathTree, destPath,
		sourcePath, oldFileCollection,
		newDirectoryCollection, newFileCollection);
	assert(!newFileCollection.empty());
	RebuildCatalogTree(newDirectoryCollection);
	CopyOriginFile(oldFileCollection, newFileCollection);
	RemoveOldCatalogTree(oldPathTreeBak);
	RemoveDirectory(sourcePath);
}

#endif
