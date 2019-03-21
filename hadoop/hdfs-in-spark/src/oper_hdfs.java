import java.io.IOException;
import java.io.PrintWriter;
import java.io.Serializable;
import java.io.StringWriter;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;

public class oper_hdfs implements Serializable{
	
	FileSystem remoteHDFS;
	boolean m_isOk = false;
	
	public void printException(Exception e)
	{
		System.out.println("exception");
		StringWriter sw=new StringWriter();
		e.printStackTrace(new PrintWriter(sw));
		System.out.println(sw.getBuffer().toString());
	}
	
	public void initRemoteHdfs()
	{
		Configuration conf=new Configuration();
		conf.set("fs.default.name", "hdfs://node1:9000");
		conf.set("fs.hdfs.impl", "org.apache.hadoop.hdfs.DistributedFileSystem");
		
		
		try{
		remoteHDFS=FileSystem.get(
				new URI("hdfs://node1:9000"),
				conf,
				"ubt");
		if(remoteHDFS != null)
			m_isOk = true;
		}catch(Exception e)
		{
			printException(e);
		}
	}
	
	public boolean isOk()
	{
		return m_isOk;
	}
	
	public void getFile()
	{
		try
		{
			Path src=new Path("/user/wzl/cleanFile2");
			Path dst=new Path("/home/ubt/wzl");
			remoteHDFS.copyToLocalFile(false, src, dst, true);
		}catch(Exception e)
		{
			printException(e);
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

}
