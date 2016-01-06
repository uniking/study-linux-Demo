#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iconv.h>

#include <string>
using namespace std;

main()
{
	string ch = "草";
	printf("utf8 草=%d\n", ch.size());

	char* pinbuf;
	size_t inbytesleft=sizeof(ch);
	char* poutbuf;
	size_t outbyteleft=32;
	poutbuf = (char*)malloc(outbyteleft);

	iconv_t cd = iconv_open("GBK", "UTF-8");
	iconv(cd, &pinbuf, &inbytesleft, &poutbuf, &outbyteleft);
	iconv_close(cd);

	ch = poutbuf;
	printf("gbk 草=%d\n", ch.size());
}
