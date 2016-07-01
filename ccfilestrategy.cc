#include "ccfilestrategy.h"

#include <assert.h>

CC_file_strategy* CC_file_strategy::f_instance = nullptr;

static long CopyFileData(FileStream* src, FileStream* dest)
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
void CC_file_strategy::Create(std::string path)
{
	assert(path.length() > 0);
	FileStream stream;
	stream.Getfd() = fopen(path.c_str(), "w");
}

intptr_t CC_file_strategy::Copy(std::string sourcePath, std::string destPath)
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

void CC_file_strategy::Delete(std::string path)
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

void CC_file_strategy::Move(std::string sourcePath, std::string destPath)
{
	long copyBytes = Copy(sourcePath, destPath);
	assert(copyBytes > 0);
	Delete(sourcePath);
}

FileStream* CC_file_strategy::Open(std::string path, FileMode mode)
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

FileStream* CC_file_strategy::OpenRead(std::string path)
{
	return Open(path, FileMode::READDONLY);
}

FileStream* CC_file_strategy::OpenWrite(std::string path)
{
	return Open(path, FileMode::WRITEONLY);
}

intptr_t CC_file_strategy::ReadAllBytes(std::string path, char destBuffer[], size_t length)
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

long CC_file_strategy::ReadAllLines(std::string path, std::vector<std::string>& destContainer)
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

std::string CC_file_strategy::ReadAllText(std::string path)
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

bool CC_file_strategy::Replace(std::string sourcePath, std::string replaceFilePath, std::string backPath)
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

intptr_t CC_file_strategy::WriteAllBytes(std::string path, const char sourceData[], size_t len)
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

long CC_file_strategy::WriteAllLines(std::string path, const std::vector<std::string>& sourceData)
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

intptr_t CC_file_strategy::WriteAllText(std::string path, std::string& content)
{
	return WriteAllBytes(path, content.c_str(), content.length());
}
