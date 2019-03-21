package com.suninfo.cloud.ndlp.discover;

import java.util.HashMap;
import java.util.Map;

public class FileModel extends Model{
	String m_modulepath;
	
	FileModel(String modulepath)
	{
		m_modulepath = modulepath;
	}


	int JsonRead()
	{
		Map json_object = GJson.File2Map_hdfs(m_modulepath);

		//String tmp_str;
		m_level = (String)json_object.get("level");
		m_modulename = (String)json_object.get("modulname");
		m_thresholdRange[0] = (Float)json_object.get("lowThreshold");
		m_thresholdRange[1] = (Float)json_object.get("highThreshold");
		m_threshold = (Float)json_object.get("threshold");
	   m_filenumber = (int)json_object.get("fileNum");
	   m_positive = (boolean)json_object.get("positive");
		
		m_uid = (String)json_object.get("uid");
		
		String tmp =  (String)json_object.get("wordPro");
		
		String[] wp = tmp.split("\\n|\\r\\n");
		for(String one : wp)
		{
			String[] kw = one.split("\\:");
			if(kw.length == 2)
				m_wordPro.put(kw[0], Float.valueOf( kw[1]));
		}
		
		return 0;
	}

	int loadmodule()
	{
		JsonRead();
		return 0;
	}

	int JsonWrite()
	{

		Map root = new HashMap<String, Object>();
		root.put("modulname",m_modulename);
		root.put("lowThreshold", m_thresholdRange[0]);
		root.put("highThreshold", m_thresholdRange[1]);
		root.put("threshold", m_threshold);
	   root.put("fileNum", m_filenumber);
	   root.put("positive", m_positive);

	   String jContent = GJson.Map2String(root);
	   
	   //write to m_modulepath

		return 0;
	}

	int savemodule()
	{
		JsonWrite();
		return 0;
	}
}
