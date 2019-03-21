package problem;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.util.Map;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.stream.JsonReader;


public class GJson {
	
	public static String File2String(String FilePath) throws Exception
	{
        File file = new File(FilePath);
        FileReader reader = new FileReader(file);
        int fileLen = (int)file.length();
        char[] chars = new char[fileLen];
        reader.read(chars);
        String txt = String.valueOf(chars);
        
        return txt;
	}
	
	public static String File2String2(String FilePath) throws Exception
	{
        File file = new File(FilePath);
        InputStreamReader reader = new InputStreamReader(new FileInputStream(FilePath), "UTF-8");
        int fileLen = (int)file.length();
        char[] chars = new char[fileLen];
        int readlen = reader.read(chars);
        String txt = String.valueOf(chars, 0, readlen);
        
        return txt;
	}
	
	public static void File2String2(String FilePath, String content) throws Exception
	{
        File file = new File(FilePath);
        FileReader reader = new FileReader(file);
        int fileLen = (int)file.length();
        char[] chars = new char[fileLen];
        reader.read(chars);
        
        content.concat(new String(chars));
	}
	
	public static void String2File(String content, String FilePath) throws Exception
	{
		File file = new File(FilePath);
		FileWriter writer = new FileWriter(file);
		
		writer.write(content);
		writer.flush();
		
		writer.close();
	}
	
	public static Map File2Map(String FilePath) throws Exception
	{
		
		Gson gson = new Gson();
		String json = File2String2(FilePath);
		
		//System.out.println(json);
		Map map  = gson.fromJson(json, Map.class);
		
		return map;
	}
	
	public static Map File2Map_hdfs(String FilePath)
	{
		Gson gson = new Gson();
		oper_hdfs oh = new oper_hdfs();
		
		oh.initRemoteHdfs();
		
		String json = new String("null");
		try {
			json = oh.readFile(FilePath);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Map map  = gson.fromJson(json, Map.class);
		
		return map;
	}
	
	public static void Map2File(Map map, String FilePath) throws Exception
	{
		GsonBuilder gson = new GsonBuilder();
		String strJ = gson.serializeNulls().create().toJson(map);
		String2File(strJ, FilePath);
	}
	
	public static String Map2String(Map map)
	{
		GsonBuilder gson = new GsonBuilder();
		String strJ = gson.serializeNulls().create().toJson(map);
		
		return strJ;
	}

}
