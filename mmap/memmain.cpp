
#include "memmap.h"
#include <stdio.h>
  
int main(int argc, char* argv[])  
{
	if(argc != 2)
	{
		printf("%s filename\n", argv[0]);
		return 0;
	}
	const char* szFileName = argv[1];
	const char* szFileNew  = "2.txt";
	MemMap mm;
	bool bFailed = !mm.Map(szFileName);
	if(bFailed)
	{
		printf("mm.map error\n");
		return -1;
	}

	size_t uFileSize  = mm.GetSize();
	const char* pData = (char*)mm.GetData();
	if(uFileSize <=0 || NULL == pData)
	{
		printf("mm.getdata error\n");
		return -2;
	}

	FILE*  pNewFile  = fopen(szFileNew, "w");
	fwrite(pData, sizeof(char), uFileSize, pNewFile);
	fclose(pNewFile);
	pNewFile = NULL;

	return 0; 
}
