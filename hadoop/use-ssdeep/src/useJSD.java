import ssdeepWrapper.Jssdeep;

public class useJSD {
	
	public static void main(String[] args)
	{
		Jssdeep jsd = new Jssdeep();
		StringBuffer result = new StringBuffer();
		jsd.fuzzy_hash_filename("/home/wzl/abc", result);
	}
}
