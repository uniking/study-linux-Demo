import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.Iterator;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.ipc.ProtocolProxy;
import org.apache.hadoop.ipc.RPC;

public class RpcClient {
	public static void main(String[] args) throws IOException 
	{
		if(args.length < 1)
		{
			System.out.println("RpcClient xxxx");
			return;
		}
		
		ProtocolProxy<Bizable> proxy = RPC.getProtocolProxy(Bizable.class, 1L, new InetSocketAddress("node1",9727), new Configuration());
		//String result =  proxy.getProxy().sysHi(args[0]);
		//System.out.println(result);
		
		//System.out.println("add 10+11");
		//int r = proxy.getProxy().add(10, 11);
		//System.out.println("=" + r);
		
		//StringBuffer outBuffer = new StringBuffer();
		//proxy.getProxy().Test1("abcd", outBuffer);
		
		//List<String> name = proxy.getProxy().Test2();
		
		/*
		myObject mo = new myObject();
		mo.setThree();
		proxy.getProxy().setname(mo);
		System.out.println(mo.getName());
		
		mo = proxy.getProxy().getMO(mo);
		System.out.println(mo.getName());
		ArrayWritable aw = mo.getThree();
		Writable[] wv = aw.get();
		System.out.println(String.valueOf(wv.length));
		
		List<String> lc = mo.getCourse();
		Iterator<String> iter = lc.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
			*/
		

		//try{
		//	proxy.getProxy().close();
		//}catch(Exception e)
		//{
		//	
		//}
		
		//proxy.getProxy().setThree(mo);
		//ArrayWritable aw = proxy.getProxy().getThree(mo);
		//Writable[] wv = aw.get();
		//System.out.println(wv.length);
		
		
		ContainMyObj  cmo = proxy.getProxy().getCMO();
		myObject mo = cmo.get_mo(0);
		System.out.println(mo.getName());
		
		
		System.out.println("client exit");
	}
}