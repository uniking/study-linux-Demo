package kafka_message;

import java.util.HashMap;
import java.util.Map;

/*
 * 

{
	"name":"",
	"groupId":"",
	"messageType":"",
	"message":""
}

 * 
 */

public class MessageProtocol {
	Map<String, String> m_map = new HashMap<String, String>();
	
	public MessageProtocol(String mp) throws Exception
	{
		m_map = GJson.String2Map(mp);
	}
	
	public MessageProtocol(String name, String mt, String msg)
	{
		m_map.put("name", name);
		m_map.put("groupId", name);
		m_map.put("messageType", mt);
		m_map.put("message", msg);
		
	}
	
	String getName()
	{
		return m_map.get("name");
	}
	
	String getMessageType()
	{
		return m_map.get("messageType");
	}
	
	String getMessage()
	{
		return m_map.get("message");
	}
	
	String getMessageProtocol()
	{
		return GJson.Map2String(m_map);
	}

}
