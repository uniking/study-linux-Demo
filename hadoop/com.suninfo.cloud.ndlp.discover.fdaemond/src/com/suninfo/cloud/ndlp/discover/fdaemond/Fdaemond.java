package com.suninfo.cloud.ndlp.discover.fdaemond;


import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.ipc.ProtocolProxy;
import org.apache.hadoop.ipc.RPC;

import com.suninfo.cloud.ndlp.discover.client.Bizable;
import com.suninfo.cloud.ndlp.discover.client.DisListString;

public class Fdaemond
{
	
	static void test1(ProtocolProxy<Bizable> proxy)
	{
		for(int i=0; i<1; ++i)
		{
			DisListString meta_path = new DisListString();
			meta_path.insertString("/user/wzl/file.meta");
			meta_path.insertString("/user/wzl/file2.meta");
			meta_path.insertString("/user/wzl/file3.meta");
			meta_path.insertString("/user/wzl/file4.meta");
			meta_path.insertString("/user/wzl/file5.meta");
			DisListString retLog = proxy.getProxy().discover(meta_path);
			
			List<String> llog = new ArrayList<String>();
			retLog.getString(llog);
			for(String one:llog)
			{
				System.out.println("log:" + one);
			}
		}
	}
	
	static void test2(ProtocolProxy<Bizable> proxy, List<String> topics)
	{
		FileConsumer fc = new FileConsumer("1", topics);
		LogProducer lp = new LogProducer("ndlp_log");
		
		while(true)
		{
			DisListString meta_path = new DisListString();
			fc.getMessage(meta_path, 2000);
			System.out.println("meta_path count:" + meta_path.getSize());
			DisListString retLog = proxy.getProxy().discover(meta_path);
			
			List<String> llog = new ArrayList<String>();
			retLog.getString(llog);
			for(String one:llog)
			{
				//System.out.println("log:" + one);
				lp.SendLog(one);
			}
		}
	}
	
	static void close(ProtocolProxy<Bizable> proxy)
	{
		try{
			proxy.getProxy().closeServer();
		}catch(Exception e)
		{
			
		}
		
	}
	
	public static void main(String[] args) throws IOException
	{
		if(args.length < 1)
		{
			System.out.println("RpcClient topcis ...");
			return;
		}

		ProtocolProxy<Bizable> proxy = RPC.getProtocolProxy(Bizable.class, 1L, new InetSocketAddress("node1",9527), new Configuration());
		//test1(proxy);
		
		
		
		
		List<String> topics = new ArrayList<String>();
		for(String one : args)
		{
			topics.add(one);
		}
		test2(proxy, topics);
		

		
		//close(proxy);
		
		
		System.out.println("fdaemond exit");
	}
}
