import java.util.List;

import org.apache.hadoop.io.ArrayWritable;

public interface Bizable {
	public static final long versionID = 1L;
	void close();
	public String sysHi(String name);
	public int add(int a, int b);
	public void Test1(String name, StringBuffer outBuffer); //can't
	public List<String> Test2(); //can't
	
	public void setname(myObject mo);
	public myObject getMO(myObject mo);
	public void setThree(myObject mo);
	public ArrayWritable getThree(myObject mo);
	
	public ContainMyObj getCMO();
}
