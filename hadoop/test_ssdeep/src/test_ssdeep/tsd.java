package test_ssdeep;

import ssdeepWrapper.Jssdeep;

public class tsd {
	public static void main(String[] args)
	{
		if(args.length < 2)
		{
			System.out.println("tds file1 file2");
			return;
		}
		
		System.out.println(args[0] + " " + args[1]);
		Jssdeep jd = new Jssdeep();
		StringBuffer h1 = new StringBuffer();
		StringBuffer h2 = new StringBuffer();
		jd.fuzzy_hash_filename(args[0], h1);
		jd.fuzzy_hash_filename(args[1], h2);
		
		int c = jd.fuzzy_compare(h1.toString(), h2.toString());
		
		System.out.println("h1:" + h1);
		System.out.println("h2:" + h2);
		System.out.println("c:" + c);
	}

}
