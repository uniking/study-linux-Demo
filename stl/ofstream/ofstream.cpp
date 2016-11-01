#include <fstream>

using namespace std;

int main()
{
	ofstream f("123.txt");

	f<<"hello ";
	f<<"world";
	f<<"\n";
	f<<"haha";

	f.close();

	return 0;
}
