#include "DataItem.hpp"

void DATA_ITEM::info()
{
	cout<<"user:"<<user<<" sip:"<<sip<<" hostname:";
	vector<string>::iterator h = hostname.begin();
	while(h != hostname.end())
	{
		cout<<*h<<" ";
		h++;
	}

	cout<<endl;
}
