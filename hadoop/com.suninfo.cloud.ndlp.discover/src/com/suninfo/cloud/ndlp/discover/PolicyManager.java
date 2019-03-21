package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class PolicyManager {
	//private CPolicyExecutor* m_policy_executor;
	List<PolicyGroup> m_list = new ArrayList<PolicyGroup>();
	//CExtractText m_eText;
	//CMetaFile m_metaFile;
	//bool m_policy_lock_ok;
	//pthread_rwlock_t m_policy_lock;
	String m_policy_path;
	boolean m_offline;//speed lower
	
	PolicyManager()
	{
		
	}
	
	private void clean_policy()
	{
		
	}
	
	//void clean_policy(list<CPolicyGroup*>& policy_list);
	
	boolean init_policy_lock()
	{
		return false;
	}
	
	void uninit_poicy_lock()
	{
		
	}
	
	void get_read_policy_lock()
	{
		
	}
	
	void release_read_policy_lock()
	{
		
	}
	
	void get_write_policy_lock()
	{
		
	}
	
	void release_write_policy_lock()
	{
		
	}
	
	void insert_policy(PolicyGroup policy_group)
	{
		m_list.add(policy_group);
	}
	
	void sort_policy()
	{
		
	}
	
	//public:
		//CPolicyManager();
		//CPolicyManager(bool offline);
		//~CPolicyManager();
		void load_policy(String policy_path)  throws Exception
		{
			List<String> filelist = new ArrayList<String>();
			//util.traversal_diretory(policy_path, filelist );
			oper_hdfs oh = new oper_hdfs();
			oh.initRemoteHdfs();
			oh.traversal_diretory(policy_path, filelist);
			
			Iterator iter = filelist.iterator();
			while(iter.hasNext())
			{
				//Map json_policy = GJson.File2Map((String)iter.next());
				Map json_policy = GJson.File2Map_hdfs((String)iter.next());
				PolicyGroup pg = new PolicyGroup();
				System.out.println(json_policy);
				if(pg.init_json_policy(json_policy) )
				{
					System.out.println("group:"+pg.get_name());
					insert_policy(pg);
				}
			}
				
		}
		
		void reload_policy(String policy_path)
		{
			
		}
		void create_executor()
		{
			
		}
		void update_executor()
		{
			
		}

		boolean discover_one_file_temp_policy(List<PolicyGroup> policy_list, MessageBus mb, Map log)
		{
			boolean bRtn = false;
			Iterator<PolicyGroup> iter = policy_list.iterator();
			while(iter.hasNext())
			{
				PolicyGroup tmp_pg = iter.next();
				bRtn = tmp_pg.execute_group(mb, log);
				if (bRtn)
				{
					break;
				}
			}

			return bRtn;
		}
		
		boolean discover(String meta_path, Map log) throws Exception
		{
			boolean bRtn = false;
			System.out.println("pm:discover:" + meta_path);

			//meta
			//Map metaJson = (Map)GJson.File2Map(meta_path);
			Map metaJson = (Map)GJson.File2Map_hdfs(meta_path);
			String file_path = (String)metaJson.get("file_path");
			String file_name = (String)metaJson.get("file_name");
			String cotent_path = (String)metaJson.get("content");
			System.out.println("file_path:"+file_path+" file_name:"+file_name);
			
			log.put("file_path", file_path);

			//full messagebus
			MessageBus mb = new MessageBus();
			mb.init(metaJson);
			mb.m_bin_file_path = file_path;
			mb.m_txt_file_path = cotent_path;
			if(null != mb.m_txt_file_path)
			{
				oper_hdfs oh = new oper_hdfs();
				oh.initRemoteHdfs();
				mb.m_out_file_content = oh.readFile(mb.m_txt_file_path);
				//System.out.println(mb.m_out_file_content);
			}
			
			//process policy
			bRtn = discover_one_file_temp_policy(m_list, mb, log);
			return bRtn;
		}
		
		void set_policy_path(String path)
		{
			m_policy_path = path;
		}
		
		boolean have_policy()
		{
			return false;
		}
		
		void cp_policy(List<PolicyGroup> src_policy, List<PolicyGroup> des_policy)
		{
			
		}
}
