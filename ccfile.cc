#include "ccfile.h"

#include <assert.h>
#include <time.h>
#include <queue>
#include <thread>
FileStream* CCFile::Create(std::string path)
{
	assert(path.length() > 0);
	FileStream* stream = new FileStream();
	stream->Getfd() = fopen(path.c_str(), "r+");
	return stream;
	//MemoryLeaks Caution!
}

intptr_t CCFile::Copy(std::string sourcePath, std::string destPath)
{
	FileStream sourceStream;
	FileStream destStream;

	sourceStream.Getfd() = fopen(sourcePath.c_str(), "rb");
	destStream.Getfd() = fopen(destPath.c_str(), "wb");
	intptr_t bytes = CopyFileData(&sourceStream, &destStream);
	//空文件bytes为0
	if (bytes == 0)
	{
		printf("SourcePath %s\n", sourcePath.c_str());
	}
	assert(bytes >= 0);
	return bytes;
}

void CCFile::CopyAsync(std::string sourcePath, std::string destPath)
{
	std::thread t(&CopySync, sourcePath, destPath);
	t.join();
}

intptr_t CCFile::CopySync(std::string sourcePath, std::string destPath)
{
	FileStream sourceStream;
	FileStream destStream;
	sourceStream.Getfd() = fopen(sourcePath.c_str(), "rb");
	destStream.Getfd() = fopen(destPath.c_str(), "wb");
	intptr_t bytes = CopyFileData(&sourceStream, &destStream);
	assert(bytes > 0);
	return bytes;
}

long CCFile::CopyFileData(FileStream* src, FileStream* dest)
{
	if (src == NULL || dest == NULL) {
		return -1;
	}
	// 指针移动到文件末尾
	fseek(src->Getfd(), 0, SEEK_END);
	// 获取文件大小
	long file_size = ftell(src->Getfd());
	// 指针回到文件头
	rewind(src->Getfd());

	int res = 0;
	intptr_t count = 0; // 进度。

	const int LEN_DATA = 128 * 1024;
	unsigned char data[LEN_DATA] = { 0 };
	//    printf("size: %lu \n", sizeof(data));
	while (1) {
		// 获取当前位置
		long current = ftell(src->Getfd());
		// 计算剩余量
		long leave = file_size - current;
		int readSize = LEN_DATA;
		if (leave == 0) {
			// 拷贝结束了。
			printf("leave == 0 feof.\n");
			break;
		}
		else if (leave < LEN_DATA) {
			// 拷贝快结束了，但数据量已经小于LEN_DATA了
			readSize = (int)leave;
		}
		// 读取数据块
		size_t res_t = fread(&data, 1, readSize, src->Getfd());
		if (res_t == readSize) {
			// 正常读取
			size_t w_res = fwrite(&data, 1, res_t, dest->Getfd());
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
		else if (feof(src->Getfd()) != 0){
			printf("feof res_t=%u \n", res_t);
			break;
		}
		else {
			printf("fread fail...\n");
			res = -1;
			break;
		}
	}
	printf("一共写字节数为: %d\n", count);
	return (res == -1) ? -1 : count;
}

void CCFile::Delete(std::string path)
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
bool CCFile::Exists(std::string path)
{
	int exist = _access(path.c_str(), FileAccess::EXISTENCEONLY);
	if (exist)
	{
		return false;
	}
	return true;
}

FileAccess CCFile::GetAccessControl(std::string path)
{
	int exist = _access(path.c_str(), FileAccess::WRITEPERMISSION);
	if (exist)
	{
		return FileAccess::READPERMISSION;
	}
	else
		return FileAccess::READANDWRITEPERMISSION;
}

FileAttributes CCFile::GetAttributes(std::string path)
{
	bool exist = Exists(path);
	_finddata_t fileInfo;
	if (exist)
	{
		intptr_t handle = _findfirst(path.c_str(), &fileInfo);
		if (handle == -1)
		{
			fprintf(stderr, "No matching Unknown attributes %s %d\n", __FUNCTION__, __LINE__);
			return FileAttributes::UNKNOWN;
		}
		auto value = (int)fileInfo.attrib;
		return (FileAttributes)value;
		_findclose(handle);
	}
	return FileAttributes::UNKNOWN;
}

DateTime CCFile::GetCreateTime(std::string path)
{
	bool exist = Exists(path);
	_finddata_t fileInfo;
	if (exist)
	{
		intptr_t handle = _findfirst(path.c_str(), &fileInfo);
		if (handle == -1)
		{
			fprintf(stderr, "No matching files at %s %d\n", __FUNCTION__, __LINE__);
			return DateTime();
		}
		else
		{
			time_t value = fileInfo.time_create;
			struct tm* tim = localtime(&value);
			DateTime date;
			date.year = tim->tm_yday + 1900;
			date.month = tim->tm_mon + 1;
			date.day = tim->tm_mday;
			date.hour = tim->tm_hour;
			date.minute = tim->tm_min;
			date.sec = tim->tm_sec;
			_findclose(handle);
			return date;
		}
	}
	return DateTime();
}

DateTime CCFile::GetLastAccessTime(std::string path)
{
	bool exist = Exists(path);
	_finddata_t fileInfo;
	if (exist)
	{
		intptr_t handle = _findfirst(path.c_str(), &fileInfo);
		if (handle == -1)
		{
			fprintf(stderr, "No matching %s %d\n", __FUNCTION__, __LINE__);
			return DateTime();
		}
		else
		{
			time_t value = fileInfo.time_access;
			struct tm* tim = localtime(&value);
			DateTime date;
			date.year = tim->tm_yday + 1900;
			date.month = tim->tm_mon + 1;
			date.day = tim->tm_mday;
			date.hour = tim->tm_hour;
			date.minute = tim->tm_min;
			date.sec = tim->tm_sec;
			_findclose(handle);
			return date;
		}
	}
	return DateTime();
}

DateTime CCFile::GetLastModifyTime(std::string path)
{
	bool exist = Exists(path);
	_finddata_t fileInfo;
	if (exist)
	{
		intptr_t handle = _findfirst(path.c_str(), &fileInfo);
		if (handle == -1)
		{
			return DateTime();
		}
		else
		{
			time_t value = fileInfo.time_write;
			struct tm* tim = localtime(&value);
			DateTime date;
			date.year = tim->tm_yday + 1900;
			date.month = tim->tm_mon + 1;
			date.day = tim->tm_mday;
			date.hour = tim->tm_hour;
			date.minute = tim->tm_min;
			date.sec = tim->tm_sec;
			_findclose(handle);
			return date;
		}
	}
	return DateTime();
}

void CCFile::Move(std::string sourcePath, std::string destPath)
{
	long copyBytes = Copy(sourcePath, destPath);
	assert(copyBytes > 0);
	Delete(sourcePath);
}

FileStream* CCFile::Open(std::string path, FileMode mode)
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
		//内存释放问题
	}
	return nullptr;
}

FileStream* CCFile::OpenRead(std::string path)
{
	return Open(path, FileMode::READDONLY);
}

FileStream* CCFile::OpenWrite(std::string path)
{
	return Open(path, FileMode::WRITEONLY);
}

intptr_t CCFile::ReadAllBytes(std::string path, char destBuffer[], size_t length)
{
	bool exist = Exists(path);
	if (!exist)
		return -1;
	FileStream stream;
	stream.Getfd() = fopen(path.c_str(), "r");

	if (stream.Getfd() == nullptr)
	{
		return -1;
	}
	fseek(stream.Getfd(), 0, SEEK_END);
	long fileLength = ftell(stream.Getfd());
	rewind(stream.Getfd());
	if (fileLength >= length)
	{
		fprintf(stderr, "Insufficient Buffer at %s %d\n", __FUNCTION__, __LINE__);
		return -1;
	}
	size_t readBytes = fread(destBuffer, 1, fileLength, stream.Getfd());
	while (readBytes < fileLength)
	{
		size_t bytes = fread(destBuffer + readBytes, 1, fileLength - readBytes, stream.Getfd());
		readBytes += bytes;
	}
	assert(readBytes == fileLength);
	return readBytes;
}

long CCFile::ReadAllLines(std::string path, std::vector<std::string>&destContainer)
{
	bool exist = Exists(path);
	if (!exist)
		return -1;
	FileStream stream;
	stream.Getfd() = fopen(path.c_str(), "r");

	if (stream.Getfd() == nullptr)
	{
		return -1;
	}

	char buff[300] = { 0 };
	long count = 0;
	const char* bufPtr = nullptr;
	while (bufPtr != nullptr)
	{
		 bufPtr = fgets(buff, sizeof(buff), stream.Getfd());
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

std::string CCFile::ReadAllText(std::string path)
{
	bool exist = Exists(path);
	if (!exist)
		return "";
	FileStream stream;
	stream.Getfd() = fopen(path.c_str(), "r");

	if (stream.Getfd() == nullptr)
	{
		return "";
	}
	std::string retValue = "";
	char buff[300] = { 0 };
	const char* bufPtr = nullptr;
	while (bufPtr != nullptr)
	{
		bufPtr = fgets(buff, sizeof(buff), stream.Getfd());
		if (bufPtr != nullptr)
		{
			retValue += bufPtr;
			*(buff + strlen(buff)) = '\0';
		}
	}
	return retValue;
}

bool CCFile::Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath)
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

intptr_t CCFile::WriteAllBytes(std::string path, const char sourceData[], size_t len)
{
	FileStream stream;
	stream.Getfd() = fopen(path.c_str(), "w");
	if (stream.Getfd() == nullptr)
	{
		return -1;
	}
	size_t writenBytes = fwrite(sourceData, 1, len, stream.Getfd());
	while (writenBytes < len)
	{
		size_t bytes = fwrite(sourceData + writenBytes, 1, len - writenBytes, stream.Getfd());
		if (bytes == 0)
		{
			int err = ::ferror(stream.Getfd());
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

long CCFile::WriteAllLines(std::string path, const std::vector<std::string>& sourceData)
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

intptr_t CCFile::WriteAllText(std::string path, std::string& str)
{
	return WriteAllBytes(path, str.c_str(), str.length());
}

void CCFile::SetAccessControl(FileMode md)
{
	/**
	 *
	 * @Brief:
	 */
}
