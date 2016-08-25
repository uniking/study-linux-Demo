#ifndef _USER_PROFILE
#define _USER_PROFILE

#include "DataItem.hpp"
#include "DNSModel.hpp"
#include "Result.hpp"

#include <map>
#include <string>

using namespace std;

class CUserProfile
{
public:

	map<string, CDNSModel> model_list;//<site+user, model>

	// site + database(different user)
	// database to matrix
	bool generate(string site, list<DATA_ITEM>& Matrix);
	bool generate(string site, map<string, list<DATA_ITEM> >& Matrix);

	void anomie(list<DATA_ITEM>& data, list<CResult>& result);

	void info();
};
#endif//_USER_PROFILE
