import java.io.IOException;
import java.net.InetSocketAddress;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.ipc.ProtocolProxy;
import org.apache.hadoop.ipc.RPC;

public class wc_client {
	public static void main(String[] args) throws IOException
	{
		if(args.length < 1)
		{
			System.out.println("RpcClient xxxx");
			return;
		}
		
		ProtocolProxy<Bizable> proxy = RPC.getProtocolProxy(Bizable.class, 1L, new InetSocketAddress("node1",9527), new Configuration());
		String result =  proxy.getProxy().wc("test");
		System.out.println(result);
	}
}