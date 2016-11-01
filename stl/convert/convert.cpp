#include <sstream>
#include <string>
#include <iostream>

using namespace std;

template<class out_type, class in_type>
out_type convert(const in_type& t)
{
	stringstream stream;
	stream << t;
	out_type result;
	stream >> result;
	return result;
}



int main()
{
	double a = 1.23;
	string sa = convert<string>(a);

	sa+="4";

	float b = convert<float>(sa);

	cout<<"b:"<<b<<endl;
}
