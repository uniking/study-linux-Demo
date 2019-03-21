package com.suninfo.cloud.ndlp.bayes;

import java.util.List;

public class BayesResult {
	public String m_uid;
	public String m_level;
	public String m_className;
	public float m_Similarity;
	public List<String> m_keyWords;
	
	public void init_data(String uid, String level, String className, String Similarity)
	{
		m_uid = uid;
		m_level = level;
		m_className = className;
		m_Similarity = Float.valueOf(Similarity);
	}
	
	public void SetUid(String uid)
	{
		m_uid = uid;
	}
	
	public void SetLevel(String level)
	{
		m_level = level;
	}
	
	public void SetClassName(String className)
	{
		m_className = className;
	}
	
	public void SetSimilarity(Float Similarity)
	{
		m_Similarity = Similarity;
	}
	
	public void SetKeyWords(List<String> keyWords)
	{
		m_keyWords = keyWords;
	}

}
