import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class nameList implements Serializable{
	
	List<String> m_name = new ArrayList<String>();
	
	public nameList()
	{
		m_name.add("xiao ming");
		m_name.add("xiao fang");
		m_name.add("lao wang");
	}
	
	public List<String> get_name()
	{
		return m_name;
	}
}
