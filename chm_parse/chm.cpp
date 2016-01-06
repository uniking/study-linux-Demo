#include <chm_lib.h>
#include <string.h>
#include <stdio.h>

#include <string>
#include <fstream>

using namespace std;

static int Unchm_extract_callback(chmFile* h, chmUnitInfo* ui, void* context)
{
	static char szBuf[128]={0};
	printf("callback chm\n");
	
	if(ui->flags & CHM_ENUMERATE_NORMAL)
		strcpy(szBuf, "normal ");
	else if(ui->flags & CHM_ENUMERATE_SPECIAL)
		strcpy(szBuf, "special ");
	else if(ui->flags & CHM_ENUMERATE_META)
		strcpy(szBuf, "meta ");
	else 
		strcpy(szBuf, "unknow ");

	if(ui->flags & CHM_ENUMERATE_DIRS)
		strcat (szBuf, "dir ");
	else if(ui->flags & CHM_ENUMERATE_FILES)
		strcat(szBuf, "file");
	
	ofstream ofile(szBuf);
	ofile<<ui->path;
	
	char buffer[32768] = {0};
	int remain = ui->length;
	int len;
	int offset = 0;
	while( remain)
	{
		len = chm_retrieve_object(h, ui, (unsigned char *)buffer, offset, 32768);

		if (len > 0) 
		{
			ofile<<buffer;
			offset += len;
			remain -= len;
		} else 
		{
			fprintf(stderr, "incomplete file: %s\n", ui->path);
			break;
		}
	}

	return CHM_ENUMERATOR_CONTINUE;
}

bool chm_parse_from_file(const std::string& file_name, std::string& text)
{
	struct chmFile *chmp = NULL;

	chmp = chm_open(file_name.c_str());

	if (chmp == NULL)
	{
		printf("chm_open file error. file = %s", file_name.c_str());
		return false;
	}

	string chmtext;
	
	if (!chm_enumerate(chmp, CHM_ENUMERATE_ALL, Unchm_extract_callback, (void*)chmtext.c_str()))
		printf("chm_enumerate error.");
	chm_close(chmp);

	return 0;
}

int main()
{
	string text;
	chm_parse_from_file(string("abc.chm"), text);

	return 0;
}
