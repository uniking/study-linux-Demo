package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MessageBus {
	public String m_bin_file_path;
	public String m_txt_file_path;
	public String m_out_file_content;
	public byte[] m_out_file_buffer;
	
	public Map<String, Object> m_metaJson = new HashMap<String, Object>();
	
	public List<String> m_out_regular_words = new ArrayList<String>();
	
	public List<String> m_out_bank_card_16 = new ArrayList<String>();
	public List<String> m_out_bank_card_16_info = new ArrayList<String>();
	public List<String> m_out_bank_card_19 = new ArrayList<String>();
	public List<String> m_out_bank_card_19_info = new ArrayList<String>();
	
	public List<String> m_out_id_card_15 = new ArrayList<String>();
	public List<String> m_out_id_card_15_info = new ArrayList<String>();
	public List<String> m_out_id_card_18 = new ArrayList<String>();
	public List<String> m_out_id_card_18_info = new ArrayList<String>();
	
	public List<String> m_out_phone = new ArrayList<String>();
	public List<String> m_out_phone_info = new ArrayList<String>();
	
	
	
	public List<String> m_date_base_hash_info = new ArrayList<String>();
	
	
	public String m_type_category;
	
	public String m_o_meta_i_addr;
	public int m_o_meta_i_port;
	public String m_o_meta_o_addr;
	public int m_o_meta_o_port;
	public String m_o_meta_s_addr;
	public int m_o_meta_s_port;
	public String m_o_meta_s_host;
	public String m_o_meta_file_url;
	public String m_o_meta_protocol;
	public String m_file_org_name;
	
	//file
	public long m_file_size;
	public boolean m_doc_is_encrypted;
	public String m_doc_extension;
	public String m_type_alias;
	public String m_doc_category;
	
	//mail
	public String m_mail_sender;
	public String m_mail_from;
	public String m_mail_to;
	public String m_mail_subject;
	public String m_mail_replyto;
	public String m_mail_cc;
	public String m_mail_bcc;
	public boolean m_mail_is_mail;
	public int m_mail_attachment_num;
	
	
	//doc
	
	public boolean m_doc_is_doc;
	public String m_doc_manager;
	public String m_doc_company;
	public String m_doc_title;
	public String m_doc_subject;
	public String m_doc_author;
	public String m_doc_keywords;
	public String m_doc_description;
	public String m_doc_lastModifiedBy;
	public String m_doc_modified_time;
	public String m_doc_template;
	public String m_doc_header;
	public String m_doc_footer;
	
	void init(Map<String, Object> mapJson)
	{
		m_metaJson = mapJson;
		
		//net meta
		m_o_meta_i_addr = (String)mapJson.get("i_addr");
		m_o_meta_i_port = (int)(double)mapJson.get("i_port");
		m_o_meta_o_addr = (String)mapJson.get("o_addr");
		m_o_meta_o_port = (int)(double)mapJson.get("o_port");
		m_o_meta_s_addr = (String)mapJson.get("s_addr");
		m_o_meta_s_port = (int)(double)mapJson.get("s_port");
		m_o_meta_s_host = (String)mapJson.get("s_host");
		m_o_meta_file_url = (String)mapJson.get("file_url");
		m_o_meta_protocol = (String)mapJson.get("protocol");
		m_file_org_name = (String)mapJson.get("file_path");
		
		
		//file meta
		m_file_size = (long)(double)mapJson.get("size");
		if(mapJson.get("is_encrypted") != null)
			m_doc_is_encrypted = (boolean)mapJson.get("is_encrypted");
		m_doc_extension = (String)mapJson.get("default_extension");
		m_type_alias = (String)mapJson.get("type_alias");
		m_doc_category = (String)mapJson.get("category");
		
		//word, ppt, excel
		if(m_doc_category.equals("Document"))
			m_doc_is_doc = true;
		else
			m_doc_is_doc = false;
		m_doc_manager = (String)mapJson.get("manager");
		m_doc_company = (String)mapJson.get("company");
		m_doc_title = (String)mapJson.get("title");
		m_doc_subject = (String)mapJson.get("subject");
		m_doc_author = (String)mapJson.get("author");
		m_doc_keywords = (String)mapJson.get("keywords");
		m_doc_description = (String)mapJson.get("description");
		m_doc_lastModifiedBy = (String)mapJson.get("last_saved_by");
		m_doc_modified_time = (String)mapJson.get("modification_date");
		m_doc_template = (String)mapJson.get("template");
		m_doc_header = (String)mapJson.get("header");
		m_doc_footer = (String)mapJson.get("footer");
		
		//mail meta
		if(m_doc_category.equals("Mics"))
			m_mail_is_mail = true;
		else
			m_mail_is_mail = false;
		m_mail_sender = (String)mapJson.get("sender");
		m_mail_from = (String)mapJson.get("from");
		m_mail_to = (String)mapJson.get("to");
		m_mail_subject = (String)mapJson.get("subject");
		m_mail_replyto = (String)mapJson.get("replyto");
		m_mail_cc = (String)mapJson.get("cc");
		m_mail_bcc = (String)mapJson.get("bcc");
		if(mapJson.get("attachment") != null)
			m_mail_attachment_num = (int)(double)mapJson.get("attachment");
		
		//	
	}

}
