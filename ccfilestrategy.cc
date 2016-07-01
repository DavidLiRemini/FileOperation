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