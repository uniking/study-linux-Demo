#ifndef _DATA_ITEM_2_MYSQL_H
#define _DATA_ITEM_2_MYSQL_H
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DataItem.hpp"

#include <map>
#include <list>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

int init_mysql(MYSQL* mysql);
int insert_item(MYSQL* mysql, DATA_ITEM& data);
int query_item(MYSQL* mysql, list<string>& ignoreDay, map<string, list<DATA_ITEM> >& Matrix);

int get_sip(MYSQL* mysql, set<string>& sipSet);
void Tokenize(const string& str,
        vector<string>& tokens,
        const string& delimiters);
int get_item_list_by_sip(MYSQL* mysql, const string& sip, list<string>& ignoreDay, list<DATA_ITEM>& itemList);
//int get_all_item_by_one_day(MYSQL* mysql, const string& day, list<DATA_ITEM>& itemList);
int get_all_item_by_one_day(MYSQL* mysql, const string& day, map<string, list<DATA_ITEM> >& Matrix);

int get_day_set_from_item_database(MYSQL* mysql, list<string>& daySet);

#endif //_DATA_ITEM_2_MYSQL_H
