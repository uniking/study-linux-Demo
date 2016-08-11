#ifndef _QUERY_BY_TIME
#define _QUERY_BY_TIME

#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "DataItem.hpp"

#include <string>
#include <vector>
#include <set>
#include <list>
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

int database_to_item(string site, int e, map<string, list<DATA_ITEM> >& Matrix, list<string>& ignoreDay, bool filtrate=false);

int get_item_by_one_day(string site, char* day, int e, list<DATA_ITEM>& itemList, bool filtrate=false);

#endif //_QUERY_BY_TIME
