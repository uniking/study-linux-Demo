package test_opfile;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;


public class opfile {
	
	public static String read_file(String FilePath) throws Exception
	{
        File file = new File(FilePath);
        FileReader reader = new FileReader(file);
        int fileLen = (int)file.length();
        char[] chars = new char[fileLen];
        int readlen = reader.read(chars);
        String txt = String.valueOf(chars);
        System.out.println(txt.length()+" "+txt);
        
        return txt;
	}
	
	public static String File2String2(String FilePath) throws Exception
	{
        File file = new File(FilePath);
        InputStreamReader reader = new InputStreamReader(new FileInputStream(FilePath));
        int fileLen = (int)file.length();
        char[] chars = new char[fileLen];
        int readlen = reader.read(chars);
        String txt = String.valueOf(chars, 0, readlen);
        
        return txt;
	}
	
	public static String File2String3(String FilePath) throws Exception
	{
        File file = new File(FilePath);
        FileInputStream fis = new FileInputStream(FilePath);
        int filelen = (int)file.length();
        byte[] buf = new byte[filelen];
        int readlen = fis.read(buf);
        
        String txt = new String(buf);
        int slen = txt.length();
        return txt;
	}
	
	public static void write_file() throws Exception
	{
		File file = new File("f:\\2.txt");
		FileWriter writer = new FileWriter(file);
		
		writer.write("hello write to java");
		writer.flush();
		
		writer.close();
	}
	
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
	
	public static void main(String[] args) throws Exception
	{
		//read_file();
		//write_file();
		
		/*
		List<String> filelist = new ArrayList<String>();
		traversal_diretory("f:\\temp", filelist);
		
		Iterator iter = filelist.iterator();
		while(iter.hasNext())
			System.out.println(iter.next());
			*/
		
		//String bit = File2String2("f:\\jssdeep.jar");
		//String bit = read_file("f:\\jssdeep.jar");
		String bit = File2String3("f:\\jssdeep.jar");
		System.out.println(bit.length() + " " + bit);
	}
}
