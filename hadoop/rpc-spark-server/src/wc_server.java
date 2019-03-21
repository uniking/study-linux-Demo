import java.io.IOException;

import org.apache.hadoop.HadoopIllegalArgumentException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.ipc.RPC;
import org.apache.hadoop.ipc.Server;


// run: spark-submit --name wc_server --executor-memory 512m --total-executor-cores 3 --master spark://node1:7077 wc_server.jar
public class wc_server implements Bizable{
	
	static spark_wc swc = new spark_wc();
	

	public String wc(String filepath)
	{
		
		String result = new String("xxxx");
		
		try {
			System.out.println("---to spark wc---");
			result = swc.wc("hdfs://node1:9000/user/ubt/WordCount/novel/novel.txt");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return result;
	}
	
	public static void main(String[] args) throws HadoopIllegalArgumentException, IOException
	{
		swc.init();
		Configuration conf = new Configuration();
		Server server = new RPC.Builder(conf).setProtocol(Bizable.class).setInstance(new wc_server())
				.setBindAddress("node1").setPort(9527).build();
		
		server.start();
		
		//swc.uninit();
	}
}
