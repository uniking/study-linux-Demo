#include "CLocalClient.hpp"
@include "CRemoteClient.hpp"

// one socket, one local client

map<int, CLocalClient*> dispatch; //<socket, client>

CGoodsPool g_s2c_pool; //server to client
CGoodsPool g_c2s_pool; //client to server

int main(int argn, char* argv[])
{
	CRemoteClient rc;
	if(rc.connect_remote_server("192.168.199.204", 8800) != 0)
	{
		cout<<"connect remote server error"<<endl
		return 1;
	}

	rc.run();

	return 0;
}
