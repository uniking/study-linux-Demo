package com.suninfo.cloud.ndlp.discover;

import java.io.IOException;
import java.text.ParseException;
import java.util.Map;

public class PolicyExecutor{

	Regular m_regular = new Regular();
	GoodsCounter m_goods_counter = new GoodsCounter();
	RegularBankCard m_credit_card = new RegularBankCard("/user/wzl/dic/bankcardinfor.txt");
	IdentityCard m_identity_card = new IdentityCard("/user/wzl/dic/idcardinfor.txt");
	Phone m_phone = new Phone("/user/wzl/dic/phoneinfor.txt");
	DataBaseHash m_db_hash = new DataBaseHash();
	FuzzyHash m_file_hash = new FuzzyHash();
	MetaFile m_meta = new MetaFile();
	DocExecutor m_doc = new DocExecutor();
	MailExecutor m_mail = new MailExecutor();
	
	boolean execut_policy(MessageBus mb, PolicyObject policy, Map log)
	{
		boolean bRtn = false;
		Map reg_log;

		switch (policy.m_policy_type)
		{
			case pe_file_extract:
				//vector< CFileInfo* > mulFileInfo;
				//bRtn = m_text_extracter->doctotxtmul(mb.m_in_file_path, mulFileInfo);
				break;
			case pe_regedit:
				{
					switch(policy.m_input_type)
					{
						case pe_file_extract:
						{
							bRtn = m_regular.ExtractAllData(policy, mb.m_out_file_content, mb.m_out_regular_words, log);
						}
						break;
						
						case pe_file_buffer:
						{
							
						}
						break;
						
					default:
						;
					}
				}
				break;
			case pe_bayes:
				break;
			case pe_phone:
				{
					switch(policy.m_input_type)
					{
						case pe_regedit:
							{
								bRtn = m_phone.isValidPhoneNumber(policy, mb.m_out_regular_words, mb.m_out_phone, mb.m_out_phone_info, log);
							}
							break;
						default:
						;
					}

				}
				break;
			case pe_id_card_15:
				{
					switch(policy.m_input_type)
					{
						case pe_regedit:
							{
								bRtn = m_identity_card.IsValideIdentityCard(policy, mb.m_out_regular_words, mb.m_out_id_card_15, mb.m_out_id_card_15_info, log);
							}
							break;
						default:
						;
					}

				}
				break;
			case pe_id_card_18:
				{
					switch(policy.m_input_type)
					{
						case pe_regedit:
							{
								bRtn = m_identity_card.IsValideIdentityCard(policy, mb.m_out_regular_words, mb.m_out_id_card_18, mb.m_out_id_card_18_info, log);
							}
							break;
						default:
						;
					}

				}
				break;
			case pe_bank_card_16:
				{
					switch(policy.m_input_type)
					{
						case pe_regedit:
							{
								bRtn = m_credit_card.IsValideCreditCard(policy, mb.m_out_regular_words, mb.m_out_bank_card_16, mb.m_out_bank_card_16_info, log);
							}
							break;
						default:
						;
					}

				}
				break;
			case pe_bank_card_19:
				{
					switch(policy.m_input_type)
					{
						case pe_regedit:
							{
								bRtn = m_credit_card.IsValideCreditCard(policy, mb.m_out_regular_words, mb.m_out_bank_card_19, mb.m_out_bank_card_19_info, log);
							}
							break;
						default:
						;
					}

				}
				break;
			case pe_goods_counter:
				{
					int count=0;
					switch(policy.m_input_type)
					{
						case pe_regedit:
							{
								bRtn = m_goods_counter.count_goods(policy, mb.m_out_regular_words, count, log);
							}
							break;
						case pe_phone:
							{
								bRtn = m_goods_counter.count_goods(policy, mb.m_out_phone, count, log);
							}
							break;
						case pe_id_card_15:
							{
								bRtn = m_goods_counter.count_goods(policy, mb.m_out_id_card_15, count, log);
							}
							break;
						case pe_id_card_18:
							{
								bRtn = m_goods_counter.count_goods(policy, mb.m_out_id_card_18, count, log);
							}
							break;
						case pe_bank_card_16:
							{
								bRtn = m_goods_counter.count_goods(policy, mb.m_out_bank_card_16, count, log);
							}
							break;
						case pe_bank_card_19:
							{
								bRtn = m_goods_counter.count_goods(policy, mb.m_out_bank_card_19, count, log);
							}
							break;
						case pe_database_hash:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_date_base_hash_info, count, log);
							}
							break;
						default:
						;
					}

				}
				break;
			case pe_goods_sum_counter:
				{
					int count=0;
					switch(policy.m_input_type)
					{
						case pe_regedit:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_out_regular_words, mb.m_sum_counter, log);
							}
							break;
						case pe_phone:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_out_phone, mb.m_sum_counter, log);
							}
							break;
						case pe_id_card_15:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_out_id_card_15, mb.m_sum_counter, log);
							}
							break;
						case pe_id_card_18:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_out_id_card_18, mb.m_sum_counter, log);
							}
							break;
						case pe_bank_card_16:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_out_bank_card_16, mb.m_sum_counter, log);
							}
							break;
						case pe_bank_card_19:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_out_bank_card_19, mb.m_sum_counter, log);
							}
							break;
						case pe_database_hash:
							{
								//bRtn = m_goods_sum_counter.count_goods(policy, mb.m_date_base_hash_info, mb.m_sum_counter, log);
							}
							break;

						default:
							;
					}

				}
				break;
			case pe_file_hash:
				bRtn = m_file_hash.GetDocInfor(policy, mb, log);
				break;
			case pe_meta_i_addr:
				bRtn = m_meta.IsValidInAddr(policy, mb.m_o_meta_i_addr, log);
				break;
			case pe_meta_i_port:
				bRtn = m_meta.IsValidInPort(policy, mb.m_o_meta_i_port, log);
				break;
			case pe_meta_o_addr:
				bRtn = m_meta.IsValidOutAddr(policy, mb.m_o_meta_o_addr, log);
				break;
			case pe_meta_o_port:
				bRtn = m_meta.IsValidOutPort(policy, mb.m_o_meta_o_port, log);
				break;
			case pe_meta_s_addr:
				bRtn = m_meta.IsValidSrcAddr(policy, mb.m_o_meta_s_addr, log);
				break;
			case pe_meta_s_port:
				bRtn = m_meta.IsValidSrcPort(policy, mb.m_o_meta_s_port, log);
				break;
			case pe_meta_s_host:
				bRtn = m_meta.IsValidSrcHost(policy, mb.m_o_meta_s_host, log);
				break;
			case pe_meta_file_url:
				bRtn = m_meta.IsValidFileUrl(policy, mb.m_o_meta_file_url, log);
				break;
			case pe_meta_file_protocol:
				bRtn = m_meta.IsValidProtocol(policy, mb.m_o_meta_protocol, log);
				break;
			case pe_meta_file_name:
				//bRtn = m_meta.IsValidFileName(policy, mb.m_o_meta_file_name, log);
				bRtn = m_meta.IsValidFileName(policy, mb.m_file_org_name, log);
				break;
				
				
				//////////////////////////////////////////////////////////////////////////////////
			case pe_file_size:
				bRtn = m_doc.is_doc_file_size(policy, mb.m_file_size, log);
				break;
			case pe_doc_file:
				bRtn = m_doc.is_doc_file(policy, mb.m_doc_is_doc, log);
				break;
			case pe_doc_manager:
				bRtn = m_doc.is_doc_manager(policy, mb.m_doc_manager, log);
				break;
			case pe_doc_company:
				bRtn = m_doc.is_doc_company(policy, mb.m_doc_company, log);
				break;
			case pe_doc_title:
				bRtn = m_doc.is_doc_title(policy, mb.m_doc_title, log);
				break;
			case pe_doc_subject:
				bRtn = m_doc.is_doc_subject(policy, mb.m_doc_subject, log);
				break;
			case pe_doc_author:
				bRtn = m_doc.is_doc_author(policy, mb.m_doc_author, log);
				break;
			case pe_doc_keywords:
				bRtn = m_doc.is_doc_keywords(policy, mb.m_doc_keywords, log);
				break;
			case pe_doc_description:
				bRtn = m_doc.is_doc_description(policy, mb.m_doc_description, log);
				break;
			case pe_doc_lastModifieBy:
				bRtn = m_doc.is_doc_lastModifieBy(policy, mb.m_doc_lastModifiedBy, log);
				break;
			case pe_doc_modified_time:
			try {
				bRtn = m_doc.is_doc_modified_time(policy, mb.m_doc_modified_time, log);
			} catch (ParseException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
				break;
			case pe_doc_category:
				bRtn = m_doc.is_doc_category(policy, mb.m_doc_category, log);
				break;
			case pe_doc_template:
				bRtn = m_doc.is_doc_template(policy, mb.m_doc_template, log);
				break;
			case pe_doc_header:
				bRtn = m_doc.is_doc_header(policy, mb.m_doc_header, log);
				break;
			case pe_doc_footer:
				bRtn = m_doc.is_doc_footer(policy, mb.m_doc_footer, log);
				break;
			case pe_doc_encrypted:
				bRtn = m_doc.is_doc_encrypted(policy, mb.m_doc_is_encrypted, log);
				break;
			case pe_doc_extension:
				bRtn = m_doc.is_doc_extension(policy, mb.m_doc_extension, log);
				break;
			case pe_doc_type:
				bRtn = m_doc.is_doc_type(policy, mb.m_type_alias, log);
				break;
			case pe_media_is_media:
				//bRtn = m_media.is_media_file(policy, mb.m_media_is_media, log);
				break;
			case pe_media_track:
				break;
			case pe_media_bitrate:
				break;
			case pe_media_disc:
				break;
			case pe_media_genre:
				break;
			case pe_media_isrc:
				break;
			case pe_media_encoder:
				break;
			case pe_media_album_artlist:
				break;
			case pe_media_album:
				break;
			case pe_media_artlist:
				break;
			case pe_media_title:
				break;
			case pe_media_date:
				break;
			case pe_media_duration:
				break;
			case pe_media_comment:
				break;
			case pe_media_creation_time:
				break;
			case pe_media_publisher:
				break;
			case pe_media_copyright:
				break;
			case pe_media_language:
				break;
			case pe_mail_sender:
				bRtn = m_mail.is_mail_sender(policy, mb.m_mail_sender, log);
				break;
			case pe_mail_from:
				bRtn = m_mail.is_mail_from(policy, mb.m_mail_from, log);
				break;
			case pe_mail_to:
				bRtn = m_mail.is_mail_to(policy, mb.m_mail_to, log);
				break;
			case pe_mail_subject:
				bRtn = m_mail.is_mail_subject(policy, mb.m_mail_subject, log);
				break;
			case pe_mail_replyto:
				bRtn = m_mail.is_mail_replyto(policy, mb.m_mail_replyto, log);
				break;
			case pe_mail_cc:
				bRtn = m_mail.is_mail_cc(policy, mb.m_mail_cc, log);
				break;
			case pe_mail_bcc:
				bRtn = m_mail.is_mail_bcc(policy, mb.m_mail_bcc, log);
				break;
			case pe_mail_is_mail:
				bRtn = m_mail.is_mail_file(policy, mb.m_mail_is_mail, log);
				break;
			case pe_mail_attachment_num:
				bRtn = m_mail.is_mail_attachment_num(policy, mb.m_mail_attachment_num, log);
				break;
				
				
			case pe_database_hash:
			try {
				//bRtn = m_db_hash.find_hash(policy, mb.m_out_file_content, mb.m_date_base_hash_info, log);
				bRtn = m_db_hash.find_hash(policy, mb.m_bin_file_path, mb.m_date_base_hash_info, log);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
				break;
			default:
			;
		}

		System.out.println("execut_policy result:"+bRtn);
		return bRtn;
	}
}
