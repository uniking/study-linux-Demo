package com.suninfo.cloud.ndlp.discover;

import java.util.List;
import java.util.Map;

public class GoodsCounter {
	boolean count_goods(PolicyObject policy, List<String> goods, int count, Map log)
	{
		boolean bRtn = false;

		int c = goods.size();
		switch(policy.m_counter_type)
		{
			case 0://equal
				if (c == policy.m_counter_small_num)
				{
					bRtn = true;
				}
				break;
			case 1://large
				if (c > policy.m_counter_small_num)
				{
					bRtn = true;
				}
				break;
			case 2://less
				if (c < policy.m_counter_small_num)
				{
					bRtn = true;
				}
				break;
			case 3://between
				{
					if (c > policy.m_counter_small_num && c < policy.m_counter_big_num)
					{
						bRtn = true;
					}
				}
				break;
			case 4://large_equal
				{
					if(c >= policy.m_counter_small_num)
					{
						bRtn = true;
					}
				}
				break;
			case 5://less_equal
				{
					if(c <= policy.m_counter_small_num)
					{
						bRtn = true;
					}
				}
				break;
			case 6://not_equal
				{
					if(c != policy.m_counter_small_num)
					{
						bRtn = true;
					}
				}
				break;
			default:
			;
		}

		return bRtn;
	}

}
