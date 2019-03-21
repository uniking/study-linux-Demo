package com.suninfo.cloud.ndlp.bayes;

import java.util.ArrayList;
import java.util.List;

public class JBayes {
	public native int IsAttractiveContent(List<String> words, BayesResult result);
	public native int LoadBayesModule(List<String> moduleContent);
	static {System.loadLibrary("jbayes");}
	
	public static void main(String[] args)
	{
		JBayes jb = new JBayes();
		BayesResult result = new BayesResult();
		
		List<String> words = new ArrayList<String>();
		words.add("hello");
		words.add("world");
		words.add("welcome");
		words.add("china");
		jb.IsAttractiveContent(words, result);
		System.out.println(result.m_uid + " " + result.m_className + " " + result.m_level + " " + result.m_Similarity);
		System.out.println("exit");
	}
}