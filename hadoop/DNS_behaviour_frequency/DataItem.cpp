#include "DataItem.hpp"

void DATA_ITEM::info()
{
	cout<<"sip:"<<sip<<endl;
	map<string, long>::iterator h = hostname.begin();
	while(h != hostname.end())
	{
		cout<<h->first<<" "<<h->second<<endl;
		h++;
	}

	map<string, map<string, long>>::iterator up = hostname_plot.begin();
	while(up != hostname_plot.end())
	{
		map<string, long>::iterator tc = up->second.begin();
		while(tc != up->second.end())
		{
			cout<<up->first<<" "<<tc->first<<" "<<tc->second<<endl;
			tc++;
		}
		up++;
	}

	cout<<endl;
}
