#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main()
{
	ifstream infile("1.txt");
	if(!infile)
		cout<<"not exit 1.txt\n";
	else
		cout<<"1 ok\n";

	ifstream infile2("2.txt");
	if(!infile2.is_open())
		cout<<"not exit 2.txt\n";
	else
		cout<<"2 ok\n";
}
