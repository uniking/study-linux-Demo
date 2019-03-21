package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class Phone {
	boolean bOK = false;
	Map<Integer, String> m_ISP = new HashMap<Integer, String>();
	
	Phone()
	{
		
	}
	
	Phone(String AddressCodePath)
	{
		try {
			bOK = InitAddressCode(AddressCodePath);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	boolean InitAddressCode(String AddressCodePath) throws Exception
	{
		List<String> code = new ArrayList<String>();
		oper_hdfs oh = new oper_hdfs();
		oh.initRemoteHdfs();
		if(oh.read_file(AddressCodePath, code))
		//if(util.File2String(AddressCodePath, code))
		{
			Iterator<String> iter = code.iterator();
			while(iter.hasNext())
			{
				String tmp = iter.next();
				String[] ac = tmp.split("\\:");
				m_ISP.put(Integer.parseInt(ac[0]), ac[1]);
			}
		}
		
		return false;
	}
	
	boolean isValidPhoneNumber(String PN, StringBuffer PNInfor)
	{
		boolean bRtn = false;
		if (PN.length() != 11)
		{
			PNInfor.append("length error");
			return false;
		}

		byte[] pPN = PN.getBytes();
		char[] cPN = new char[32];
		System.arraycopy(PN.toCharArray(), 0, cPN, 0, 3);
		cPN[3]=0;

		if(cPN[0] != '1')
		{
			PNInfor.append("first number must be 1");
			return false;
		}

		int iPN = Integer.parseInt(new String().valueOf(cPN, 0, 3));
		
		String  one= m_ISP.get(iPN);
		if(one != null)
		{
			PNInfor.append(PN+" "+one);
			bRtn = true;
		}

		if (bRtn == false)
		{
			System.arraycopy(PN.toCharArray(), 0, cPN, 0, 4);
			cPN[4]=0;
			
			iPN = Integer.parseInt(new String().valueOf(cPN, 0, 4));
			
			one= m_ISP.get(iPN);
			if(one != null)
			{
				PNInfor.append(one);
				bRtn = true;
			}
		}


		bRtn = true;//database not use to check phone numbers
		return bRtn;
	}

	boolean isValidPhoneNumber(PolicyObject policy, List<String> words, List<String> phone, List<String> PNInfor, Map log)
	{
		boolean bRtn = false;

		if (words.size() == 0)
		{
			return false;
		}
		phone.clear();
		PNInfor.clear();

		Map phone_log;
		StringBuffer phone_info = new StringBuffer();
		
		Iterator<String> iter = words.iterator();
		while(iter.hasNext())
		{
			StringBuffer l_PNInfo = new StringBuffer();
			String one = iter.next();
			if (isValidPhoneNumber(one, l_PNInfo))
			{
				if(phone_info.length() < 512*1024)
				{
					phone_info.append(one + ":" + l_PNInfo + " ");
				}
				phone.add(one);
				PNInfor.add(l_PNInfo.toString());
			}
		}

		if (phone.size() != 0)
		{
			bRtn = true;
		}

		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_phone);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("data_info", phone_info);

			log.put("exe_list", log_tmp);
		}

		return bRtn;
	}

}
