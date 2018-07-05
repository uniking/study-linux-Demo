#include "memmap.h"
#include <stdio.h>
#include <elf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;
#include <string>
#include <map>

map<int, string> getStringTable64(const char* pStringTable, Elf64_Word length)
{
	map<int, string> tables;
	char buf[1024];

	printf("pStringTable=%p length=%d\n", pStringTable, length);

	for(int index = 0, i=0; i<length;)
	{
		char* p = strncpy(buf, pStringTable + i, 1024);
		if(p == buf)
		{
			if(strlen(p) == 0)
			{
				++i;
			}
			else
			{
				printf("%x %s\n", index, p);
				tables.insert(make_pair(index, p));
				++index;
				i = i + strlen(p);
			}
		}
	}

	return tables;
}

void printX64ELF(const char* start)
{
	const Elf64_Ehdr* hdr = (Elf64_Ehdr*)start;
	//printf("section offset 0x%x, num %d\n", hdr->e_shoff, hdr->e_shnum);
	const Elf64_Shdr* pSection = (const Elf64_Shdr*)(start + hdr->e_shoff);

	const char* pstringTable = (start + (pSection+hdr->e_shstrndx)->sh_offset);
	Elf64_Word lenght = (pSection + hdr->e_shstrndx)->sh_size;
	//getStringTable64(pstringTable, lenght);

	const char* pstrtab = 0;
	Elf64_Word sstrtab = 0;
	const char* pdynstr = 0;
	Elf64_Word sdynstr = 0;
	const char* pdynamic = 0;
	Elf64_Word sdynamic = 0;
	Elf64_Xword dmEntsize = 0;
	for(int i=0; i < hdr->e_shnum; ++i)
	{
		//printf("%x %s %d\n", i, pstringTable + pSection->sh_name, pSection->sh_entsize);
		++pSection;

		if(strncmp(".strtab", pstringTable + pSection->sh_name, 7) == 0)
		{
			pstrtab = start + pSection->sh_offset;
			sstrtab = pSection->sh_size;
		}

		if(strncmp(".dynstr", pstringTable + pSection->sh_name, 7) == 0)
		{
			pdynstr = start + pSection->sh_offset;
			sdynstr = pSection->sh_size;
		}

		if(strncmp(".dynamic", pstringTable + pSection->sh_name, 8) == 0)
		{
			pdynamic = start + pSection->sh_offset;
			sdynamic = pSection->sh_size;
			dmEntsize = pSection->sh_entsize;
		}
	}

	//getStringTable64(pstrtab, sstrtab);
	//getStringTable64(pdynstr, sdynstr);

	//依赖的so文件
	Elf64_Dyn* one = (Elf64_Dyn*) pdynamic;
	for(int i=0; i<dmEntsize; ++i)
	{
		if(one->d_tag == 1)
			printf("%d %s\n", one->d_tag, pdynstr + one->d_un.d_val);
		else
			;//printf("%d %d\n", one->d_tag, one->d_un);
		++one;
	}
}

int main(int argn, char** argv)
{
	//printf("argn=%d\n", argn);
	if(argn != 2)
	{
		printf("elfParser /bin/bash\n");
		return 0;
	}
	else
	{
		;//printf("%s %s\n", argv[0], argv[1]);
	}

	MemMap elfFile;
	bool bFailed = elfFile.Map(argv[1]);
	if(!bFailed)
	{
		printf("map error\n");
		return -1;
	}

	size_t uFileSize  = elfFile.GetSize();
	const char* pData = (const char*)elfFile.GetData();
	if(uFileSize <=0 || NULL == pData)
	{
		printf("getdata error\n");
		return -2;
	}

	Elf32_Ehdr* hdr = (Elf32_Ehdr*)pData;
	printf("%d\n", hdr->e_machine);
	switch(hdr->e_machine)
	{
		case 62:
			printX64ELF(pData);
			break;
		default:
			break;
	}

	return 0;
}
