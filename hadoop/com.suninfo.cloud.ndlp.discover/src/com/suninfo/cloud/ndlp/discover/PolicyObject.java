package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.suninfo.cloud.ndlp.discover.PolicyObject._POLICY_TYPE;

public class PolicyObject implements PolicyInit{
	public enum _POLICY_TYPE
	{
		p_null,
		p_and,
		p_or,
		p_seq
	};

	public enum _NUM_TYPE
	{
	    n_unknow,
	    n_less_then,
	    n_great_then,
	    n_between,
	    n_equal
	};

	public enum _TIME_TYPE
	{
	    t_unknow,
	    t_before,
	    t_after,
	    t_between,
	    t_last
	};


	public enum _POLICY_TO_EXE
	{
		pe_file_buffer,
		pe_file_extract,
		pe_regedit,
		pe_phone,
		pe_id_card_15,
		pe_id_card_18,
		pe_bank_card_16,
		pe_bank_card_19,
		pe_goods_counter,
		pe_goods_sum_counter,
		pe_file_hash,
		pe_bayes,
		pe_meta_i_addr,
		pe_meta_i_port,
		pe_meta_o_addr,
		pe_meta_o_port,
		pe_meta_s_addr,
		pe_meta_s_port,
		pe_meta_s_host,
		pe_meta_file_url,
		pe_meta_file_protocol,
		pe_doc_file,
		pe_doc_manager,
		pe_doc_company,
		pe_doc_title,
		pe_doc_subject,
		pe_doc_author,
		pe_doc_keywords,
		pe_doc_description,
		pe_doc_lastModifieBy,
		pe_doc_modified_time,
		pe_doc_category,
		pe_doc_header,
		pe_doc_footer,
		pe_doc_encrypted,
		pe_doc_extension,
		pe_media_is_media,
		pe_media_track,
		pe_media_bitrate,
		pe_media_disc,
		pe_media_genre,
		pe_media_isrc,
		pe_media_encoder,
		pe_media_album_artlist,
		pe_media_album,
		pe_media_artlist,
		pe_media_title,
		pe_media_date,
		pe_media_duration,
		pe_media_comment,
		pe_media_creation_time,
		pe_media_publisher,
		pe_media_copyright,
		pe_media_language,
		pe_mail_sender,
		pe_mail_from,
		pe_mail_to,
		pe_mail_subject,
		pe_mail_replyto,
		pe_mail_cc,
		pe_mail_bcc,
		pe_mail_is_mail,
		pe_mail_attachment_num,
		pe_file_attr,
		pe_meta_file_name,
		pe_file_size,
		pe_database_hash,
		pe_doc_template,
		pe_doc_type,
		pe_result_negate
	};

	public _POLICY_TO_EXE m_input_type;// find input buffer
	public _POLICY_TO_EXE m_policy_type;

	public String m_Name;//policy name
	public _POLICY_TO_EXE m_Class;
	public String m_level;
	public String m_uid;
	public boolean m_not;
	public _POLICY_TYPE m_root_type;

	public String m_regular;
	public long m_size;
	public List<String> m_bayes_class_name;

		//goods_counter
	public int m_counter_type;//0 equal, 1 large, 2 less, 3 between, 4 large_equal, 5 less_equal, 6 not_equal
	public int m_counter_big_num;
	public int m_counter_small_num;

		//goods sum counter
	public int m_sum_counter;

		//meta file
	public String m_i_addr;
		//String m_i_port;// 80|8080|139
	public List<Integer> m_i_port;
	public String m_o_addr;
		//String m_o_port;
	public List<Integer>  m_o_port;
	public String m_s_addr;
		//String m_s_port;
	public List<Integer> m_s_port;
	public String m_s_host;
	public String m_file_url;
	public String m_protocol;//http|smtp|smb

	public int m_file_hash_threshold; // 0-100
	public List<String> m_file_hash_data_name = new ArrayList<String>();

		//CPolicyExecutor* m_policy_executor;
		//char* m_policy_executor;

		//doc file basic attr
		_NUM_TYPE m_file_size_type;
		public long m_file_size_first; //64bit
		public long m_file_size_second; //64bit

		//doc file attributes
		boolean m_doc_is_doc;
		String m_doc_manager;
		String m_doc_company;
		String m_doc_title;
		String m_doc_subject;
		String m_doc_author;
		String m_doc_keywords;
		String m_doc_description;
		String m_doc_lastModifiedBy;
		String m_doc_modified_time;
		_TIME_TYPE m_doc_modified_time_type;
		long m_doc_modified_time_first;
		long m_doc_modified_time_second;
		long m_doc_modified_time_last_days;
		String m_doc_category;
		String m_doc_header;
		String m_doc_footer;

		boolean m_doc_is_encrypted;
		String m_doc_extension;//regular

		//media file attributes
		boolean m_media_is_media;
		int m_media_track;
		int m_media_bitrate;
		int m_media_disc;
		String m_media_genre;
		String m_media_isrc;
		String m_media_encoder;
		String m_media_album_artlist;
		String m_media_album;
		String m_media_artlist;
		String m_media_title;
		String m_media_date;
		String m_media_duration;
		String m_media_comment;
		String m_media_creation_time;
		String m_media_publisher;
		String m_media_copyright;
		String m_media_language;

		//mail file attributes
		String m_mail_sender;
		String m_mail_from;
		String m_mail_to;
		String m_mail_subject;
		String m_mail_replyto;
		String m_mail_cc;
		String m_mail_bcc;
		//boolean m_mail_is_encrypted;
		boolean m_mail_is_mail;
		_NUM_TYPE m_mail_attachment_num_type;
		long m_mail_attachment_num_first;
		long m_mail_attachment_num_second;

		List<String> m_data_hash_name = new ArrayList<String>();
		int m_data_hash_row;
		int m_data_hash_rol;
	    
		//boolean init_file_extract_policy(Map policy);
		//public static boolean init_regedit_policy(Map policy);
		
		
		
		boolean init_policy_object(Map policy)
		{
			boolean bRtn = true;
			m_policy_type = _POLICY_TO_EXE.values()[(int)(double)policy.get("policy_exe")];
			m_input_type = _POLICY_TO_EXE.values()[(int)(double)policy.get("input_type")];
			m_Name = (String)policy.get("name");
			System.out.println("object:"+ m_Name);
			if(1 == (double)policy.get("not"))
				m_not = true;
			else
				m_not = false;

			String root_type = (String)policy.get("root_type");
			if(root_type == null)
				m_root_type = _POLICY_TYPE.p_null;
			else if(root_type.equals("null"))
				m_root_type = _POLICY_TYPE.p_null;
			else if(root_type.equals("and"))
				m_root_type = _POLICY_TYPE.p_and;
			else if(root_type.equals("or"))
				m_root_type = _POLICY_TYPE.p_or;
			else if(root_type.equals("seq"))
				m_root_type = _POLICY_TYPE.p_seq;
			else
				;

			m_regular = (String)policy.get("rule");

			switch(m_policy_type)
			{
				case pe_file_buffer:
					break;
				case pe_file_extract:
					break;
				case pe_regedit:
					//init_regedit_policy(policy);
					break;
				case pe_phone:
					break;
				case pe_id_card_15:
					break;
				case pe_id_card_18:
					break;
				case pe_bank_card_16:
					break;
				case pe_bank_card_19:
					break;
				case pe_goods_counter:
					bRtn = init_goods_counter_policy(policy);
					break;
				case pe_goods_sum_counter:
					//bRtn = init_goods_sum_counter_policy(policy);
					break;
				case pe_file_hash:
					bRtn = init_file_hash_policy(policy);
					break;
				case pe_bayes:
					//bRtn = init_bayes_policy(policy);
					break;

				case pe_meta_i_addr:
					//bRtn = init_in_addr_policy(policy);
					break;
				case pe_meta_i_port:
					//bRtn = init_in_port_policy(policy);
					break;
				case pe_meta_o_addr:
					//bRtn = init_out_addr_policy(policy);
					break;
				case pe_meta_o_port:
					//bRtn = init_out_port_policy(policy);
					break;
				case pe_meta_s_addr:
					//bRtn = init_src_addr_policy(policy);
					break;
				case pe_meta_s_port:
					//bRtn = init_src_port_policy(policy);
					break;
				case pe_meta_s_host:
					//bRtn = init_src_host_policy(policy);
					break;
				case pe_meta_file_url:
					//bRtn = init_file_url_policy(policy);
					break;
				case pe_meta_file_name:
					break;

				case pe_file_size:
					//bRtn = init_file_size(policy);
					break;
				case pe_doc_file:
				case pe_doc_manager:
				case pe_doc_company:
				case pe_doc_title:
				case pe_doc_subject:
				case pe_doc_author:
				case pe_doc_keywords:
				case pe_doc_description:
				case pe_doc_lastModifieBy:
					break;

				case pe_doc_modified_time:
					//bRtn = init_doc_modified_time(policy);
					break;
				case pe_doc_category:
				case pe_doc_template:
				case pe_doc_header:
				case pe_doc_footer:
				case pe_doc_encrypted:
				case pe_doc_type:
				case pe_doc_extension:
				case pe_media_is_media:
				case pe_media_track:
				case pe_media_bitrate:
				case pe_media_disc:
				case pe_media_genre:
				case pe_media_isrc:
				case pe_media_encoder:
				case pe_media_album_artlist:
				case pe_media_album:
				case pe_media_artlist:
				case pe_media_title:
				case pe_media_date:
				case pe_media_duration:
				case pe_media_comment:
				case pe_media_creation_time:
				case pe_media_publisher:
				case pe_media_copyright:
				case pe_media_language:
				case pe_mail_sender:
				case pe_mail_from:
				case pe_mail_to:
				case pe_mail_subject:
				case pe_mail_replyto:
				case pe_mail_cc:
				case pe_mail_bcc:
					break;
				case pe_mail_is_mail:
					//bRtn = init_mail_is_mail(policy);
					break;
				case pe_mail_attachment_num:
					//bRtn = init_mail_attachment_num(policy);
					break;
				case pe_file_attr:
					break;
				case pe_database_hash:
					bRtn = init_data_hash(policy);
					break;

				default:
					;
			}

			return bRtn;
		}
		
		boolean init_data_hash(Map policy)
		{
			m_data_hash_row = (int)(double)policy.get("row");
			//m_data_hash_rol = (int)(double)policy.get("rol");
			m_data_hash_rol = 0;
			
			m_data_hash_name = (List<String>)policy.get("data_name");

			return true;
		}
		
		public boolean init_goods_counter_policy(Map policy)
		{
			m_counter_small_num = (int)(double)policy.get("small_num");
			String strType = (String)policy.get("count_type");
			if(strType.equals("equal"))
			{
				m_counter_type = 0;
			}
			else if(strType.equals("large"))
			{
				m_counter_type = 1;
			}
			else if(strType.equals("less"))
			{
				m_counter_type = 2;
			}
			else if(strType.equals("between"))
			{
				m_counter_type = 3;
				m_counter_big_num = (int)(double)policy.get("large_num");
			}
			else if(strType.equals("large_equal"))
			{
				m_counter_type = 4;
			}
			else if(strType.equals("less_equal"))
			{
				m_counter_type = 5;
			}
			else if(strType.equals("not_equal"))
			{
				m_counter_type = 6;
			}

			return true;
		}
		
		boolean init_file_hash_policy(Map policy)
		{
			m_file_hash_threshold = (int)(double)policy.get("threshold");
			String data_name = (String)policy.get("data-name");
			m_file_hash_data_name.add(data_name);
			return true;
		}
		
		
		
		boolean init_bayes_policy(Map policy)
		{
			String cl = (String)policy.get("class_list");
			String[] cl_s = cl.split("\\|");
			for(String one:cl_s)
			{
				m_bayes_class_name.add(one);
			}

			return true;
		}

		boolean init_protocol_policy(Map policy)
		{
			m_protocol = (String)policy.get("protocol");
			return true;
		}
		boolean init_in_addr_policy(Map policy)
		{
			m_i_addr = (String)policy.get("rule");
			return true;
		}
		boolean init_in_port_policy(Map policy)
		{
			String cl = (String)policy.get("port");
			String[] cl_s = cl.split("\\|");
			for(String one:cl_s)
			{
				m_i_port.add(Integer.parseInt(one));
			}

			return true;
		}
		boolean init_out_addr_policy(Map policy)
		{
			m_o_addr = (String)policy.get("rule");
			return true;
		}
		boolean init_out_port_policy(Map policy)
		{			
			String cl = (String)policy.get("port");
			String[] cl_s = cl.split("\\|");
			for(String one:cl_s)
			{
				m_o_port.add(Integer.parseInt(one));
			}

			return true;
		}
		boolean init_src_addr_policy(Map policy)
		{
			m_s_addr = (String)policy.get("rule");
			return true;
		}
		boolean init_src_port_policy(Map policy)
		{
			String cl = (String)policy.get("port");
			String[] cl_s = cl.split("\\|");
			for(String one:cl_s)
			{
				m_s_port.add(Integer.parseInt(one));
			}

			return true;
		}
		boolean init_src_host_policy(Map policy)
		{
			m_s_host = (String)policy.get("rule");
			return true;
		}
		boolean init_file_url_policy(Map policy)
		{
			m_file_url = (String)policy.get("rule");
			return true;
		}

		boolean init_doc_file(Map policy)
		{
			if((int)(double)policy.get("is_doc") == 0)
				m_doc_is_doc = false;
			else
				m_doc_is_doc = true;
			
			return true;
		}
		boolean init_doc_manager(Map policy)
		{
			m_doc_manager = (String)policy.get("manager");
			return true;
		}
		boolean init_doc_company(Map policy)
		{
			m_doc_company = (String)policy.get("company");
			return true;
		}
		boolean init_doc_title(Map policy)
		{
			m_doc_title = (String)policy.get("title");
			return true;
		}
		boolean init_doc_subject(Map policy)
		{
			m_doc_subject = (String)policy.get("subject");
			return true;
		}
		boolean init_doc_author(Map policy)
		{
			m_doc_author = (String)policy.get("author");
			return true;
		}
		boolean init_doc_keywords(Map policy)
		{
			m_doc_keywords = (String)policy.get("keywords");
			return true;
		}
		boolean init_doc_description(Map policy)
		{
			m_doc_description = (String)policy.get("description");
			return true;
		}
		boolean init_doc_lastModifieBy(Map policy)
		{
			m_doc_lastModifiedBy = (String)policy.get("lastModefieBy");
			return true;
		}
		boolean init_doc_modified_time(Map policy)
		{
			String compare_type = (String)policy.get("compare");

			if(compare_type == "before")
			{
				m_doc_modified_time_type = _TIME_TYPE.t_before;
				m_doc_modified_time_first = (long)(double)policy.get("first_time");
			}
			else if(compare_type == "after")
			{
				m_doc_modified_time_type = _TIME_TYPE.t_after;
				m_doc_modified_time_first = (long)(double)policy.get("first_time");
			}
			else if(compare_type == "between")
			{
				m_doc_modified_time_type = _TIME_TYPE.t_between;
				m_doc_modified_time_first = (long)(double)policy.get("first_time");
				m_doc_modified_time_second = (long)(double)policy.get("second_time");
			}
			else if(compare_type == "last")
			{
				m_doc_modified_time_type = _TIME_TYPE.t_last;
				m_doc_modified_time_first = (long)(double)policy.get("first_time");
			}
			else
				m_doc_modified_time_type = _TIME_TYPE.t_unknow;

			return true;
		}
		boolean init_doc_category(Map policy)
		{
			m_doc_category = (String)policy.get("category");
			return true;
		}
		boolean init_doc_header(Map policy)
		{
			m_doc_header = (String)policy.get("header");
			return true;
		}
		boolean init_doc_footer(Map policy)
		{
			m_doc_footer = (String)policy.get("footer");
			return true;
		}
		boolean init_doc_encrypted(Map policy)
		{
			if((int)(double)policy.get("is_encrypted") == 0)
				m_doc_is_encrypted = false;
			else
				m_doc_is_encrypted = true;
			return true;
		}
		boolean init_doc_extension(Map policy)
		{
			m_doc_extension = (String)policy.get("extension");
			return true;
		}
		boolean init_file_size(Map policy)
		{
			String type = (String)policy.get("compare");
			if(type == "less_than")
				m_file_size_type = _NUM_TYPE.n_less_then;
			else if(type == "great_than")
				m_file_size_type = _NUM_TYPE.n_great_then;
			else if(type == "between")
				m_file_size_type = _NUM_TYPE.n_between;
			else
				m_file_size_type = _NUM_TYPE.n_unknow;

			m_file_size_first = (int)(double)policy.get("first_size");
			m_file_size_second = (int)(double)policy.get("second_size");

			return true;
		}

		boolean init_mail_attachment_num(Map policy)
		{
			String type = (String)policy.get("compare");
			if(type == "less_than")
				m_mail_attachment_num_type = _NUM_TYPE.n_less_then;
			else if(type == "great_than")
				m_mail_attachment_num_type = _NUM_TYPE.n_great_then;
			else if(type == "between")
				m_mail_attachment_num_type = _NUM_TYPE.n_between;
			else if(type == "equal")
				m_mail_attachment_num_type = _NUM_TYPE.n_equal;
			else
				m_mail_attachment_num_type = _NUM_TYPE.n_unknow;

			m_mail_attachment_num_first = (int)(double)policy.get("first_size");
			m_mail_attachment_num_second = (int)(double)policy.get("second_size");

			return true;
		}

		boolean init_mail_is_mail(Map policy)
		{
			if((int)(double)policy.get("is_mail") == 0)
				m_mail_is_mail = false;
			else
				m_mail_is_mail= true;
			
			return true;
		}
}
