import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.HadoopIllegalArgumentException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.ipc.RPC;
import org.apache.hadoop.ipc.Server;

public class RpcServer implements Bizable{
	
	static Server server = null;
	
	public String sysHi(String name)
	{
		//do something
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return "Hi~" + name;
	}
	
	public int add(int a, int b)
	{
		return a+b;
	}
	
	public void Test1(String name, StringBuffer outBuffer)
	{
		outBuffer.append(name);
	}
	
	public void close()
	{
		server.stop();
	}
	
	public List<String> Test2()
	{
		List<String> name = new ArrayList<String>();
		name.add("abc");
		name.add("def");
		
		return name;
	}
	
	public void setname(myObject mo)
	{
		System.out.println("s:setname:server");
		mo.setName("server");
	}
	
	public myObject getMO(myObject mo)
	{
		myObject new_mo = new myObject();
		new_mo.setName("xServer");
		new_mo.setThree();
		new_mo.setCourse();
		return new_mo;
	}
	
	public void setThree(myObject mo)
	{
		mo.setThree();
	}
	public ArrayWritable getThree(myObject mo)
	{
		return mo.getThree();
	}
	
	
	public ContainMyObj getCMO()
	{
		ContainMyObj one = new ContainMyObj();
		myObject mo = new myObject();
		one.init_cmo(mo);
		
		return one;
	}
	
	public static void main(String[] args) throws HadoopIllegalArgumentException, IOException
	{
		Configuration conf = new Configuration();
		server = new RPC.Builder(conf).setProtocol(Bizable.class).setInstance(new RpcServer())
				.setBindAddress("node1").setPort(9727).build();
		server.start();
	}
}