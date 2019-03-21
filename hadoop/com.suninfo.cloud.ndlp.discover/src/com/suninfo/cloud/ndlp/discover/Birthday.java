package com.suninfo.cloud.ndlp.discover;

public class Birthday {
	int m_num;
	int m_year;
	int m_month;
	int m_day;
	
	//格式 20150826 2015年08月26日 18位身份证
	//或 150826 15位身份证
	Birthday(String Birthday)
	{
		char[] temp = new char[16];
		if (Birthday.length() == 8)
		{
			m_num = 8;
			System.arraycopy(Birthday.toCharArray(), 0, temp, 0, 4);
			temp[4] = 0;
			m_year = Integer.parseInt(new String().valueOf(temp, 0, 4));

			System.arraycopy(Birthday.toCharArray(), 4, temp, 0, 2);
			temp[2] = 0;
			m_month = Integer.parseInt(new String().valueOf(temp, 0, 2));

			System.arraycopy(Birthday.toCharArray(), 6, temp, 0, 2);
			temp[2] = 0;
			m_day = Integer.parseInt(new String().valueOf(temp, 0, 2));
		}
		else if(Birthday.length() == 6)
		{
			m_num = 6;
			System.arraycopy(Birthday.toCharArray(), 0, temp, 0, 2);
			temp[2] = 0;
			m_year = Integer.parseInt(new String().valueOf(temp, 0, 2));

			System.arraycopy(Birthday.toCharArray(), 2, temp, 0, 2);
			temp[2] = 0;
			m_month = Integer.parseInt(new String().valueOf(temp, 0, 2));

			System.arraycopy(Birthday.toCharArray(), 4, temp, 0, 2);
			temp[2] = 0;
			m_day = Integer.parseInt(new String().valueOf(temp, 0, 2));
		}

	}

	boolean IsValidBirthday()
	{
		boolean bRtn = false;
		do 
		{
			if (m_num == 8)
			{
				if (m_year > 2100 ||
					m_year < 1900)
				{
					break;
				}
			}


			if (m_month > 12 ||
				m_month < 1)
			{
				break;
			}

			if (m_day > 31 ||
				m_day <1)
			{
				break;
			}

			bRtn = true;
		} while (false);

		return bRtn;
	}
}
