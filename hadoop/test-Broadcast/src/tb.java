import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Logger;
import java.util.regex.Pattern;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.broadcast.Broadcast;
import org.apache.spark.streaming.Duration;
import org.apache.spark.streaming.api.java.JavaDStream;
import org.apache.spark.streaming.api.java.JavaStreamingContext;

public class tb {
	static Logger logger = Logger.getLogger(tb.class.getName());
	static JavaStreamingContext jssc = null;
	static JavaDStream<String> jds = null;
	static JavaSparkContext sc = null;
	
	public static void test_init()
	{
		SparkConf sparkConf = new SparkConf().setAppName("wzl_tb");
		sc = new JavaSparkContext(sparkConf);
		jssc = new JavaStreamingContext(sc, new Duration(30000));
		jssc.checkpoint("hdfs://node1:9000/user/ubt/mydir");
		
  	  
		jds = jssc.textFileStream("hdfs://node1:9000/user/wzl/mdir");
		jds.checkpoint(new Duration(30000));
	}
	
	public static void test_spark(Broadcast<nameList> boh)  throws InterruptedException
	{
		JavaDStream<String> line = jds.flatMap(new FlatMapFunction<String, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) throws IOException 
			{	
				//System.out.println("flat map");
				List<String> tnl = boh.value().get_name();
				Iterator<String> iter = tnl.iterator();
				while(iter.hasNext())
				{
					String tmp = iter.next();
					logger.info(tmp);
					System.out.println(tmp);
				}
				
		    	  List<String> Content = new ArrayList<String>();
		    	  Content.add(s);
		    	  return Content.iterator();
			}
		});
		
		
		JavaDStream<Long> cds = line.count();
		cds.print();
		
		jssc.start();
		jssc.awaitTermination();
	}
	
	public static void test_uninit() throws InterruptedException
	{
	    jssc.close();
	}
	
	//不用broadcast， nameList中竟然没有数据， 复制代码，没有执行构造函数？？？？
	public static void test_spark2()  throws InterruptedException
	{
		SparkConf sparkConf = new SparkConf().setAppName("wzl_tb");
		JavaSparkContext sc = new JavaSparkContext(sparkConf);
		JavaStreamingContext jssc = new JavaStreamingContext(sc, new Duration(30000));
		jssc.checkpoint("hdfs://node1:9000/user/ubt/mydir");
		
  	  
		JavaDStream<String> jds = jssc.textFileStream("hdfs://node1:9000/user/wzl/mdir");
		jds.checkpoint(new Duration(30000));
		nameList nl = new nameList();
		
		JavaDStream<String> line = jds.flatMap(new FlatMapFunction<String, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) throws IOException 
			{	
				//System.out.println("flat map");
				List<String> tnl = nl.get_name();
				Iterator<String> iter = tnl.iterator();
				while(iter.hasNext())
				{
					logger.info(iter.next());
				}
				
		    	  List<String> Content = new ArrayList<String>();
		    	  Content.add(s);
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
		//test_spark2();
		
		test_init();
		
		Broadcast<nameList> boh = null;
		nameList nl = new nameList();
		boh = sc.broadcast(nl);
		
		
		test_spark(boh);
	}
}
