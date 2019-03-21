package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class IdentityCard {
	int weight[]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};    //十七位数字本体码权重
	char validate[]={ '1','0','X','9','8','7','6','5','4','3','2'};    //mod11,对应校验码字符值
	Map<Integer, String> m_AddressCode = new HashMap<Integer, String>();
	boolean bOk = false;
	
	IdentityCard()
	{
		
	}
	
	IdentityCard(String AddressCodePath)
	{
		try {
			bOk = InitAddressCode(AddressCodePath);
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
		if(oh.read_file(AddressCodePath, code) )
		//if(util.File2String(AddressCodePath, code))
		{
			Iterator<String> iter = code.iterator();
			while(iter.hasNext())
			{
				String tmp = iter.next();
				String[] ac = tmp.split("\\:");
				m_AddressCode.put(Integer.parseInt(ac[0]), ac[1]);
			}
		}
		
		return false;
	}
	
	boolean IsValideCheckCode(byte[] strIC)
	{
		int sum = 0;
		int mode = 0;

		for(int i=0; i<17; ++i)
		{
			sum = sum + weight[i] * (strIC[i]-'0');
		}

		mode = sum%11;

		if (strIC[17] == validate[mode])
		{
			return true;
		}

		return false;
	}
	
	boolean IsValideSequence(int Sequence, StringBuffer sex)
	{
		int i = Sequence % 2;
		if (i==0)
		{
			sex.append("女");
		}
		else
		{
			sex.append("男");
		}

		return true;
	}
	
	boolean IsValideAddressCode(int AddressCode, StringBuffer Address)
	{
		String add = m_AddressCode.get(AddressCode);
		if(add != null)
		{
			int remainder;

			remainder = AddressCode / 10000;
			remainder*=10000;
			String pProvince = m_AddressCode.get(remainder);
			if(pProvince != null)
				Address.append(pProvince + " ");

			remainder = AddressCode / 100;
			remainder*=100;
			String pCity = m_AddressCode.get(remainder);
			if (pCity != null)
				Address.append(pCity + " ");

			Address.append(add);
			return true;
		}
		
		return false;
	}
	
	boolean IsValideIdentityCard(String strIC, StringBuffer Infor)
	{
		boolean bRtn = false;
		char[] buffer = new char[32];

		do 
		{
			if (strIC.length() == 18)
			{
				//提取地址码
				int AddressCode;
				StringBuffer Address = new StringBuffer();
				System.arraycopy(strIC.toCharArray(), 0, buffer, 0, 6);
				buffer[6]=0;
				
				AddressCode = Integer.parseInt(new String().valueOf(buffer, 0, 6));
				if(buffer[0] == '0')
					break;
				IsValideAddressCode(AddressCode, Address);
				//if (!IsValideAddressCode(AddressCode, Address))
				//{
				//	Infor = "地址码无效";
				//	break;
				//}

				//提取生日
				System.arraycopy(strIC.toCharArray(), 6, buffer, 0, 8);
				buffer[8] = 0;
				
				Birthday lBirthday = new Birthday(new String().valueOf(buffer, 0, 8));
				if (!lBirthday.IsValidBirthday())
				{
					Infor.append("生日无效");
					break;
				}

				//提取序号
				System.arraycopy(strIC.toCharArray(), 6+8, buffer, 0, 3);
				buffer[3] = 0;
				StringBuffer sex = new StringBuffer();
				if (!IsValideSequence(Integer.parseInt(new String().valueOf(buffer, 0, 3)), sex))
				{
					Infor.append("序号无效");
					break;
				}

				//计算校验码
				if (!IsValideCheckCode(strIC.getBytes()))
				{
					Infor.append("校验码无效");
					break;
				}

				Infor.append(Address + " " + sex);

				bRtn = true;
			}
			else
			{
				//错误的长度
				Infor.append("长度无效");
			}
		} while (false);

		return bRtn;
	}
	
	
	boolean IsValideIdentityCard(PolicyObject policy, List<String> word, List<String> Id, List<String> Infor, Map log)
	{
		boolean bRtn = false;

		if (word.size() == 0)
		{
			return false;
		}

		Id.clear();
		Infor.clear();

		Map id_log;
		StringBuffer id_log_info = new StringBuffer();

		Iterator<String> iter = word.iterator();
		while(iter.hasNext())
		{
			String card = iter.next();
			StringBuffer info = new StringBuffer();
			if(IsValideIdentityCard(card, info))
			{
				Id.add(card);
				Infor.add(info.toString());
				id_log_info.append(info).append(":");
			}
		}

		if (Id.size() != 0)
		{
			bRtn = true;
		}

	    if(bRtn)
	    {	    	
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_id_card_18);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("data_info", id_log_info);

			log.put("exe_list", log_tmp);
	    }

		return bRtn;
	}
	
	
	//15
	boolean IsValideIdentityCard15(String strIC, StringBuffer Infor)
	{
		boolean bRtn = false;
		char[] buffer = new char[32];

		do 
		{
			if (strIC.length() == 15)
			{
				//提取地址码
				int AddressCode;
				StringBuffer Address = new StringBuffer();
				System.arraycopy(strIC.toCharArray(), 0, buffer, 0, 6);
				buffer[6]=0;
				AddressCode = Integer.parseInt(new String().valueOf(buffer, 0, 6));
				if(buffer[0] == '0')
					break;
				IsValideAddressCode(AddressCode, Address);
				//if (!IsValideAddressCode(AddressCode, Address))
				//{
				//	Infor = "地址码无效";
				//	break;
				//}

				//提取生日
				System.arraycopy(strIC.toCharArray(), 6, buffer, 0, 6);
				buffer[6] = 0;
				Birthday lBirthday = new Birthday(new String().valueOf(buffer, 0, 6));
				if (!lBirthday.IsValidBirthday())
				{
					Infor.append("生日无效");
					break;
				}

				//提取序号
				System.arraycopy(strIC.toCharArray(), 6+6, buffer, 0, 3);
				buffer[3] = 0;
				StringBuffer sex = new StringBuffer();
				if (!IsValideSequence(Integer.parseInt(new String().valueOf(buffer, 0, 3)), sex))
				{
					Infor.append("序号无效");
					break;
				}

				Infor.append(Address + " " + sex);

				bRtn = true;
			}
			else
			{
				//错误的长度
				Infor.append("长度无效");
			}
		} while (false);

		return bRtn;
	}

	boolean IsValideIdentityCard15(PolicyObject policy, List<String> word, List<String> Id, List<String> Infor, Map log)
	{
		boolean bRtn = false;

		if (word.size() == 0)
		{
			return false;
		}

		Id.clear();
		Infor.clear();

		Map id_log;
		StringBuffer id_log_info = new StringBuffer();
		
		Iterator<String> iter = word.iterator();
		while(iter.hasNext())
		{
			String card = iter.next();
			StringBuffer info = new StringBuffer();
			if(IsValideIdentityCard15(card, info))
			{
				Id.add(card);
				Infor.add(info.toString());
				id_log_info.append(info).append(":");
			}
		}

		if (Id.size() != 0)
		{
			bRtn = true;
		}

		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_id_card_15);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("data_info", id_log_info);

			log.put("exe_list", log_tmp);
		}

		return bRtn;
	}

}