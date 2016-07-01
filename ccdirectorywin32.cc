#include "platformconfig.h"

#if CCTARGET_PLATFORM == CCPLATFORM_WIN32
#include "ccdirectorywin32.h"
#include "ccfilewin32.h"
#include <time.h>
#include <assert.h>
#include <algorithm>

CC_directory_strategy* CC_directory_strategy::GetInstance()
{
	if (d_instance == nullptr)
	{
		d_instance = new CC_directory_win32();
	}
	return d_instance;
}

static inline std::string ConvertFormatToUnixStyle(const std::string& path)
{
	std::string ret = path;
	int len = ret.length();
	for (int i = 0; i < len; ++i)
	{
		if (ret[i] == '\\')
		{
			ret[i] = '/';
		}
	}
	return ret;
}
static inline bool CheckNonUnixStyle(const std::string& path)
{
	auto index = path.find_first_of('\\');
	if (index == std::string::npos)
	{
		return false;
	}
	return true;
}

void CC_directory_win32::CreateDirectory(std::string path)
{
	if (CheckNonUnixStyle(path))
	{
		path = ConvertFormatToUnixStyle(path);
	}
	static int count = 1;
	int stat = _mkdir(path.c_str());
	//printf("Create directory %d\n", count++);
	if (stat == -1)
	{
		int err = errno;
		if (err == EEXIST)
		{
			fprintf(stderr, "%s already exists %s %d\n", path.c_str(), __FUNCTION__, __LINE__);
			return;
		}
		else if (err == ENOENT)
		{
			fprintf(stderr, "%s is an illegal path %s %d\n", path.c_str(), __FUNCTION__, __LINE__);
			return;
		}
	}
}

void CC_directory_win32::RemoveDirectory(std::string path)
{
	if (!Exists(path))
	{
		return;
	}
	int deleResult = _rmdir(path.c_str());
	if (deleResult == -1)
	{
		int err = errno;
		if (err == ENOTEMPTY)
		{
			fprintf(stderr, "Delete %s failed. Given path is not a directory or directory is not empty %s %d\n",
				path.c_str(), __FUNCTION__, __LINE__);
			return;
		}
	}
}

std::vector<std::string> CC_directory_win32::GetSubDirectoriesCollection(std::string path)
{
	if (!Exists(path))
	{
		return std::vector<std::string>();
	}

	std::vector<std::string>directory;
	std::string searchPattern = path + "*.*";
	_finddata_t info;
	intptr_t handle = _findfirst(searchPattern.c_str(), &info);
	if (handle == -1)
	{
		fprintf(stderr, "No subdirectory %s %d\n", __FUNCTION__, __LINE__);
		return std::vector<std::string>();
	}

	do
	{
		if (info.attrib != FileAttributes::DIRECTORY)
		{
			continue;
		}
		directory.push_back(path + info.name);
	} while (!_findnext(handle, &info));
	_findclose(handle);
	return directory;
}

std::vector<std::string>CC_directory_win32::GetFiles(std::string path)
{
	if (path.back() != '/')
	{
		path += "/";
	}
	if (!Exists(path))
	{
		return std::vector<std::string>();
	}

	std::vector<std::string>files;
	std::string searchPattern = path + "*.*";
	_finddata_t fileInfo;
	intptr_t handle = _findfirst(searchPattern.c_str(), &fileInfo);
	if (handle == -1)
	{
		fprintf(stderr, "No files %s %d\n", __FUNCTION__, __LINE__);
		return files;
	}

	do
	{
		if (fileInfo.attrib == FileAttributes::DIRECTORY)
		{
			continue;
		}
		files.push_back(path + fileInfo.name);
	} while (!_findnext(handle, &fileInfo));
	_findclose(handle);
	return files;
}

std::vector<std::string> CC_directory_win32::GetFilesAndDirectories(std::string path)
{
	std::vector<std::string>directory = GetSubDirectoriesCollection(path);
	std::vector<std::string>files = GetFiles(path);
	files.insert(files.end(), directory.begin(), directory.end());
	return files;
}

bool CC_directory_win32::Exists(std::string path)
{
	int stat = _access(path.c_str(), 0);
	if (stat != 0)
	{
		int err = errno;
		if (err == ENOENT)
		{
			fprintf(stderr, "No such directory %s %d\n", __FUNCTION__, __LINE__);
			return false;
		}
		else
		{
			fprintf(stderr, "Invalid arguments %s %d\n", __FUNCTION__, __LINE__);
			return false;
		}
	}
	return true;
}

void CC_directory_win32::GetAccessControl(std::string path)
{

}

DateTime CC_directory_win32::GetCreateTime(std::string path)
{
	if (!Exists(path))
	{
		return DateTime();
	}
	_finddata_t info;
	intptr_t handle = _findfirst(path.c_str(), &info);
	if (handle == -1)
	{
		fprintf(stderr, "No Such directory\n");
		return DateTime();
	}
	DateTime dt;
	time_t value = info.time_create;
	struct tm* tim = localtime(&value);
	SetDateTime(dt, tim);
	_findclose(handle);
	return dt;
}

DateTime CC_directory_win32::GetLastAccessTime(std::string path)
{
	if (!Exists(path))
	{
		return DateTime();
	}
	_finddata_t info;
	intptr_t handle = _findfirst(path.c_str(), &info);
	if (handle == -1)
	{
		fprintf(stderr, "No Such directory\n");
		return DateTime();
	}
	DateTime dt;
	time_t value = info.time_access;
	struct tm* tim = localtime(&value);
	SetDateTime(dt, tim);
	_findclose(handle);
	return dt;
}

void CC_directory_win32::SetDateTime(DateTime& dt, struct tm* tim)
{
	dt.year = tim->tm_yday + 1900;
	dt.month = tim->tm_mon + 1;
	dt.day = tim->tm_mday;
	dt.hour = tim->tm_hour;
	dt.minute = tim->tm_min;
	dt.sec = tim->tm_sec;
}

DateTime CC_directory_win32::GetLastWriteTime(std::string path)
{
	if (!Exists(path))
	{
		return DateTime();
	}
	_finddata_t info;
	intptr_t handle = _findfirst(path.c_str(), &info);
	if (handle == -1)
	{
		fprintf(stderr, "No Such directory\n");
		return DateTime();
	}
	DateTime dt;
	time_t value = info.time_write;
	struct tm* tim = localtime(&value);
	SetDateTime(dt, tim);
	_findclose(handle);
	return dt;
}

std::string CC_directory_win32::GetCurrentDirectory()
{
	char buf[100] = { 0 };
	char* str = _getcwd(buf, sizeof(buf));
	return str;
}

std::string CC_directory_win32::GetDirectoryRoot(std::string path)
{
	if (!Exists(path))
	{
		return "";
	}
	int currentDrive = _getdrive();
	int c = currentDrive + 'A' - 1;
	return GetRootDriveSymbol(c);
	//未完待续。。。
}

std::string CC_directory_win32::GetRootDriveSymbol(int index)
{
	static std::string symbolMap[100] = {
		"", "", "", "", "", "", "", "", "", "",
		"", "", "", "", "", "", "", "", "", "",
		"", "", "", "", "", "", "", "", "", "",
		"", "", "", "", "", "", "", "", "", "",
		"", "", "", "", "", "", "", "", "", "",
		"", "", "", "", "", "", "", "", "", "",
		"", "", "", "", "", R"(A:\)", R"(B:\)", R"(C:\)", R"(D:\)", R"(E:\)",
		R"(F:\)", R"(G:\)", R"(H:\)", R"(I:\)", R"(J:\)", R"(K:\)", R"(L:\)", R"(M:\)", R"(N:\)", R"(O:\)",
		R"(P:\)", R"(Q:\)", R"(R:\)", R"(S:\)", R"(T:\)", R"(U:\)", R"(V:\)", R"(W:\)", R"(X:\)", R"(Y:\)",
		R"(Z:\)", "", "", "", "", "", "", "", "", ""
	};
	return symbolMap[index];
}

std::string CC_directory_win32::GetParentDirectory(std::string path)
{
	if (!Exists(path))
	{
		return "";
	}

	size_t index = path.rfind('/');
	std::string parent = path.substr(0, index);
	return parent;
}

void CC_directory_win32::MoveDirectory(std::string sourcePath, std::string destPath)
{
	/*if (destPath.back() != '/')
	{
		destPath += "/";
	}
	if (sourcePath.back() != '/')
	{
		sourcePath += "/";
	}
	FolderNode catalogTreeNode = GetOldCatalogTree(sourcePath);
	std::deque<std::string>destPathDeque;
	destPathDeque.push_back(destPath);
	ConstructNewCatalogTree(catalogTreeNode, sourcePath, destPathDeque);
	CopyFile(catalogTreeNode, sourcePath, destPath);
	RemoveCatalogTree(catalogTreeNode);*/
	MoveDirectoryFast(sourcePath, destPath);

}

void CC_directory_win32::MoveDirectoryFast(std::string sourcePath, std::string destPath)
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
	CC_directory_win32::CreateDirectory(destPath + folderName);
	destPath = destPath + folderName + "/";
	std::vector<std::string>oldPathTree;
	std::vector<std::string>oldFileCollection;
	std::vector<std::string>oldPathTreeBak;
	std::vector<std::string>newDirectoryCollection;
	std::vector<std::string>newFileCollection;
	ParseOldCatalogTree(sourcePath, oldPathTree);
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
	CC_directory_win32::RemoveDirectory(sourcePath);
}

void CC_directory_win32::CopyDirectory(std::string sourcePath, std::string destPath)
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
	CC_directory_win32::CreateDirectory(destPath + folderName);
	destPath = destPath + folderName + "/";
	std::vector<std::string>oldPathTree;
	std::vector<std::string>oldFileCollection;
	std::vector<std::string>newDirectoryCollection;
	std::vector<std::string>newFileCollection;
	ParseOldCatalogTree(sourcePath, oldPathTree);
	SplitFilesAndDirectories(oldPathTree, destPath,
		sourcePath, oldFileCollection,
		newDirectoryCollection, newFileCollection);
	assert(!newFileCollection.empty());
	RebuildCatalogTree(newDirectoryCollection);
	CopyOriginFile(oldFileCollection, newFileCollection);
}

void CC_directory_win32::CopyFile(FolderNode& node, std::string sourcePath, std::string destPath)
{
	CC_file_win32 file;
	for (auto item : node.subChildren)
	{
		std::string str = item.name.substr(sourcePath.size());
		std::string dest = destPath + str;
		if (item.nodeAttr == FileAttributes::NORMAL)
		{
			file.Copy(item.name, dest);
		}
		else
			CopyFile(item, sourcePath, destPath);
	}
}

void CC_directory_win32::SetCurrentDirectory(std::string path)
{
	/**
	*
	* ///////
	*/
}

void CC_directory_win32::SetCreateTime(DateTime tim)
{
	/**
	*
	* @Brief:
	*/
}
void CC_directory_win32::SetLastAccessTime(DateTime tm)
{
	/**
	*
	* @Brief:
	*/
}
void CC_directory_win32::SetLastWriteTime(DateTime tm)
{
	/**
	*
	* @Brief:
	*/
}

FolderNode CC_directory_win32::GetOldCatalogTree(std::string path)
{
	if (path.back() != '/')
	{
		path += "/";
	}
	FolderNode root;
	root.name = path;
	root.nodeAttr = FileAttributes::DIRECTORY;
	root.hasDirectory = true;
	std::string searchPath = path + "*.*";
	_finddata_t info;
	intptr_t handle = _findfirst(searchPath.c_str(), &info);
	if (handle == -1)
	{
		//
		int err = errno;
		if (err == ENOENT)
		{
			fprintf(stderr, "No Matchings\n");
			return FolderNode();
		}
	}
	do
	{
		FolderNode nd;
		FileAttributes attr = (FileAttributes)info.attrib;
		nd.name = info.name;
		if ((attr &FileAttributes::DIRECTORY) && nd.name != "." && nd.name != "..")
		{
			nd.nodeAttr = FileAttributes::DIRECTORY;
			std::string s = path + nd.name + "/";
			root.subChildren.push_back(GetOldCatalogTree(s));
		}
		else if (nd.name != "." && nd.name != "..")
		{
			nd.nodeAttr = FileAttributes::NORMAL;
			std::string t = path + nd.name;
			nd.name = t;
			root.subChildren.push_back(nd);
		}
	} while (!_findnext(handle, &info));
	_findclose(handle);
	return root;
}


void CC_directory_win32::ConstructNewCatalogTree(FolderNode root, std::string oldpath, std::deque<std::string>& prefix)
{
	if (root.hasDirectory)
	{
		for (auto it : root.subChildren)
		{
			if (it.nodeAttr == FileAttributes::DIRECTORY)
			{
				std::string str = it.name.substr(oldpath.size());
				std::string newPath = prefix.front() + str;
				if (!Exists(newPath))
				{
					CreateDirectory(newPath);
					prefix.push_back(str);
					ConstructNewCatalogTree(it, oldpath, prefix);
				}
			}
		}
	}
	assert(!prefix.empty());
	prefix.pop_back();
}

void CC_directory_win32::RemoveCatalogTree(FolderNode& node)
{
	CC_file_win32 file;
	if (node.hasDirectory)
	{
		for (auto it : node.subChildren)
		{
			if (it.nodeAttr == FileAttributes::DIRECTORY)
			{
				RemoveCatalogTree(it);
			}
		}
	}
	for (auto it : node.subChildren)
	{
		if (it.nodeAttr == FileAttributes::NORMAL)
		{
			file.Delete(it.name);
		}
	}

	CC_directory_win32::RemoveDirectory(node.name);
}

void CC_directory_win32::Search(std::string path, std::vector<std::string>& vec, std::string searchSymbol)
{
	if (path.back() != '/')
	{
		path += "/";
	}
	std::string searchPattern = path + "*.*";
	_finddata_t info;
	intptr_t handle = _findfirst(searchPattern.c_str(), &info);
	if (handle == -1)
	{
		//something
		int err = errno;
		if (err == ENOENT)
		{
			fprintf(stderr, "No Matchings\n");
			return;
		}
	}

	do
	{
		std::string name = info.name;
		if (info.attrib != FileAttributes::DIRECTORY)
		{
			if (name == searchSymbol)
			{
				vec.push_back(path + name);
			}
			else
				continue;
		}
		else if (name != "." && name != "..")
		{
			Search(path + name, vec, searchSymbol);
		}
	} while (!_findnext(handle, &info));
	_findclose(handle);
}

std::vector<std::string> CC_directory_win32::SearchFile(std::string path, bool recursive/* =false */)
{
	std::vector<std::string>matchFiles;
	auto index = path.rfind('/');
	std::string fileName = path.substr(index + 1);
	std::string filePath = std::string(path.begin(), path.begin() + index);
	if (filePath.back() != '/')
	{
		filePath += "/";
	}
	if (recursive)
	{
		Search(filePath, matchFiles, fileName);
	}
	else
	{
		std::string matchPattern = filePath + "*.*";
		_finddata_t info;
		intptr_t handle = _findfirst(matchPattern.c_str(), &info);
		if (handle == -1)
		{
			//
			int err = errno;
			if (err == ENOENT)
			{
				fprintf(stderr, "No Matchings\n");
				return matchFiles;
			}

		}
		do
		{
			std::string name = info.name;
			if (info.attrib != FileAttributes::DIRECTORY)
			{
				if (name == fileName)
				{
					matchFiles.push_back(path + name);
				}
				else
					continue;
			}
		} while (!_findnext(handle, &info));
		_findclose(handle);
	}
	return matchFiles;
}

void CC_directory_win32::ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath)
{
	std::string searchPath = path + "*.*";
	_finddata_t info;
	intptr_t handle = _findfirst(searchPath.c_str(), &info);
	if (handle == -1)
	{
		//
	}
	do
	{
		std::string nd = info.name;
		FileAttributes attr = (FileAttributes)info.attrib;
		if ((attr & FileAttributes::DIRECTORY) && nd != "." && nd != "..")
		{
			std::string s = path + nd + "/";
			treePath.push_back(s);
			ParseOldCatalogTree(s, treePath);
		}
		else if (nd != "." && nd != "..")
		{
			std::string t = path + nd;
			treePath.push_back(t);
		}
	} while (!_findnext(handle, &info));
	_findclose(handle);
}

void CC_directory_win32::ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath, DirectoryInfo& d_info)
{
	std::string searchPath = path + "*.*";
	_finddata_t info;
	intptr_t handle = _findfirst(searchPath.c_str(), &info);
	if (handle == -1)
	{
		//
	}
	do
	{
		std::string nd = info.name;
		FileAttributes attr = (FileAttributes)info.attrib;
		if ((attr & FileAttributes::DIRECTORY) && nd != "." && nd != "..")
		{
			std::string s = path + nd + "/";
			++d_info.totalDirectoryNumber;
			treePath.push_back(s);
			ParseOldCatalogTree(s, treePath, d_info);
		}
		else if (nd != "." && nd != "..")
		{
			std::string t = path + nd;
			++d_info.totalFileNumber;
			d_info.totalDirectorySize += info.size;
			treePath.push_back(t);
		}
	} while (!_findnext(handle, &info));
	_findclose(handle);
}

void CC_directory_win32::DisplayDirectoryInformation(std::string path)
{
	if (path.back() != '/')
	{
		path += "/";
	}
	std::vector<std::string>oldPath;
	DirectoryInfo info;
	CC_directory_win32::ParseOldCatalogTree(path, oldPath, info);
	if (info.totalDirectorySize >= 1024 * 1024 * 1024)
	{
		printf("Directory's total file size: %4.3gG\n", info.totalDirectorySize / (1024 * 1024 * 1024));
	}
	else if (info.totalDirectorySize >= 1024 * 1024)
	{
		printf("Directory's total file size: %4.3gM\n", info.totalDirectorySize/ (1024 * 1024));
	}
	else
		printf("Directory's total file size: %4.3gK\n", info.totalDirectorySize / 1024);
	printf("Total number of files: %lu\n", info.totalFileNumber);
	printf("Total number of subdirectory: %lu\n", info.totalDirectoryNumber);
}

void CC_directory_win32::SplitFilesAndDirectories(
	std::vector<std::string>& originTree,
	std::string& newPath,
	std::string& oldPath,
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

void CC_directory_win32::RebuildCatalogTree(std::vector<std::string>& newDirectory)
{
	for (auto& it : newDirectory)
	{
		if (!CC_directory_win32::Exists(it))
		{
			CC_directory_win32::CreateDirectory(it);
		}
	}
}

void CC_directory_win32::CopyOriginFile(std::vector<std::string>& originFiles, std::vector<std::string>& newFiles)
{
	/**
	*
	* 新旧目录文件数目应该一样
	*/
	assert(originFiles.size() == newFiles.size());
	CC_file_win32 file;
	for (size_t i = 0; i < originFiles.size(); i++)
	{
		file.Copy(originFiles.at(i), newFiles.at(i));
	}
}

void CC_directory_win32::RemoveOldCatalogTree(std::vector<std::string>& originalPathTree)
{
	assert(originalPathTree.size() > 0);
	CC_file_win32 file;
	std::sort(originalPathTree.begin(), originalPathTree.end(), [](const std::string&lhs, const std::string& rhs){
		return strcmp(lhs.c_str(), rhs.c_str()) < 0;
	});
	for (auto iter = originalPathTree.rbegin(); iter != originalPathTree.rend(); iter++)
	{
		std::string s = *iter;
		if ((*iter).back() == '/')
		{
			(*iter).erase(iter->end() - 1);
			CC_directory_win32::RemoveDirectory(*iter);
		}
		else
		{
			file.Delete(*iter);
		}
	}
}
#endif

