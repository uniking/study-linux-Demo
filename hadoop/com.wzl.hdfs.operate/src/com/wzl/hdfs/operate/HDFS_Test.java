package com.wzl.hdfs.operate;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.ContentSummary;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.LocatedFileStatus;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.RemoteIterator;

public class HDFS_Test {
	
	static FileSystem remoteHDFS;
	
	private static void lsDir(Path f) throws FileNotFoundException, IOException
	{
		FileStatus files[];
		files = remoteHDFS.listStatus(f);
		
		for(FileStatus file:files)
		{
			System.out.println(file.getPath());
		}
	}
	
	private static void printException(Exception e)
	{
		System.out.println("exception");
		StringWriter sw=new StringWriter();
		e.printStackTrace(new PrintWriter(sw));
		System.out.println(sw.getBuffer().toString());
	}
	
	private static void initRemoteHdfs()
	{
		Configuration conf=new Configuration();
		conf.set("fs.default.name", "hdfs://node1:9000");
		conf.set("fs.hdfs.impl", "org.apache.hadoop.hdfs.DistributedFileSystem");
		
		
		try{
		remoteHDFS=FileSystem.get(
				new URI("hdfs://node1:9000"),
				conf,
				"ubt");
		}catch(Exception e)
		{
			printException(e);
		}
	}
	
	private static void copyToHdfs(){
			Path src=new Path("f:\\abcd.txt");
			Path dst=new Path("/");
			
			try
			{
				remoteHDFS.copyFromLocalFile(src, dst);
				lsDir(dst);
			}catch(Exception e)
			{
				printException(e);
			}
		
	}
	
	private static void createDir()
	{
		try
		{
			remoteHDFS.mkdirs(new Path("/user/ubt/mydir"));
			lsDir(new Path("/user/ubt"));
		}catch(Exception e)
		{
			printException(e);
		}
	}
	
	private static void deleteFile()
	{
		try
		{
			remoteHDFS.delete(new Path("/abcd.txt"), false);
			lsDir(new Path("/"));
		}catch(Exception e)
		{
			printException(e);
		}
	}
	
	private static void getFile()
	{
		try
		{
			Path src=new Path("/abcd.txt");
			Path dst=new Path("f:\\");
			remoteHDFS.copyToLocalFile(false, src, dst, true);
		}catch(Exception e)
		{
			printException(e);
		}
	}
	
	
	public String readFile(String filepath) throws IOException
	{
		Path src=new Path(filepath);
		FSDataInputStream in=remoteHDFS.open(src);
		
		byte[] b = new byte[1025];
		int numBytes = 0;
		String rStr = new String();
		while ((numBytes = in.read(b, 0, 1024)) > 0)
		{
				//out.write(b, 0, numBytes);
			b[numBytes] = 0;
			String t= new String(b);
			//System.out.println(t);
			rStr += t;
	    }
		
		long fl = getLength(filepath);
		
		in.close();
		return rStr.substring(0, (int)fl);
	}
	
	public void traversal_diretory(String strPath, List<String> filelist) throws IllegalArgumentException, IOException
	{
		if(remoteHDFS.isDirectory(new Path(strPath)) == false)
		{
			System.out.println("traversal_diretory isn't directory");
			return;
		}
		
		RemoteIterator<LocatedFileStatus> fl = remoteHDFS.listFiles(new Path(strPath), false);
		while(fl.hasNext())
		{
			LocatedFileStatus lf = fl.next();
			//System.out.println(lf.getPath().getParent() +"/"+ lf.getPath().getName());
			//System.out.println(lf.getPath().toString());
			//System.out.println(lf.getPath().toUri());
			filelist.add(lf.getPath().getPathWithoutSchemeAndAuthority(new Path(lf.getPath().toString())).toString());
			System.out.println(lf.getPath().getPathWithoutSchemeAndAuthority(new Path(lf.getPath().toString())).toString());
		}
	}
	
	public long getLength(String filepath)
	{
		long flen=0;
		try
		{
			Path src=new Path(filepath);		
			flen = remoteHDFS.getFileStatus(src).getLen();
		}catch(Exception e)
		{
			printException(e);
		}
		
		return flen;
	}
	

	
	public static void main(String[] args) throws Exception
	{
		HDFS_Test ht = new HDFS_Test();
		ht.initRemoteHdfs();
		//copyToHdfs();
		//createDir();
		//getFile();
		//deleteFile();
		//readFile("/user/wzl/cleanFile2");
		//readFile("/user/wzl/kafka");
		//readFile("/user/wzl/run");
		
		//List<String> filelist = new ArrayList<String>();
		//ht.traversal_diretory("/user/wzl/policy", filelist);
		
		String ka = ht.readFile("/user/wzl/kafka");
		System.out.println(ka);
		
		System.out.println("exit");
	}
}
