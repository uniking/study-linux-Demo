import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.logging.Logger;
import java.util.regex.Pattern;

import org.apache.spark.SparkConf;
import org.apache.spark.SparkContext;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.broadcast.Broadcast;
import org.apache.spark.streaming.Duration;
import org.apache.spark.streaming.api.java.JavaDStream;
import org.apache.spark.streaming.api.java.JavaStreamingContext;
import org.apache.spark.internal.Logging;

public class test_hdfs {
	static oper_hdfs oh = new oper_hdfs(); //被复制到每一个分区中
	static Logger logger = Logger.getLogger(test_hdfs.class.getName());
	
	public static void test_spark()  throws InterruptedException
	{
		SparkConf sparkConf = new SparkConf().setAppName("wzl_hs");
		JavaStreamingContext jssc = new JavaStreamingContext(sparkConf, new Duration(30000));
		jssc.checkpoint("hdfs://node1:9000/user/ubt/mydir");
  	  
		JavaDStream<String> jds = jssc.textFileStream("hdfs://node1:9000/user/wzl/mdir");
		jds.checkpoint(new Duration(30000));
		
		//spark streaming的每一个批处理都会触发一次
		JavaDStream<String> ll = jds.mapPartitions(new FlatMapFunction<Iterator<String>, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(Iterator<String> its) throws IOException 
			{
				logger.info("----------------map partitions----------");
					oh.initRemoteHdfs();
		    	  return its;
		    	  
			}
		});

		JavaDStream<String> line = ll.flatMap(new FlatMapFunction<String, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) throws IOException 
			{
				//System.out.println("flat map");
				String c = oh.readFile("/user/wzl/cleanFile2");
				logger.info(c);
				
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
	
	public static void test_spark2()  throws InterruptedException
	{
		SparkConf sparkConf = new SparkConf().setAppName("wzl_hs");
		JavaStreamingContext jssc = new JavaStreamingContext(sparkConf, new Duration(30000));
		jssc.checkpoint("hdfs://node1:9000/user/ubt/mydir");
		
  	  
		JavaDStream<String> jds = jssc.textFileStream("hdfs://node1:9000/user/wzl/mdir");
		jds.checkpoint(new Duration(30000));
		
		oh.initRemoteHdfs();

		JavaDStream<String> line = jds.flatMap(new FlatMapFunction<String, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) throws IOException 
			{
				if(!oh.isOk())
				{
					oh.initRemoteHdfs();
					logger.info("----------init remove hdfs");
				}
				
				//System.out.println("flat map");
				String c = oh.readFile("/user/wzl/cleanFile2");
				logger.info(c);
				
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
	
	public static void test_spark3()  throws InterruptedException
	{
		SparkConf sparkConf = new SparkConf().setAppName("wzl_hs");
		JavaSparkContext sc = new JavaSparkContext(sparkConf);
		JavaStreamingContext jssc = new JavaStreamingContext(sc, new Duration(30000));
		jssc.checkpoint("hdfs://node1:9000/user/ubt/mydir");
		
  	  
		JavaDStream<String> jds = jssc.textFileStream("hdfs://node1:9000/user/wzl/mdir");
		jds.checkpoint(new Duration(30000));
		
		oper_hdfs ohh = new oper_hdfs();
		ohh.initRemoteHdfs();
		Broadcast<oper_hdfs> boh = sc.broadcast(ohh);
		
		

		JavaDStream<String> line = jds.flatMap(new FlatMapFunction<String, String>() 
		{
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) throws IOException 
			{	
				//System.out.println("flat map");
				String c = boh.value().readFile("/user/wzl/cleanFile2");
				logger.info(c);
				
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
		test_spark3();
	}
}
