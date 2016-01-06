#include <unistd.h>
#include <stdio.h>

#include <iostream>
using namespace std;
int main()
{
	int i=0;
	while(true)
	{
		cout<<i++<<endl;
		sleep(1);
	}
}
