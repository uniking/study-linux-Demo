

import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.URI;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.LocatedFileStatus;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.RemoteIterator;

public class oper_hdfs {
	
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
	
	public void getFile(String s, String d)
	{
		try
		{
			Path src=new Path(s);
			Path dst=new Path(d);
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
		long fl = getLength(filepath);
		
		FSDataInputStream in=remoteHDFS.open(src);
		
		byte[] b = new byte[(int)fl];
	    int numBytes = 0;
	    String rStr = new String();
	    while ((numBytes = in.read(b)) > 0) {
	    	String t=new String(b);
	    	rStr += t;
	    }
		
	    in.close();
	    return rStr;
	}
	
	public long readFileBin(String filepath, byte[] buffer, long buf_len) throws IOException
	{
		Path src=new Path(filepath);
		FSDataInputStream in=remoteHDFS.open(src);
		
		byte[] b = new byte[1025];
		int numBytes = 0;
		int index = 0;
		while ((numBytes = in.read(b, 0, 1024)) > 0)
		{
			if(buf_len < index+numBytes)
				break;
			System.arraycopy(b, 0, buffer, index, numBytes);
			index += numBytes;
	    }
		
		in.close();
		
		return index;
	}
	
	public long readFile(String filepath, byte[] buf, long length) throws IOException
	{
		Path src=new Path(filepath);
		FSDataInputStream in=remoteHDFS.open(src);
		boolean isExit = false;
		
		byte[] b = new byte[1024];
	    int numBytes = 0;
	    int index = 0;
		while ((numBytes = in.read(b)) > 0)
		{
			int i=0;
			while(i<numBytes)
			{
				buf[index]=b[i];
				++index;
				++i;
				if(index >= length)
				{
					isExit=true;
					break;
				}
			}

			//System.arraycopy(b, 0, buf, index, numBytes);

			if(isExit)
			break;
		}

		in.close();
		return index;
	}
	
	public void traversal_diretory(String strPath, List<String> filelist) throws IllegalArgumentException, IOException
	{
		if(remoteHDFS.isDirectory(new Path(strPath)) == false)
		{
			System.out.println("traversal_diretory:" + strPath + " isn't directory");
			return;
		}
		
		RemoteIterator<LocatedFileStatus> fl = remoteHDFS.listFiles(new Path(strPath), false);
		while(fl.hasNext())
		{
			LocatedFileStatus lf = fl.next();
			filelist.add(lf.getPath().getPathWithoutSchemeAndAuthority(new Path(lf.getPath().toString())).toString());
			//System.out.println(lf.getPath().getPathWithoutSchemeAndAuthority(new Path(lf.getPath().toString())).toString());
		}
	}
	
	public boolean read_file(String strPath, List<String> line) throws IOException
	{
		String[] l = readFile(strPath).split("\n|\r\n|\r");
		for(String one : l)
			line.add(one);
		
		return true;
	}

}
