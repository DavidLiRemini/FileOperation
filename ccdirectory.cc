#include "ccdirectory.h"

#include <assert.h>
#include <time.h>
#include <queue>
#include <thread>
void CCDirectory::CreateDirectory(std::string path)
{
	static int count = 1;
	int stat = _mkdir(path.c_str());
	printf("Create directory %d\n", count++);
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

void CCDirectory::Delete(std::string path)
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

std::vector<std::string> CCDirectory::GetSubDirectoriesCollection(std::string path)
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

std::vector<std::string>CCDirectory::GetFiles(std::string path)
{
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

std::vector<std::string> CCDirectory::GetFilesAndDirectories(std::string path)
{
	std::vector<std::string>directory = GetSubDirectoriesCollection(path);
	std::vector<std::string>files = GetFiles(path);
	directory.insert(directory.end(), files.begin(), files.end());
	return directory;
}

bool CCDirectory::Exists(std::string path)
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

void CCDirectory::GetAccessControl(std::string path)
{

}

DateTime CCDirectory::GetCreateTime(std::string path)
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

DateTime CCDirectory::GetLastAccessTime(std::string path)
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

void CCDirectory::SetDateTime(DateTime& dt, struct tm* tim)
{
	dt.year = tim->tm_yday + 1900;
	dt.month = tim->tm_mon + 1;
	dt.day = tim->tm_mday;
	dt.hour = tim->tm_hour;
	dt.minute = tim->tm_min;
	dt.sec = tim->tm_sec;
}

DateTime CCDirectory::GetLastWriteTime(std::string path)
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

std::string CCDirectory::GetCurrentDirectory()
{
	char buf[100] = { 0 };
	char* str = _getcwd(buf, sizeof(buf));
	return str;
}

std::string CCDirectory::GetDirectoryRoot(std::string path)
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

std::string CCDirectory::GetRootDriveSymbol(int index)
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

std::string CCDirectory::GetParentDirectory(std::string path)
{
	if (!Exists(path))
	{
		return "";
	}

	size_t index = path.rfind('/');
	std::string parent = path.substr(0, index);
	return parent;
}

void CCDirectory::MoveDirectory(std::string sourcePath, std::string destPath)
{
	if (destPath.back() != '/')
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
	RemoveCatalogTree(catalogTreeNode);

}

void CCDirectory::MoveDirectoryFast(std::string sourcePath, std::string destPath)
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
	CCDirectory::CreateDirectory(destPath + folderName);
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
	CCDirectory::Delete(sourcePath);
}

void CCDirectory::CopyDirectory(std::string sourcePath, std::string destPath)
{

	if (sourcePath.back() != '/')
	{
		sourcePath += '/';
	}
	auto index = std::string(sourcePath.begin(), sourcePath.end() -1).rfind('/');
	std::string folderName = sourcePath.substr(index + 1, sourcePath.length() - index - 2);
	if (destPath.back() != '/')
	{
		destPath += "/";
	}
	CCDirectory::CreateDirectory(destPath + folderName);
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

void CCDirectory::CopyFile(FolderNode& node, std::string sourcePath, std::string destPath)
{
	for (auto item : node.subChildren)
	{
		std::string str = item.name.substr(sourcePath.size());
		std::string dest = destPath + str;
		if (item.nodeAttr == FileAttributes::NORMAL)
		{
			CCFile::Copy(item.name, dest);
		}
		else
			CopyFile(item, sourcePath, destPath);
	}
}

void CCDirectory::SetCurrentDirectory(std::string path)
{
	/**
	 *
	 * ///////
	 */
}

void CCDirectory::SetCreateTime(DateTime tim)
{
	/**
	 *
	 * @Brief:
	 */
}
void CCDirectory::SetLastAccessTime(DateTime tm)
{
	/**
	 *
	 * @Brief:
	 */
}
void CCDirectory::SetLastWriteTime(DateTime tm)
{
	/**
	 *
	 * @Brief:
	 */
}

FolderNode CCDirectory::GetOldCatalogTree(std::string path)
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


void CCDirectory::ConstructNewCatalogTree(FolderNode root, std::string oldpath, std::deque<std::string>& prefix)
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

void CCDirectory::RemoveCatalogTree(FolderNode& node)
{
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
			CCFile::Delete(it.name);
		}
	}

	CCDirectory::Delete(node.name);
}

void CCDirectory::Search(std::string path,std::vector<std::string>& vec,std::string searchSymbol)
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
			return ;
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
		else if (name !=  "." && name != "..")
		{
			Search(path + name, vec, searchSymbol);
		}
	} while (!_findnext(handle, &info));
	_findclose(handle);
}

std::vector<std::string> CCDirectory::SearchFile(std::string path, bool recursive/* =false */)
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

void CCDirectory::ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath)
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

void CCDirectory::ParseOldCatalogTree(std::string path, std::vector<std::string>& treePath, Directory_Info& d_info)
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
			++d_info.directoryNumberInfo;
			treePath.push_back(s);
			ParseOldCatalogTree(s, treePath, d_info);
		}
		else if (nd != "." && nd != "..")
		{
			std::string t = path + nd;
			++d_info.fileNumberInfo;
			d_info.fileTotalSizeInfo += info.size;
			treePath.push_back(t);
		}
	} while (!_findnext(handle, &info));
	_findclose(handle);
}

void CCDirectory::DisplayDirectoryInformation(std::string path)
{
	if (path.back() != '/')
	{
		path += "/";
	}
	std::vector<std::string>oldPath;
	Directory_Info info;
	CCDirectory::ParseOldCatalogTree(path, oldPath, info);
	if (info.fileTotalSizeInfo >= 1024*1024*1024)
	{
		printf("Directory's total file size: %4.3gG\n", info.fileTotalSizeInfo / (1024*1024*1024));
	}
	else if (info.fileTotalSizeInfo >= 1024 * 1024)
	{
		printf("Directory's total file size: %4.3gM\n", info.fileTotalSizeInfo / (1024 * 1024));
	}
	else
		printf("Directory's total file size: %4.3gK\n", info.fileTotalSizeInfo / 1024);
	printf("Total number of files: %lu\n", info.fileNumberInfo);
	printf("Total number of subdirectory: %lu\n", info.directoryNumberInfo);
}

void CCDirectory::SplitFilesAndDirectories(
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

void CCDirectory::RebuildCatalogTree(std::vector<std::string>& newDirectory)
{
	for (auto& it : newDirectory)
	{
		if (!CCDirectory::Exists(it))
		{
			CCDirectory::CreateDirectory(it);
		}
	}
}

void CCDirectory::CopyOriginFile(std::vector<std::string>& originFiles, std::vector<std::string>& newFiles)
{
	/**
	 *
	 * 新旧目录文件数目应该一样
	 */
	assert(originFiles.size() == newFiles.size());
	for (size_t i = 0; i < originFiles.size(); i++)
	{
		CCFile::Copy(originFiles.at(i), newFiles.at(i));
	}
}

void CCDirectory::RemoveOldCatalogTree(std::vector<std::string>& originalPathTree)
{
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
			CCDirectory::Delete(*iter);
		}
		else
		{
			CCFile::Delete(*iter);
		}
	}
}
