import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.streaming.Duration;
import org.apache.spark.streaming.Time;
import org.apache.spark.streaming.api.java.JavaDStream;
import org.apache.spark.streaming.api.java.JavaPairReceiverInputDStream;
import org.apache.spark.streaming.api.java.JavaStreamingContext;



public class SSuS implements Serializable{
	
	public static void test_spark()  throws InterruptedException
	{
		SparkConf sparkConf = new SparkConf().setAppName("wzl_MD");
		JavaStreamingContext jssc = new JavaStreamingContext(sparkConf, new Duration(3000));
		jssc.checkpoint("hdfs://node1:9000/user/ubt/mydir");
		
  	  	//JavaDStream<String> mjds = jssc.textFileStream("hdfs://node1:9000/user/ubt/WordCount/input/hadoop-policy.xml");
  	  	//mjds.print();
  	  
		JavaDStream<String> jds = jssc.textFileStream("hdfs://node1:9000/user/ubt/mydir");
		jds.checkpoint(new Duration(3000));

		JavaDStream<String> line = jds.flatMap(new FlatMapFunction<String, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) 
			{
		    	  System.out.println("---------------------map------------------------");
		    	  

		    	  //******* not regedit in this !!!
		    	  //List<String> Content = new ArrayList<String>();
		    	  //ReguluarDis rd = new ReguluarDis();
		    	  //test distribution
		    	  //ReguluarDis.ExtractAllData(jssc, "abc", "hdfs://node1:9000/user/ubt/WordCount/input/hadoop-policy.xml", Content);
		    	  //String a=new String();
		    	  //return Content.iterator();
		    	  
		    	  
		    	  
		    	  List<String> Content = new ArrayList<String>();
		    	  Content.add(s);
		    	  return Content.iterator();
		    	  
			}
		});
		
		String PolicyReg = "abc";
		JavaDStream<String> reg =  line.flatMap(new FlatMapFunction<String, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) 
			{
		    	  System.out.println("---------------------map------------------------");		    	  
		    	  List<String> Content = new ArrayList<String>();
		    	  ReguluarDis rd = new ReguluarDis();
		    	  rd.ExtractAllData3(PolicyReg, s, Content);
		    	  return Content.iterator();  
			}
		});
		
		JavaDStream<Long> cds = line.count();
		cds.print();
		
	    jssc.start();
	    jssc.awaitTermination();
	    jssc.close();
	}
	
	public static void main(String[] args) throws Exception
	{
		test_spark();
	}

}