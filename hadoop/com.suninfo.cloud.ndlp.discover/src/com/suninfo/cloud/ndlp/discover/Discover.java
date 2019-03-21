package com.suninfo.cloud.ndlp.discover;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.hadoop.HadoopIllegalArgumentException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.ipc.RPC;
import org.apache.hadoop.ipc.Server;

import com.suninfo.cloud.ndlp.discover.client.Bizable;
import com.suninfo.cloud.ndlp.discover.client.DisListString;

public class Discover implements Bizable{
	static Server server = null;
	static Dispatcher dp = null;
	

	public DisListString discover(DisListString meta_path)
	{
		DisListString log = new DisListString();
		List<String> metaPath = new ArrayList<String>();
		List<String> logMap = new ArrayList<String>();
		if(meta_path.getString(metaPath))
		{
			try {
				if( dp.discover_file(metaPath, logMap) )
				{
					Iterator<String> iter = logMap.iterator();
					while(iter.hasNext())
					{
						log.insertString(iter.next());
					}
				}
				
				System.out.println("log size:" + logMap.size());
				
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		System.out.println("ret log size:"+ log.getSize());
		return log;
	}
	
	public void ResetDispatcher()
	{
		dp.uninit();
		dp = null;
	}
	
	public void closeServer()
	{
		if(dp != null)
			dp.uninit();
		if(server != null)
			server.stop();
	}

	public static void main(String[] args) throws HadoopIllegalArgumentException, IOException
	{
		dp = new Dispatcher();
		dp.init();
		
		Configuration conf = new Configuration();
		server = new RPC.Builder(conf).setProtocol(Bizable.class).setInstance(new Discover())
				.setBindAddress("node1").setPort(9527).build();
		
		server.start();
	}
}