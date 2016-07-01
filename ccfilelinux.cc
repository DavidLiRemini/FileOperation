#include "platformconfig.h"
#if CCTARGET_PLATFORM == CCPLATFORM_LINUX

#include "ccfilelinux.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

CC_file_strategy* CC_file_strategy::GetInstance()
{
	if(f_instance == nullptr)
	{
		f_instance = new CC_file_linux();
	}
	return f_instance;
}

void CC_file_linux::Create(std::string path)
{
	CC_file_strategy::Create(path);
}

bool CC_file_linux::Exists(std::string path)
{
	if (path.back() == '/')
		path.erase(path.end() - 1);
	int stat = access(path.c_str(), F_OK);
	if (stat)
	{
		return false;
	}
	return true;
}
void CC_file_linux::Delete(std::string path)
{
	bool exist = Exists(path);
	if (!exist)
	{
		fprintf(stderr, "File does not exists\n");
		return;
	}
	int stat = ::remove(path.c_str());
	if (stat != 0)
	{
		fprintf(stderr, "Remove file %s failed\n", path.c_str());
		return;
	}
}

bool CC_file_linux::CanRead(std::string path)
{
	if (!Exists(path))
	{
		fprintf(stderr, "No such a file or Directory\n");
		return false;
	}
	int state = access(path.c_str(), R_OK);
	if (!state)
	{
		int err = errno;
		if (err == EACCES)
		{
			return true;
		}
	}
	return false;
}

bool CC_file_linux::CanWrite(std::string path)
{
	if (!Exists(path))
	{
		fprintf(stderr, "No such a file or Directory\n");
		return false;
	}
	int state = access(path.c_str(), W_OK);
	if (!state)
	{
		int err = errno;
		if (err == EACCES)
		{
			return true;
		}
	}
	return false;
}

bool CC_file_linux::CanExecute(std::string path)
{
	if (!Exists(path))
	{
		fprintf(stderr, "No such a file or Directory\n");
		return false;
	}
	int state = access(path.c_str(), X_OK);
	if (!state)
	{
		int err = errno;
		if (err == EACCES)
		{
			return true;
		}
	}
	return false;
}

void CC_file_linux::Move(std::string sourcePath, std::string destPath)
{
	long copyBytes = Copy(sourcePath, destPath);
	assert(copyBytes > 0);
	Delete(sourcePath);
}

intptr_t CC_file_linux::Copy(std::string sourcePath, std::string destPath)
{
	return CC_file_strategy::Copy(sourcePath, destPath);
}
FileStream* CC_file_linux::Open(std::string path, FileMode mode)
{
	bool exists = Exists(path);
	std::string openMode = "";
	FileMode md = mode;
	if (md & FileMode::READDONLY)
	{
		openMode = "r";
	}
	else if (md & FileMode::RDWRITE)
	{
		openMode = "r+";
	}
	else if (md& FileMode::WRITEONLY)
	{
		openMode = "w";
	}
	else if (md & FileMode::WRITE)
	{
		openMode = "w+";
	}
	else if (md & FileMode::APPEND)
	{
		openMode = "a";
	}
	else if (md & FileMode::APPENDANDRD)
	{
		openMode = "a+";
	}
	if (md & FileMode::BINARY)
	{
		openMode += "b";
	}
	FileStream* stream = new FileStream();
	if (exists)
	{
		stream->Getfd() = fopen(path.c_str(), openMode.c_str());
		return stream;
		//�ڴ��ͷ�����
	}
	return nullptr;
}

FileStream* CC_file_linux::OpenRead(std::string path)
{
	return CC_file_linux::Open(path, FileMode::READDONLY);
}

FileStream* CC_file_linux::OpenWrite(std::string path)
{
	return CC_file_linux::Open(path, FileMode::WRITEONLY);
}

intptr_t CC_file_linux::ReadAllBytes(std::string path, char destBuffer[], size_t length)
{
	bool exist = Exists(path);
	if (!exist)
		return -1;
	FILE* stream = nullptr;
	stream = fopen(path.c_str(), "r");
	if (stream == nullptr)
	{
		return -1;
	}
	fseek(stream, 0, SEEK_END);
	long fileLength = ftell(stream);
	rewind(stream);
	if (fileLength >= length)
	{
		fprintf(stderr, "Insufficient Buffer at %s %d\n", __FUNCTION__, __LINE__);
		return -1;
	}
	size_t readBytes = fread(destBuffer, 1, fileLength, stream);
	while (readBytes < fileLength)
	{
		size_t bytes = fread(destBuffer + readBytes, 1, fileLength - readBytes, stream);
		readBytes += bytes;
	}
	assert(readBytes == fileLength);
	return readBytes;
}
long CC_file_linux::ReadAllLines(std::string path, std::vector<std::string>& destContainer)
{
	bool exist = Exists(path);
	if (!exist)
		return -1;
	FILE* stream = nullptr;
	stream = fopen(path.c_str(), "r");
	if (stream == nullptr)
	{
		return -1;
	}
	char buff[300] = { 0 };
	long count = 0;
	const char* bufPtr = nullptr;
	while (bufPtr != nullptr)
	{
		bufPtr = fgets(buff, sizeof(buff), stream);
		if (bufPtr != nullptr)
		{
			*(buff + strlen(buff)) = '\0';
			destContainer.push_back(buff);
			++count;
		}
	}
	assert(count == destContainer.size());
	return count;
}
std::string CC_file_linux::ReadAllText(std::string path)
{
	bool exist = Exists(path);
	if (!exist)
		return "";
	FILE* stream = nullptr;
	stream = fopen(path.c_str(), "r");

	if (stream == nullptr)
	{
		return "";
	}
	std::string retValue = "";
	char buff[300] = { 0 };
	const char* bufPtr = nullptr;
	while (bufPtr != nullptr)
	{
		bufPtr = fgets(buff, sizeof(buff), stream);
		if (bufPtr != nullptr)
		{
			retValue += bufPtr;
			*(buff + strlen(buff)) = '\0';
		}
	}
	return retValue;
}

bool CC_file_linux::Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath)
{
	if (!Exists(sourcePath) || !Exists(replaceFilePath))
	{
		return false;
	}
	long copyByes = Copy(sourcePath, backPath);
	assert(copyByes > 0);
	Delete(sourcePath);
	long sz = Copy(replaceFilePath, sourcePath);
	assert(sz > 0);
	return true;
}

intptr_t CC_file_linux::WriteAllBytes(std::string path, const char sourceData[], size_t len)
{
	FILE* stream = nullptr;
	stream = fopen(path.c_str(), "w");
	if (stream == nullptr)
	{
		return -1;
	}
	size_t writenBytes = fwrite(sourceData, 1, len, stream);
	while (writenBytes < len)
	{
		size_t bytes = fwrite(sourceData + writenBytes, 1, len - writenBytes, stream);
		if (bytes == 0)
		{
			int err = ::ferror(stream);
			if (err)
			{
				fprintf(stderr, "write file failed %s %s %d\n", strerror(err), __FUNCTION__, __LINE__);
			}
		}
		writenBytes += bytes;
	}
	assert(writenBytes == len);
	return writenBytes;
}

long CC_file_linux::WriteAllLines(std::string path, const std::vector<std::string>&sourceData)
{
	std::string s = "";
	long lines = 0;
	for (auto it : sourceData)
	{
		s += it;
		lines++;
	}
	intptr_t bytes = WriteAllBytes(path, s.c_str(), sourceData.size());
	assert(bytes == s.size());
	return lines;
}

intptr_t CC_file_linux::WriteAllText(std::string path, std::string& content)
{
	return WriteAllBytes(path, content.c_str(), content.length());
}

long CC_file_linux::CopyFileData(FILE* src, FILE* dest)
{
	if (src == NULL || dest == NULL) {
		return -1;
	}
	fseek(src, 0, SEEK_END);
	//Get file size
	long file_size = ftell(src);
	//reset pointer
	rewind(src);
	int res = 0;
	intptr_t count = 0; //progress

	const int LEN_DATA = 128 * 1024;
	unsigned char data[LEN_DATA] = { 0 };
	//    printf("size: %lu \n", sizeof(data));
	while (1) {
		//current position
		long current = ftell(src);
		//remainder
		long leave = file_size - current;
		int readSize = LEN_DATA;
		if (leave == 0) {
			//copy finished
			printf("leave == 0 feof.\n");
			break;
		}
		else if (leave < LEN_DATA) {
			readSize = (int)leave;
		}
		size_t res_t = fread(&data, 1, readSize, src);
		if (res_t == readSize) {
			size_t w_res = fwrite(&data, 1, res_t, dest);
			count += w_res;
			if (res_t == w_res) {
				//printf("write=%ld res_t=%u \n", count, res_t);
			}
			else {
				printf("fwrite fail. \n");
				res = -1;
				break;
			}
		}
		else if (feof(src) != 0){
			printf("feof res_t=%u \n", res_t);
			break;
		}
		else {
			printf("fread fail...\n");
			res = -1;
			break;
		}
	}
	printf("一共写入: %d\n", count);
	return (res == -1) ? -1 : count;
}
#endif
