#include <stdio.h>
#include <boost/filesystem.hpp>

int main()
{
	boost::filesystem::path bpath;

	// ".filetotext/abc/Images/Common/"
	bpath = boost::filesystem::path(".filetotext/abc/Images/Common/test");

	
	if ( !boost::filesystem::is_directory(bpath)  )
	{
		printf("is not dir file\n");

		//创建单级目录，多级目录会崩溃
		//if(!boost::filesystem::create_directory(bpath))
		//	return 1;

		if(!boost::filesystem::create_directories(bpath))
			return 1;
	}
	else
	{
		printf("is dir file\n");
	}

	
	return 0;
}
