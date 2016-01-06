#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <sstream>

using namespace boost::uuids;

int main()
{
	random_generator rgen;
	uuid u=rgen();
	std::stringstream ss;
	ss << u;
	std::string guid;
	ss >> guid;

	printf("create guid = %s\n", guid.c_str()); 

	return -1;
}
