package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class Regular {
	
	boolean ExtractDataOnce(String rule, String buffer, StringBuffer Content)
	{
		boolean bRtn = false;
		Pattern pattern;
		int group;
		String GROUP = "sxis.bj.fdeamond..group";
		
		group = 0;
		pattern = Pattern.compile(rule);

		Matcher matcher = pattern.matcher(buffer);

		if(matcher.find())
		{
			System.out.println(matcher.group(group));
			Content.append(matcher.group(group));
			bRtn = true;
		}
		
		return bRtn;
	}

	boolean ExtractAllData(PolicyObject policy, String fileBuffer, List<String> Content, Map log)
	{
		boolean bRtn = false;
		if (fileBuffer==null || policy.m_regular.length()==0 || fileBuffer.length()==0)
		{
	        System.out.println("m_regular == 0 or fileBuffer==0");
			return false;
		}
		Content.clear();
		
		Pattern pattern;
		int group;
		String GROUP = "sxis.bj.fdeamond..group";
		
		group = 0;
		pattern = Pattern.compile(policy.m_regular);

		Matcher matcher = pattern.matcher(fileBuffer);

		while (matcher.find())
		{
			System.out.println(matcher.group(group));
			Content.add(matcher.group(group));
		}

    	Map reg_log = new HashMap<String, Object>();
		if (0 != Content.size())
		{
			StringBuffer out_buffer = new StringBuffer();
			for(String one: Content)
				out_buffer.append(one).append(":");
			
			reg_log.put("rule", policy.m_regular);
			log.put("out_buffer", out_buffer);
			reg_log.put("p_exe", _POLICY_TO_EXE.pe_regedit);
			reg_log.put("policy_name", policy.m_Name);

			log.put("exe_list", reg_log);
			bRtn = true;
		}

		return bRtn;
	}
}
