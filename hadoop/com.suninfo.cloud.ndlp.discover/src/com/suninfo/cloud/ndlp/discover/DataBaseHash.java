package com.suninfo.cloud.ndlp.discover;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.suninfo.cloud.ndlp.dbf.client.DbfClient;
import com.suninfo.cloud.ndlp.dbf.common.MatchInfo;
import com.suninfo.cloud.ndlp.dbf.common.MatchRecord;
import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class DataBaseHash {
	
	static DbfClient dbf = new DbfClient();


	boolean find_hash(PolicyObject policy, String file_buffer, List<String> hash_info, Map log) throws IOException
	{
		boolean bRtn = false;
		hash_info.clear();
		
		MatchInfo matchInfo = new MatchInfo();
		for(String one: policy.m_data_hash_name)
		{
			matchInfo.addDbf(one);
		}
		
		matchInfo.setMatchFilePath(file_buffer);
		matchInfo.setCol(policy.m_data_hash_rol);
		matchInfo.setRow(policy.m_data_hash_row);
		
		
		MatchRecord mr = dbf.match(matchInfo);
		
		if(mr != null)
		{
			Map<String, Object> log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_database_hash);
			log_tmp.put("policy_name", policy.m_Name);
			
			StringBuffer info = new StringBuffer();
			
			info.append("dbfName:").append(mr.getDbfName()).append(" ");
			
			info.append("recordsInfo: ");
			List<String> records = new ArrayList<String>();
			mr.getRecord(records);
			for(String one : records)
			{
				info.append(one).append("  ");
				hash_info.add(one);
			}
			
			
			log_tmp.put("db_info", info.toString());
			
			log.put("exe_list", log_tmp);
			bRtn = true;
		}

		return bRtn;
	}
}
