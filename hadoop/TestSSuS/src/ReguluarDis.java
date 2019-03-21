import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.streaming.api.java.JavaDStream;
import org.apache.spark.streaming.api.java.JavaStreamingContext;

public class ReguluarDis  implements Serializable{
	boolean ExtractAllData2(String regular, String FilePath, List<String> Content)
	{
	    SparkConf sparkConf = new SparkConf().setAppName("wzl-regular-distribution");
	    sparkConf.setMaster("spark://node1:7077");
	    
	    JavaSparkContext ctx = new JavaSparkContext(sparkConf);
	    JavaRDD<String> lines = ctx.textFile(FilePath, 1);
	    JavaRDD<String> words = lines.flatMap(new FlatMapFunction<String, String>() {
	      private int group;
	  	  private Pattern pattern;
	      @Override
	      public Iterator<String> call(String s) 
	      {
	    	  /*
	    	  group = 0;
	    	  pattern = Pattern.compile(regular);
	    	  List<String> ar = new ArrayList<String>();
		    	Matcher matcher = pattern.matcher(s);
		    	while (matcher.find())
		    		ar.add(matcher.group(group));
		    	return ar.iterator();
		    	*/
	    	  
	    	  System.out.println(s);
	    	  List<String> ar = new ArrayList<String>();
	    	  ar.add(s);
	    	  return ar.iterator();
	      }
	    });
	    
	    Content = words.collect();
	    
	    ctx.stop();
	    if(Content.size() >0)
	    	return true;
	    return false;
	}
	
	
	public static boolean ExtractAllData(JavaStreamingContext jssc, String regular, String FilePath, List<String> Content)
	{
		//JavaDStream<String> jds = jssc.textFileStream(FilePath);
		//jds.print();
	    return false;
	}
	
	public static boolean ExtractAllData3(String regular, String strSplit, List<String> Content)
	{
      int group=0;
  	  Pattern pattern;
  	  
  pattern = Pattern.compile(regular);
  List<String> ar = new ArrayList<String>();
    	Matcher matcher = pattern.matcher(strSplit);
    	while (matcher.find())
    		ar.add(matcher.group(group));
    	
	    if(Content.size() >0)
	    	return true;
	    return false;
	}
}
