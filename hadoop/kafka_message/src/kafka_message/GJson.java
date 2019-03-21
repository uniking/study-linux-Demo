package kafka_message;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.util.Map;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class GJson {
	
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
	
	public static Map File2Map(String FilePath) throws Exception
	{

		Gson gson = new Gson();
		String json = File2String2(FilePath);

		//System.out.println(json);
		Map map  = gson.fromJson(json, Map.class);

		return map;
	}
	
	public static void String2File(String content, String FilePath) throws Exception
	{
		File file = new File(FilePath);
		FileWriter writer = new FileWriter(file);

		writer.write(content);
		writer.flush();

		writer.close();
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
	
	public static Map String2Map(String content) throws Exception
	{

		Gson gson = new Gson();
		Map map  = gson.fromJson(content, Map.class);

		return map;
	}

}
