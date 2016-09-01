/*  
 * File:   sqlite_test.cpp 
 * Author: Carl 
 * 
 * Created on September 20, 2012, 3:28 PM 
 */  

#include <stdio.h> 
#include <sqlite3.h>  
  
static int _sql_callback(void *notused, int argc, char **argv, char **szColName)  
{  
	int i = 0;  

	printf("notused:0x%x, argc:%d\n", notused, argc);  
	for (i = 0; i < argc; i++)  
	{  
		printf("%s = %s\n", szColName[i], argv[i] == 0 ? "NULL" : argv[i]);  
	}  
	printf("\n");  

	return 0;  
}  
  
/* 
 *  
 */  
int main(int argc, char** argv)  
{  
    const char *sSQL1 = "create table users(userid varchar(20) PRIMARY KEY, age int, birthday datetime);";  
    const char *sSQL2 = "insert into users values('wang', 20, '1989-5-4');";  
    const char *sSQL3 = "select * from users;";  
      
    sqlite3 *db = 0;  
    char *pErrMsg = 0;  
    int ret = 0;  
      
    //连接数据库  
	ret = sqlite3_open("./test.db", &db);  
	if (ret != SQLITE_OK)  
	{  
		fprintf(stderr, "sqlite3_open error：%s\n", sqlite3_errmsg(db));  
		sqlite3_close(db);  
		return 1;  
	}  
    printf("sqlite3_open ok\n");  
      
    //执行建表SQL  
    ret = sqlite3_exec(db, sSQL1, _sql_callback, 0, &pErrMsg);  
    if (ret != SQLITE_OK)  
    {  
        fprintf(stderr, "sqlite3_exec error: %s\n", pErrMsg);  
        sqlite3_free(pErrMsg); //这个要的哦，要不然会内存泄露的哦！！！  
        sqlite3_close(db);  
        return 1;  
    }  
    printf("sqlite3_exec ok\n");  
      
    //执行插入数据  
    ret = sqlite3_exec(db, sSQL2, _sql_callback, 0, &pErrMsg);  
    if (ret != SQLITE_OK)  
    {  
        fprintf(stderr, "SQL insert error: %s\n", pErrMsg);  
        sqlite3_free(pErrMsg); //这个要的哦，要不然会内存泄露的哦！！！  
        sqlite3_close(db);  
        return 1;  
    }  
    printf("sqlite3_exec ok\n");  
      
    //执行查询操作  
    ret = sqlite3_exec(db, sSQL3, _sql_callback, 0, &pErrMsg);  
    if (ret != SQLITE_OK)  
    {  
        fprintf(stderr, "SQL error: %s\n", pErrMsg);  
        sqlite3_free(pErrMsg);  
        sqlite3_close(db);  
        return 1;  
    }  
    printf("sqlite3_exec ok\n");  
      
    //关闭数据库  
    sqlite3_close(db);  
    db = 0;  
      
    return 0;  
}  
