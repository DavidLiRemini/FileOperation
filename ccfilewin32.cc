#include "platformconfig.h"
#if CCTARGET_PLATFORM == CCPLATFORM_WIN32

#include "ccfilewin32.h"
#include <time.h>
#include <assert.h>

CC_file_strategy* CC_file_strategy::GetInstance()
{
	if (CC_file_strategy::f_instance == nullptr)
	{
		f_instance = new CC_file_win32();
	}
	return f_instance;
}

void CC_file_win32::Create(std::string path)
{
	CC_file_strategy::Create(path);
}

intptr_t CC_file_win32::Copy(std::string sourcePath, std::string destPath)
{
	return CC_file_strategy::Copy(sourcePath, destPath);
}


void CC_file_win32::Delete(std::string path)
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
bool CC_file_win32::Exists(std::string path)
{
	int exist = _access(path.c_str(), FileAccess::EXISTENCEONLY);
	if (exist)
	{
		return false;
	}
	return true;
}

bool CC_file_win32::CanRead(std::string path)
{
	int exist = _access(path.c_str(), FileAccess::READPERMISSION);
	if (exist)
	{
		return false;
	}
	return true;
}

bool CC_file_win32::CanExecute(std::string path)
{
	if (!Exists(path))
		return false;
	auto index = path.rfind('.');
	std::string str = std::string(path.begin() + index + 1, path.end());
	if (str == "exe" || str == "bat")
		return true;
	return false;
}

bool CC_file_win32::CanWrite(std::string path)
{
	int exist = _access(path.c_str(), FileAccess::WRITEPERMISSION);
	if (exist)
	{
		return false;
	}
	return true;
}

FileAccess CC_file_win32::GetAccessControl(std::string path)
{
	int exist = _access(path.c_str(), FileAccess::WRITEPERMISSION);
	if (exist)
	{
		return FileAccess::READPERMISSION;
	}
	else
		return FileAccess::READANDWRITEPERMISSION;
}

FileAttributes CC_file_win32::GetAttributes(std::string path)
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
		_findclose(handle);
		return (FileAttributes)value;
	}
	return FileAttributes::UNKNOWN;
}

DateTime CC_file_win32::GetCreateTime(std::string path)
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

DateTime CC_file_win32::GetLastAccessTime(std::string path)
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

DateTime CC_file_win32::GetLastModifyTime(std::string path)
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

long CC_file_win32::CopyFileData(FileStream* src, FileStream* dest)
{
	if (src == NULL || dest == NULL) {
		return -1;
	}
	// move to the end of file	
	fseek(src->Getfd(), 0, SEEK_END);
	// get file size
	long file_size = ftell(src->Getfd());
	// back to the beginning of file
	rewind(src->Getfd());

	int res = 0;
	intptr_t count = 0; // percentage。

	const int LEN_DATA = 128 * 1024;
	unsigned char data[LEN_DATA] = { 0 };
	//    printf("size: %lu \n", sizeof(data));
	while (1) {
		// current position
		long current = ftell(src->Getfd());
		// remainder
		long leave = file_size - current;
		int readSize = LEN_DATA;
		if (leave == 0) {
			// done
			printf("leave == 0 feof.\n");
			break;
		}
		else if (leave < LEN_DATA) {
			// almost done 
			readSize = (int)leave;
		}
		
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
#endif
