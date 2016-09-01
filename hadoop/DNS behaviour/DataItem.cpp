#include "DataItem.hpp"

void DATA_ITEM::info()
{
	cout<<"time:"<<strTime<<" user:"<<user<<" sip:"<<sip<<" hostname:"<<endl;
	vector<string>::iterator h = hostname.begin();
	while(h != hostname.end())
	{
		cout<<*h<<" ";
		h++;
	}

	cout<<endl;
}
