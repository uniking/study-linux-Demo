package com.suninfo.cloud.ndlp.discover;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.util.List;

public class util {
	public static void traversal_diretory(String strPath, List<String> filelist)
	{
		File dir = new File(strPath);
		File[] files = dir.listFiles(); 
		if (files == null)
		return;
		for (int i = 0; i < files.length; i++)
		{
			if (files[i].isDirectory())
			{
				traversal_diretory(files[i].getAbsolutePath(), filelist);
			}
			else
			{
				String strFileName = files[i].getAbsolutePath().toLowerCase();
				//System.out.println(strFileName);
				filelist.add(files[i].getAbsolutePath());                   
			}
		}
	}

	public static String File2String(String FilePath) throws Exception
	{
		File file = new File(FilePath);
		InputStreamReader reader = new InputStreamReader(new FileInputStream(FilePath));
		int fileLen = (int)file.length();
		char[] chars = new char[fileLen];
		int readlen = reader.read(chars);
		String txt = String.valueOf(chars, 0, readlen);

		return txt;
	}

	public static boolean File2String(String FilePath, List<String> line) throws Exception
	{
		FileReader reader = new FileReader(FilePath);
		BufferedReader br = new BufferedReader(reader);
		while(true)
		{
			String l = br.readLine();
			if(l == null)
				break;
			if(l.length() > 0)
				line.add(l);
		}

		if(line.size() > 0)
			return true;
		return false;
	}
}