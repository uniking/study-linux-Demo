package file_daemond;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.ipc.ProtocolProxy;
import org.apache.hadoop.ipc.RPC;

public class fdaemond {
	public static void main(String[] args) throws IOException
	{
		if(args.length < 1)
		{
			System.out.println("RpcClient xxxx");
			return;
		}
		
		//List<String> meta_path = new ArrayList<String>();
		//List<Map> log = new ArrayList<Map>();
		
		//meta_path.add(new String("/user/wzl/kafka"));
		
		ProtocolProxy<Bizable> proxy = RPC.getProtocolProxy(Bizable.class, 1L, new InetSocketAddress("node1",9527), new Configuration());
		

		
		
		//boolean result =  proxy.getProxy().discover(meta_path, log);
		//System.out.println(result);
		
		proxy.getProxy().closeServer();
	}
}