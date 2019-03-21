package com.suninfo.cloud.ndlp.discover;

import java.util.HashMap;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class MetaFile {
	
	Regular m_regular = new Regular();

boolean IsValidProtocol(PolicyObject policy, String data, Map log)
{
	if (policy.m_regular.length() == 0 ||
	data.length() == 0)
	{
		return false;
	}

	boolean bRtn = false;
	StringBuffer pro = new StringBuffer();
	bRtn = m_regular.ExtractDataOnce(policy.m_regular, data, pro);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_file_protocol);
		
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_rule", policy.m_regular);
		log_tmp.put("meta_attr", pro);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidInAddr(PolicyObject policy, String data, Map log)
{
	if (null == data ||
		policy.m_regular.length() == 0 ||
		data.length() == 0)
	{
		return false;
	}

	boolean bRtn = false;
	StringBuffer pro = new StringBuffer();
	bRtn = m_regular.ExtractDataOnce(policy.m_regular, data, pro);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_i_addr);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_rule", policy.m_regular);
		log_tmp.put("meta_attr", pro);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidInPort(PolicyObject policy, Integer data, Map log)
{
	boolean bRtn = false;
	for(Integer one : policy.m_i_port)
	{
		if(data.equals(one))
		{
			bRtn = true;
			break;
		}
	}
	
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_i_port);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_i_port", data);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidOutAddr(PolicyObject policy, String data, Map log)
{
	if (policy.m_regular.length() == 0 ||
	data.length() == 0)
	{
		return false;
	}

	boolean bRtn = false;
	StringBuffer pro = new StringBuffer();
	bRtn = m_regular.ExtractDataOnce(policy.m_regular, data, pro);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_o_addr);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_rule", policy.m_regular);
		log_tmp.put("meta_attr", pro);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidOutPort(PolicyObject policy, Integer data, Map log)
{
	boolean bRtn = false;
	for(Integer one : policy.m_o_port)
	{
		if(data.equals(one))
		{
			bRtn = true;
			break;
		}
	}
	
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_o_port);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_o_port", data);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidSrcAddr(PolicyObject policy, String data, Map log)
{
	if (policy.m_regular.length() == 0 ||
	data.length() == 0)
	{
		return false;
	}

	boolean bRtn = false;
	StringBuffer pro = new StringBuffer();
	bRtn = m_regular.ExtractDataOnce(policy.m_regular, data, pro);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_s_addr);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_rule", policy.m_regular);
		log_tmp.put("meta_attr", pro);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidSrcPort(PolicyObject policy, Integer data, Map log)
{
	boolean bRtn = false;
	for(Integer one : policy.m_s_port)
	{
		if(data.equals(one))
		{
			bRtn = true;
			break;
		}
	}

	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_s_port);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_s_port", data);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidSrcHost(PolicyObject policy, String data, Map log)
{
	if (policy.m_regular.length() == 0 ||
	data.length() == 0)
	{
		return false;
	}

	boolean bRtn = false;
	StringBuffer pro = new StringBuffer();
	bRtn = m_regular.ExtractDataOnce(policy.m_regular, data, pro);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_s_host);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_rule", policy.m_regular);
		log_tmp.put("meta_attr", pro);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidFileUrl(PolicyObject policy, String data, Map log)
{
	if (policy.m_regular.length() == 0 ||
	data.length() == 0)
	{
		return false;
	}

	boolean bRtn = false;
	StringBuffer pro = new StringBuffer();
	bRtn = m_regular.ExtractDataOnce(policy.m_regular, data, pro);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_file_url);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_rule", policy.m_regular);
		log_tmp.put("meta_attr", pro);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
boolean IsValidFileName(PolicyObject policy, String data, Map log)
{
	if (policy.m_regular.length() == 0 ||
	data.length() == 0)
	{
		return false;
	}

	boolean bRtn = false;
	StringBuffer pro = new StringBuffer();
	bRtn = m_regular.ExtractDataOnce(policy.m_regular, data, pro);
	if(bRtn)
	{
		Map log_tmp = new HashMap<String, Object>();
		log_tmp.put("p_exe", _POLICY_TO_EXE.pe_meta_file_name);
		log_tmp.put("policy_name", policy.m_Name);
		log_tmp.put("meta_rule", policy.m_regular);
		log_tmp.put("meta_attr", pro);
		log.put("exe_list", log_tmp);
	}
	return bRtn;
}
}
