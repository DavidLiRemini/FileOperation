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

intptr_t CC_file_linux::Copy(std::string sourcePath, std::string destPath)
{
	return CC_file_strategy::Copy(sourcePath, destPath);
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
