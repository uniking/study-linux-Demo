package com.suninfo.cloud.ndlp.discover;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

import ssdeepWrapper.Jssdeep;

import com.suninfo.ndlp.filehash.MyListString;
import com.suninfo.ndlp.filehash.RPC_Client_test;


public class FuzzyHash {
	
	RPC_Client_test file_hash = new RPC_Client_test();
	
	public FuzzyHash()
	{
		try {
			file_hash.RPC_Init("node1", 9530);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	boolean match_hash(String fileHash, PolicyObject policy, Map info)
	{
		boolean bRtn = false;
		
		MyListString filter_qua = new MyListString();
		for(String one : policy.m_file_hash_data_name)
		{
			System.out.println(one + " " + policy.m_file_hash_threshold);
			filter_qua.insertString(one);
		}

		//List<String> tmp_log = file_hash.Fingerprint_Mach(filter_qua, 90, fileHash);
		List<String> tmp_log = file_hash.Fingerprint_Mach(filter_qua, policy.m_file_hash_threshold, fileHash);
		
		if(tmp_log != null)
		{
			StringBuffer sb_log = new StringBuffer();
			sb_log.append("[");
			for(String one:tmp_log)
			{
				sb_log.append(one);
				sb_log.append(",");
			}
			sb_log.setLength(sb_log.length()-1);//del ,
			sb_log.append("]");
			
			Map<String, Object> log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_file_hash);
			log_tmp.put("policy_name", policy.m_Name);
			//log_tmp.put("sim_file_name", "");
			//log_tmp.put("sim_data_name", "");
			//log_tmp.put("sim_val", 0);
			log_tmp.put("info", sb_log.toString());
			
			if(info.get("exe_list") == null)
			{
				List<Map>  el = new ArrayList<Map>();
				el.add(log_tmp);
				info.put("exe_list", el);
			}
			else
			{
				List<Map>  el = (ArrayList<Map>)info.get("exe_list");
				el.add(log_tmp);
				info.put("exe_list", el);
			}
			bRtn = true;
		}

		return bRtn;
	}
	
	public boolean GetDocInfor(PolicyObject policy, MessageBus mb, Map log)
	{
		boolean bRtn = false;
		StringBuffer filehash = new StringBuffer();
		Jssdeep jsd = new Jssdeep();
		if(mb.m_txt_file_path != null)
		{
			jsd.fuzzy_hash_buf(mb.m_out_file_content.getBytes(), mb.m_out_file_content.getBytes().length, filehash);
		}
		else
		{
			oper_hdfs oh = new oper_hdfs();
			oh.initRemoteHdfs();
			long flength = oh.getLength(mb.m_bin_file_path);
			byte[] buf = new byte[(int)flength];
			try {
				oh.readFile(mb.m_bin_file_path, buf, flength);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			jsd.fuzzy_hash_buf(buf, flength, filehash);
			//jsd.fuzzy_hash_filename(mb.m_bin_file_path, filehash);
			System.out.println(mb.m_bin_file_path + filehash.toString());
		}
		
		
		bRtn = match_hash(filehash.toString(), policy, log);
		
		return bRtn;
	}

}
