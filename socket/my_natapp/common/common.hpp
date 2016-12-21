typdef struct _transfer_data
{
	int nat_socket; //socket in nat server, default 0
	int user_socket; //socket in user server, default 0
	long data_length;
	char* data[1];
}transfer_data, *ptransfer_data;
