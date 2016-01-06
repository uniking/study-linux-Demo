#include <stdio.h>

#include <map>

using namespace std;

int main()
{
	//multimap 有序，可重复key
	multimap<int, int> testMultimap;
	testMultimap.insert(make_pair(1,1));
	testMultimap.insert(make_pair(2,2));
	testMultimap.insert(make_pair(2,3));
	testMultimap.insert(make_pair(3,4));

	multimap<int, int>::iterator index = testMultimap.begin();
	while(index != testMultimap.end())
	{
		printf("%d %d\n", (*index).first, (*index).second);
		index++;
	}
}
