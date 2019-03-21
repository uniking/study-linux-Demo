package problem;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class debug_problem {
	
	static void test1()
	{
		Map<String, Object> mm = new HashMap<String, Object>();
		
		mm.put("123", "456");
		mm.put("234", "567");
		
		Iterator<String> k = mm.keySet().iterator();
		while(k.hasNext())
		{
			String kk = k.next();
			System.out.println(kk + mm.get(kk));
		}
	}
	
	static void test2()
	{
		List<List<String>> a = new ArrayList<List<String>>();
		
		List<String> a1 = new ArrayList<String>();
		a1.add("a1");
		List<String> a2 = new ArrayList<String>();
		a1.add("a2");
		
		a.add(a1);
		a.add(a2);
	}
	
	public static void main(String[] args)
	{
		//Map jj = GJson.File2Map_hdfs("/user/wzl/policy/12");
		//System.out.println(jj);
		
		/*
		String ph = "13299999999";
		char[] cPN = new char[32];
		System.arraycopy(ph.toCharArray(), 0, cPN, 0, 3);
		cPN[3] = 0;
		
		int iPN = Integer.parseInt(new String().valueOf(cPN, 0, 3));
		
		System.out.println(cPN);
		*/
		
		
		test2();
		
	}
}
