package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TO_EXE;

public class RegularBankCard {
	boolean bOK = false;
	Map<Integer, String> m_start = new HashMap<Integer, String>();
	
	RegularBankCard()
	{
		
	}
	
	RegularBankCard(String AddressCodePath)
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
				m_start.put(Integer.parseInt(ac[0]), ac[1]);
			}
		}
		
		return false;
	}
	
	boolean Luhn(String sCard)
	{
		int CardLeng = sCard.length();
		byte[] pCard = sCard.getBytes();
		int odd = 0;
		int pairs = 0;
		int i;
		for(i= CardLeng-1; i>=0; i--)
		{
			odd += (pCard[i] - '0');
			i--;//跳过偶数位
		}

		for(i= CardLeng-1; i>=0; i--)
		{
			i--;//跳过偶数位
			if (i<0)
			{
				break;
			}
			int j = (pCard[i] - '0')*2;
			if (j>9)
			{
				j-=9;
			}
			odd += j;
		}

		return ((odd+pairs)%10) == 0 ?true:false;
	}
	
	boolean GetBankInfor(String sCard, StringBuffer BankInfor)
	{
		if(m_start.size() < 1)
			return false;
		
		boolean bRtn = false;
		char[] iBank = new char[32];

		System.arraycopy(sCard.toCharArray(), 0, iBank, 0, 6);
		String info = m_start.get(Integer.parseInt(new String().valueOf(iBank, 0, 6)));
		if(info != null &&info.length() != 0)
		{
			BankInfor.append(info);
			bRtn=true;
		}
		else
			BankInfor.append("not find card info");

		return bRtn;
	}
	
	
	boolean IsValideCreditCard(String sCard, StringBuffer CardInfor)
	{
		if (sCard.length() != 16 &&
				sCard.length() != 19)
		{
			CardInfor.append("card length must be 16 or 19");
			return false;
		}

		if (!Luhn(sCard))
		{
			CardInfor.append("card Luhn error");
			return false;
		}

		GetBankInfor(sCard, CardInfor);

		return true;
	}
	
	boolean IsValideCreditCard(PolicyObject policy, List<String> word, List<String> bCard, List<String> Infor, Map log)
	{
		if (word.size() == 0)
		{
			return false;
		}

		bCard.clear();
		Infor.clear();

		boolean bRtn = false;
		Map card_log;
		StringBuffer log_info = new StringBuffer();
		Iterator<String> iter = word.iterator();
		while(iter.hasNext())
		{
			StringBuffer card_info =new StringBuffer();
			String card = iter.next();
			if(IsValideCreditCard(card, card_info))
			{
				log_info.append(card + ":" + card_info.toString() + " ");
				bCard.add(card);
				Infor.add(card_info.toString());
			}
		}

		if (bCard.size() != 0)
		{
			bRtn = true;
		}

		if(bRtn)
		{
			Map log_tmp = new HashMap<String, Object>();
			log_tmp.put("p_exe", _POLICY_TO_EXE.pe_bank_card_16);
			log_tmp.put("policy_name", policy.m_Name);
			log_tmp.put("data_info", log_info);

			log.put("exe_list", log_tmp);
		}

		return bRtn;
	}
}
