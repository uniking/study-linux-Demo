package com.suninfo.cloud.ndlp.discover;

import java.util.HashMap;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class MailExecutor {
	Regular m_regular = new Regular();
	
	boolean match_String(String rule, String buffer, StringBuffer Content)
	{
		boolean bRtn;
		if(buffer.length() == 0)
			return false;
		bRtn = m_regular.ExtractDataOnce(rule, buffer, Content);
		return bRtn;
	}

	boolean is_mail_file(PolicyObject policy, boolean bMail, Map log)
	{
		boolean bRtn = policy.m_mail_is_mail == bMail ? true:false;
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_is_mail);
			log_tmp.put("policy_name", policy.m_Name);
			
			log.put("exe_list", log_tmp);
		}
		
		return bRtn;
	}
	boolean is_mail_sender(PolicyObject policy, String buffer, Map log)
	{
		boolean bRtn = false;
		StringBuffer exBuffer = new StringBuffer();
		bRtn = match_String(policy.m_regular, buffer, exBuffer);
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_sender);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("mail_rule", policy.m_regular);
			log_tmp.put("mail_attr", exBuffer);
			log.put("exe_list", log_tmp);
		}
		return bRtn;
	}
	boolean is_mail_from(PolicyObject policy, String buffer, Map log)
	{
		boolean bRtn = false;
		StringBuffer exBuffer = new StringBuffer();
		bRtn = match_String(policy.m_regular, buffer, exBuffer);
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_from);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("mail_rule", policy.m_regular);
			log_tmp.put("mail_attr", exBuffer);
			log.put("exe_list", log_tmp);
		}
		return bRtn;
	}
	boolean is_mail_to(PolicyObject policy, String buffer, Map log)
	{
		boolean bRtn = false;
		StringBuffer exBuffer = new StringBuffer();
		bRtn = match_String(policy.m_regular, buffer, exBuffer);
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_to);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("mail_rule", policy.m_regular);
			log_tmp.put("mail_attr", exBuffer);
			log.put("exe_list", log_tmp);
		}
		return bRtn;
	}
	boolean is_mail_subject(PolicyObject policy, String buffer, Map log)
	{
		boolean bRtn = false;
		StringBuffer exBuffer = new StringBuffer();
		bRtn = match_String(policy.m_regular, buffer, exBuffer);
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_subject);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("mail_rule", policy.m_regular);
			log_tmp.put("mail_attr", exBuffer);
			log.put("exe_list", log_tmp);
		}
		return bRtn;
	}
	boolean is_mail_replyto(PolicyObject policy, String buffer, Map log)
	{
		boolean bRtn = false;
		StringBuffer exBuffer = new StringBuffer();
		bRtn = match_String(policy.m_regular, buffer, exBuffer);
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_replyto);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("mail_rule", policy.m_regular);
			log_tmp.put("mail_attr", exBuffer);
			log.put("exe_list", log_tmp);
		}
		return bRtn;
	}
	boolean is_mail_cc(PolicyObject policy, String buffer, Map log)
	{
		boolean bRtn = false;
		StringBuffer exBuffer = new StringBuffer();
		bRtn = match_String(policy.m_regular, buffer, exBuffer);
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_cc);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("mail_rule", policy.m_regular);
			log_tmp.put("mail_attr", exBuffer);
			log.put("exe_list", log_tmp);
		}
		return bRtn;
	}
	boolean is_mail_bcc(PolicyObject policy, String buffer, Map log)
	{
		boolean bRtn = false;
		StringBuffer exBuffer = new StringBuffer();
		bRtn = match_String(policy.m_regular, buffer, exBuffer);
		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_bcc);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("mail_rule", policy.m_regular);
			log_tmp.put("mail_attr", exBuffer);
			log.put("exe_list", log_tmp);
		}
		return bRtn;
	}
	boolean is_mail_attachment_num(PolicyObject policy, int num, Map log)
	{
		boolean bRtn = false;
		if(num == -1)
			return false;

		switch(policy.m_mail_attachment_num_type)
		{
			case n_less_then:
				if(num < policy.m_mail_attachment_num_first)
					bRtn = true;
				break;
			case n_great_then:
				if(num > policy.m_mail_attachment_num_first)
					bRtn = true;
				break;
			case n_between:
				if(policy.m_mail_attachment_num_first <= num &&
				policy.m_mail_attachment_num_second >= num)
					bRtn = true;
				break;
			case n_equal:
				if(policy.m_mail_attachment_num_first == num )
					bRtn = true;
				break;
			default:
			;
		}

		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_mail_attachment_num);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("first_num", policy.m_mail_attachment_num_first);
			switch(policy.m_mail_attachment_num_type)
			{
				case n_less_then:
					log_tmp.put("type","less then");
					break;
				case n_great_then:
					log_tmp.put("type","great then");
					break;
				case n_between:
					log_tmp.put("type","between");
					log_tmp.put("second_num", policy.m_mail_attachment_num_second);
					break;
				case n_equal:
					log_tmp.put("type","equal");
					break;
				default:
				;
			}
			
			log_tmp.put("number", num);
			log.put("exe_list", log_tmp);
		}

		return bRtn;
	}
}
