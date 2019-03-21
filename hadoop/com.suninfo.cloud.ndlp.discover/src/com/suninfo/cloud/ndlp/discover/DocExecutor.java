package com.suninfo.cloud.ndlp.discover;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.TimeZone;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class DocExecutor {
	Regular m_regular = new Regular();

boolean match_String(String rule, String buffer, StringBuffer Content)
{
	boolean bRtn;
	if(buffer.length() == 0)
		return false;
	bRtn = m_regular.ExtractDataOnce(rule, buffer, Content);
	return bRtn;
}

boolean is_doc_file(PolicyObject policy, boolean bDoc, Map log)
{
	return policy.m_doc_is_doc == bDoc ? true:false;
}

boolean is_doc_manager(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_manager);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_company(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_company);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_title(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_title);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_subject(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_subject);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_author(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_author);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_keywords(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_keywords);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_description(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_description);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_lastModifieBy(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_lastModifieBy);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

long String_to_time(String timeStr) throws ParseException
{
	SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
	df.setTimeZone(TimeZone.getTimeZone("UTC")) ;
	return df.parse(timeStr).getTime();
	//return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(timeStr).getTime();
}

boolean is_doc_modified_time(PolicyObject policy, String buffer, Map log) throws ParseException
{
	if(0 == buffer.length())
	{// there is not modified time attribute.
		return false;
	}

	boolean bRtn = false;
	long f_t = String_to_time(buffer);
	switch(policy.m_doc_modified_time_type)
	{
		case t_before:
			{
				if(policy.m_doc_modified_time_first > f_t)
				{
					bRtn = true;
				}
			}
			break;
		case t_after:
			{
				if(policy.m_doc_modified_time_first < f_t)
				{
					bRtn = true;
				}
			}
			break;
		case t_between:
			if(policy.m_doc_modified_time_first < f_t &&
			policy.m_doc_modified_time_second > f_t)
			{
				bRtn = true;
			}
			break;
		case t_last:
			{
				long c_timer = new Date().getTime();
				if(c_timer >= f_t)
				{
					long dif = c_timer - f_t;
					dif = dif/60/60/24;
					if(dif <= policy.m_doc_modified_time_first)
						bRtn = true;
				}
			}
			break;
		default:
		;	
	}
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_modified_time);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("first_time", policy.m_doc_modified_time_first);
		switch(policy.m_doc_modified_time_type)
		{
			case t_before:
				log_tmp.put("type", "before");
				break;
			case t_after:
				log_tmp.put("type", "after");
				break;
			case t_between:
				log_tmp.put("type", "between");
				log_tmp.put("second_time", policy.m_doc_modified_time_second);
				break;
			case t_last:
				log_tmp.put("type", "last");
				break;
			default:
				;
		}
		
		log_tmp.put("doc_modified_time", buffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_category(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_category);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_template(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_template);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_header(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_header);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_footer(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_footer);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_encrypted(PolicyObject policy, boolean bEncrypted, Map log)
{
	return policy.m_doc_is_encrypted == bEncrypted? true:false;
}

boolean is_doc_extension(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_extension);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}

boolean is_doc_type(PolicyObject policy, String buffer, Map log)
{
	boolean bRtn = false;
	StringBuffer exBuffer = new StringBuffer();
	bRtn = match_String(policy.m_regular, buffer, exBuffer);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_doc_type);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("doc_rule", policy.m_regular);
		log_tmp.put("doc_attr", exBuffer);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean is_doc_file_size(PolicyObject policy, long file_size, Map log)
{
	boolean bRtn = false;

	switch(policy.m_file_size_type)
	{
		case n_less_then:
			if(file_size < policy.m_file_size_first)
				bRtn = true;
			break;
		case n_great_then:
			if(file_size > policy.m_file_size_first)
				bRtn = true;
			break;
		case n_between:
		if(policy.m_file_size_first <= file_size &&
			policy.m_file_size_second >= file_size)
				bRtn = true;
			break;
		default:
		;
	}

	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_file_size);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("first_size", policy.m_file_size_first);
		switch(policy.m_file_size_type)
		{
			case n_less_then:
				log_tmp.put("type", "less then");
				break;
			case n_great_then:
				log_tmp.put("type", "great then");
				break;
			case n_between:
				log_tmp.put("type", "between");
				log_tmp.put("second_size", policy.m_file_size_second);
				break;
			default:
				;
		}
		log_tmp.put("doc_file_size", file_size);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
}
