package ssdeepWrapper;

public class Jssdeep {
	public native int fuzzy_hash_filename(String filename, StringBuffer result);
	public native int fuzzy_hash_buf(byte[] buf, long length, StringBuffer result);
	public native int fuzzy_compare(String sig1, String sig2);

	static {System.loadLibrary("jfuzzy");}

	/*
	public static void main(String[] args)
	{
		Jssdeep jd = new Jssdeep();
		StringBuffer s1 = new StringBuffer();
		jd.fuzzy_hash_filename("/root/wzl/ssdeep/libjfuzzy.so", s1);
		System.out.println(s1);
		StringBuffer s2 = new StringBuffer();
		jd.fuzzy_hash_filename("/root/wzl/ssdeep/Jssdeep.java", s2);
		System.out.println(s2);

		int i = jd.fuzzy_compare(s1.toString(), s2.toString());	
		System.out.println(i);
		
		
		
				Jssdeep jsd = new Jssdeep();
		StringBuffer result = new StringBuffer();
		byte[] buf = new byte[1024];
		int i=0;
		while(i<1000)
		{
			buf[i]=(byte)i;
			++i;
		}
		long length = 1000;
		jsd.fuzzy_hash_buf(buf, length, result);
		System.out.println(result);
	}
	*/
}