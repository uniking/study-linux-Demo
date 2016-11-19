#include <bitset>
#include <iostream>
using namespace std;

int main()
{
	bitset<32> bs(3);
	cout<<"have bit==1 :"<<bs.any()<<endl;
	cout<<"1 size:"<<bs.count()<<endl;
	cout<<"bit 0 :"<<bs[0]<<endl;
	cout<<"bit 31 :"<<bs[31]<<endl;

	bs[31]=1;
	cout<<"bit 32 :"<<bs[31]<<endl;

	for(int i=31; i>=0; i--)
		cout<<bs[i];
	cout<<endl;

	return 0;
}