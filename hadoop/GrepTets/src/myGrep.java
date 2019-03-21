import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class myGrep {
	
	  public static void main(String[] args) throws Exception 
	  {
			Pattern pattern;
			int group;
			String GROUP = "mapreduce.mapper.regexmapper..group";
			
		    group = 0;
		    pattern = Pattern.compile("(abc)");
		    
	    	Matcher matcher = pattern.matcher("abc kdsj lksdjf abc kldsjfl kjk");
	    	
	    	List<String> ar = new ArrayList<String>();
	    	while (matcher.find())
	    	{
	    		System.out.println(matcher.group(group));
	    		ar.add(matcher.group(group));
	    	}
	    	
	    	//Iterator<String> it = ar.iterator();
	    	//while(it.hasNext())
	    	//	System.out.println(it.next());
	    	
		  //System.out.println("aaaaa");
	  }

}
