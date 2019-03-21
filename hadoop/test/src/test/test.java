package test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class test {
	
	static void ch(Float a)
	{
		a.valueOf(20);
	}
	
	public static void main(String[] args)
	{
		/*
		Map<String, String> s = new HashMap<String, String>();
		

		if(null == s.get("abc"))
			System.out.println("no");
		else
			System.out.println("yes");
			*/
		
		//int a = Integer.valueOf("84687");
		//System.out.println("xxx" + a);
		
		//String[] tt = new String[3];
		
		//tt[7] = "hello";
		//System.out.println(tt[7]);
		
		/*
		Map<String, Object> s = new HashMap<String, Object>();
		List<Map> lm = new ArrayList<Map>();
		Map<String , String> a = new HashMap<String , String>();
		a.put("aa", "bb");
		Map<String , String> b = new HashMap<String , String>();
		a.put("bb", "cc");
		lm.add(a);
		lm.add(b);
		s.put("list", lm);
		
		List<Map> aaa = (List<Map>)s.get("list");
		*/
		
		Float b = new Float(1) ;
		ch(b);
		System.out.println(b);
		
	}

}
