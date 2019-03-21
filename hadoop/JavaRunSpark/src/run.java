import java.util.ArrayList;
import java.util.List;

public class run {
	public static void main(String[] args) throws Exception
	{
		List<String> Content = new ArrayList<String>();
		ReguluarDis rd = new ReguluarDis();
		rd.ExtractAllData("abc", "hdfs://node1:9000/user/ubt/WordCount/input/hadoop-policy.xml", Content);
	}

}
