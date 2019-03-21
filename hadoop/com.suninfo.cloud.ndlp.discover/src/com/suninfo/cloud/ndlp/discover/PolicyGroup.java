package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TYPE;

public class PolicyGroup {
	boolean m_offline;
	boolean m_isGroup;
	boolean m_not;
	String m_id;
	String m_severity;
	String m_seq;
	int m_level;
	String m_reaction;
	private String m_name = new String();
	_POLICY_TYPE m_type;
	List<PolicyGroup>m_policyFrame = new ArrayList<PolicyGroup>();
	PolicyObject m_policyObject = new PolicyObject();
	static PolicyExecutor m_policy_executor = new PolicyExecutor();
	
	public boolean init_group(Map policy)
	{
		boolean bRtn = false;
		if(1 == (double)policy.get("group"))
			m_isGroup = true;
		else
			m_isGroup = false;
		
		if(1 == (double)policy.get("not"))
			m_not = true;
		else
			m_not = false;
		
		m_name = (String)policy.get("name");
		
		String group_type = (String)policy.get("group_type");
		
		if(group_type.equals("null"))
			m_type = _POLICY_TYPE.p_null;
		else if(group_type.equals("and"))
			m_type = _POLICY_TYPE.p_and;
		else if(group_type.equals("or"))
			m_type = _POLICY_TYPE.p_or;
		else if(group_type.equals("seq"))
			m_type = _POLICY_TYPE.p_seq;
		else
			;

		if (m_isGroup)
		{
			List<Map> rule_list = (List<Map>)policy.get("r_list");
			Iterator<Map> iter = rule_list.iterator();
			while(iter.hasNext())
			{
				PolicyGroup rule = new PolicyGroup();
				bRtn = rule.init_group(iter.next());
				if(bRtn)
				{
					if(rule.m_isGroup)
						System.out.println("--group:"+rule.m_name);
					else
						;//this rule is an object;
					m_policyFrame.add(rule);
				}
			}
		}
		else
		{
			bRtn = init_policy_object(policy);
		}

		return bRtn;
	}
	
	public boolean init_policy_object(Map policy)
	{
		m_isGroup = false;
		if(1 == (double)policy.get("not"))
			m_not = true;
		else
			m_not = false;
		String group_type = (String)policy.get("group_type");

		if(group_type.equals("null"))
			m_type = _POLICY_TYPE.p_null;
		else if(group_type.equals("and"))
			m_type = _POLICY_TYPE.p_and;
		else if(group_type.equals("or"))
			m_type = _POLICY_TYPE.p_or;
		else if(group_type.equals("seq"))
			m_type = _POLICY_TYPE.p_seq;
		else
			;

		return m_policyObject.init_policy_object(policy);
	}
	
	public boolean init_json_policy(Map policy)
	{
		boolean bRtn = false;
		if((boolean)policy.get("offline"))
		{
			if( m_offline != (boolean)policy.get("offline") )
				return false;
		}
		else
		{
			if(m_offline == true)
				return false;
		}

		if((double) policy.get("group") == 1)
			m_isGroup = true;
		else
			m_isGroup = false;
		
		m_not = Boolean.valueOf( String.valueOf(policy.get("not")) );
		String group_type = (String)policy.get("group_type");

		m_id = String.valueOf(policy.get("id"));
		m_severity = String.valueOf(policy.get("severity"));
		
		m_seq = String.valueOf(policy.get("seq"));
		if(m_seq.length() != 0)
			m_level = (int)(double)policy.get("seq");
		
		m_reaction = (String)policy.get("reaction");
		m_name = (String)policy.get("name");
		if(group_type.equals("null"))
			m_type = _POLICY_TYPE.p_null;
		else if(group_type.equals("and"))
			m_type = _POLICY_TYPE.p_and;
		else if(group_type.equals("or"))
			m_type = _POLICY_TYPE.p_or;
		else if(group_type.equals("seq"))
			m_type = _POLICY_TYPE.p_seq;
		else
			;

		if (m_isGroup)
		{
			List<Map> rule_list = (List<Map>)policy.get("r_list");
			Iterator<Map> iter = rule_list.iterator();
			while(iter.hasNext())
			{
				PolicyGroup rule = new PolicyGroup();
				bRtn = rule.init_group(iter.next());
				if(bRtn)
				{
					System.out.println("-group:"+rule.m_name);
					m_policyFrame.add(rule);
				}
			}
		}
		else
		{

		}

		return bRtn;
	}
	
	public String get_name()
	{
		return m_name;
	}

	_POLICY_TYPE get_policy_type()
	{
		return m_type;
	}
	
	boolean is_not()
	{
		return m_not;
	}
	
	boolean is_group()
	{
		return m_isGroup;
	}

	boolean execute_policy(MessageBus mb, Map log)
	{
		boolean bRtn = false;
		bRtn = m_policy_executor.execut_policy(mb, m_policyObject, log);
		return bRtn;
	}
	
	boolean execute_group(MessageBus mb, Map log)
	{
		boolean bRtn = false;
		boolean bExit = false;
		
		Iterator<PolicyGroup> iter = m_policyFrame.iterator();
		
		l_Ret:
		while(iter.hasNext() && !bExit)
		{
			PolicyGroup pg = iter.next();
			_POLICY_TYPE tmp_type = pg.get_policy_type();
			boolean tmp_not = pg.is_not();
			if(pg.is_group())
			{
				bRtn = pg.execute_group(mb, log);
			}
			else
			{
				bRtn = pg.execute_policy(mb, log);
			}
			
			if (tmp_not)
			{
				bRtn = !bRtn;
				if(bRtn)
				{
					//Map log_tmp = new HashMap<String, Object>();
					//log_tmp.put("p_exe", pe_result_negate);
					//log_tmp["info"] = Json::Value("result negate");
					//CLogCreate::insert_exe_array(log, log_tmp);
					;
				}
			}
	
			switch(tmp_type)
			{
				case p_null:
					break;
				case p_and:
					if (false == bRtn)
					{
						bExit = true;
						break l_Ret;
					}
					break;
				case p_or:
					if(true == bRtn)
					{
						bExit = true;
						break l_Ret;
					}
					break;
				case p_seq: // for policy
					if (false == bRtn)
					{
						bExit = true;
						break l_Ret;
					}
					break;
				default:
					;
			}
		}
	
	
		return bRtn;
	}
}