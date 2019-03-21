package kafka_message;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;

public class GetMsgThread extends Observable implements Runnable{
	boolean m_run = true;
	String m_name;
	MConsumer mc = null;
	List<String> m_msg = new ArrayList<String>();
	
	GetMsgThread(String name)
	{
		m_run = true;
		m_name = name;
		List<String> topics = new ArrayList<String>();
		topics.add("ndlp_msg");
		mc = new MConsumer(name, topics);
	}
	
	GetMsgThread(String ip, String name)
	{
		m_run = true;
		m_name = name;
		List<String> topics = new ArrayList<String>();
		topics.add("ndlp_msg");
		mc = new MConsumer(ip, name, topics);
	}
	
	void setUserName(String name)
	{
		m_name = name;
	}
	
	void getMsg(List<String> msg)
	{
		System.out.println("getMsg m_msg:" + m_msg.size());
		for(String one:m_msg)
			msg.add(one);
	}
	
	void stopServer()
	{
		mc.close();
		try {
			Thread.sleep(2000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		m_run = false;
		System.out.println("stopServer m_rum == " + m_run);
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		
		while(m_run)
		{
			//System.out.println("GetMsgThread run==" + m_run);
			
			try {
				List<String> msg = new ArrayList<String>();
				mc.getMessage(msg);
				if(msg.size() > 0)
				{
					m_msg = msg;
					System.out.println("notifyObservers:" + msg.size());
					super.setChanged();
					this.notifyObservers();
				}
				
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		System.out.println("GetMsgThread run exit");
	}

}
