package test_json;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class test_json {
	
	public static void test2()
	{
		Gson gson = new Gson();
		String json = "{\"name\":\"guolicheng\",\"id\":123456,\"date\":\"2013-4-13 12:36:54\"}";
		//new һ��Product����
		
		
		Product product = new Product();
		//��һ��json�ַ���ת��Ϊjava����
		product  = gson.fromJson(json, Product.class);
		
		
		
		//���
		System.out.println(product.getName());
		System.out.println(product.getId());
		//System.out.println("Date:" + product.getDate());
	}
	
	public static void test1()
	{
		Gson gson = new Gson();
		String json = "{\"name\":\"中国\",\"id\":123456,\"date\":\"2013-4-13 12:36:54\"}";
		//new һ��Product����
		
		
		//Product product = new Product();
		//��һ��json�ַ���ת��Ϊjava����
		Map map  = gson.fromJson(json, Map.class);
		
		//���
		System.out.println(map.get("name"));
		System.out.println(map.get("id"));
	}
	
	
	public static void test3()
	{
		Gson gson = new Gson();
		String json = "{\"name\":\"guolicheng\",\"id\":123456,\"date\":\"2013-4-13 12:36:54\",\"list\":[{\"name1\":\"abc\"},{\"name1\":\"def\"}]}";
		//new һ��Product����
		
		
		//Product product = new Product();
		//��һ��json�ַ���ת��Ϊjava����
		Map map  = gson.fromJson(json, Map.class);
		
		//���
		List lt = (List)map.get("list");
		//map.get(key)
		//String sn = lt.get("name1");
		Iterator iter = lt.iterator();
		while(iter.hasNext())
		{
			Map om = (Map)iter.next();
			System.out.println(om.get("name1"));
		}
		
		
		
		//lt[1]
	}
	
	public static void test4()
	{
		Map map = new HashMap<String, Object>();
		map.put("name", "guolicheng");
		
		Map map1 = new HashMap<String, Object>();
		map1.put("name1", "abc");
		Map map2 = new HashMap<String, Object>();
		map2.put("name1", 128);
		
		List lmap = new ArrayList<Map>();
		lmap.add(map1);
		lmap.add(map2);
		
		map.put("list", lmap);
		
		GsonBuilder gson = new GsonBuilder();
		String strJ = gson.serializeNulls().create().toJson(map);
		
		
		
		System.out.println(strJ);
	}
	
	public static void main(String[] args) throws Exception
	{
		//test1();
		test4();
	}


}
