package com.suninfo.cloud.ndlp.discover;

import java.util.HashMap;
import java.util.Map;

public class Model {
	int m_filenumber;
	Float m_threshold;
	String m_modulename;
	String m_uid;
	String m_level;
	boolean m_positive;
	Float[] m_thresholdRange = new Float[2];
	Map<String, Float> m_wordPro = new HashMap<String, Float>();

	Model()
	{
		m_filenumber = 0;
		m_threshold = new Float(0);
	}
	
	int loadmodule()
	{
		return 0;
	}
	
	int savemodule()
	{
		return 0;
	}
	
	Map<String, Float> getwordpro()
	{
		return m_wordPro;
	}
	String getmodulename()
	{
		return m_modulename;
	}
	
	String getuid()
	{
		return m_uid;
	}
	
	String getlevel()
	{
		return m_level;
	}
	int getthresholdrange(Float[] buffer)
	{
		buffer[0] = m_thresholdRange[0];
		buffer[1] = m_thresholdRange[1];
	
		return 0;
	}
	Float getthreshold()
	{
		return m_threshold;
	}
	int getfilenumber()
	{
		return m_filenumber;
	}
	//////////////////////////////////////////////////////////////////////////
	int setwordpro(Map<String, Float> wordpro)
	{
		m_wordPro.putAll(wordpro);
	
		return 0;
	}
	int setmodulename(String modulename)
	{
		m_modulename = modulename;
		return 0;
	}
	int setthresholdrange(Float[] buffer)
	{
		m_thresholdRange[0] = buffer[0];
		m_thresholdRange[1] = buffer[1];
		return 0;
	}
	int setthreshold(Float threshold)
	{
		m_threshold = threshold;
		return 0;
	}
	int setfilenumber(int filenumber)
	{
		m_filenumber = filenumber;
		return 0;
	}
	boolean is_positive()
	{
	    return m_positive;
	}
	void set_positive(boolean positive)
	{
	    m_positive = positive;
	}
}
