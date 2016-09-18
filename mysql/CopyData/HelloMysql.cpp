#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/*
127.0.0.1 admin abcd
testdata t1 (id name)
*/
int main()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* query;
	int t,r;

	mysql_init(&mysql);
	
	if(!mysql_real_connect(&mysql, "localhost", "root", "abcd", "DNSdata", 0, NULL, 0) )
	{
		printf("mysql_real_connect error\n");
		return 1;
	}

	query = "select * from dns";
	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));
	if(t)
	{
		printf("mysql_real_query error\n");
		return 1;
	}

	res = mysql_store_result(&mysql);
	while(row = mysql_fetch_row(res))
	{
		for(t=0; t<mysql_num_fields(res); t++)
		{
			printf("%s ", row[t]);
		}
		printf("\n");
	}

	mysql_free_result(res);
	mysql_close(&mysql);
	return 0;
}
