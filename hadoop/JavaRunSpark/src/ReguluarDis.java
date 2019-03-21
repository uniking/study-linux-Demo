import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import java.io.Serializable;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;

public class ReguluarDis implements Serializable{
	boolean ExtractAllData(String regular, String FilePath, List<String> Content)
	{
	    SparkConf sparkConf = new SparkConf().setAppName("wzl-regular-distribution");
	    sparkConf.setMaster("spark://node1:7077");
	    sparkConf.set("SCALA_HOME", "/usr/share/scala-2.11");
	    sparkConf.set("HADOOP_HOME", "/home/ubt/hadoop_bin/hadoop-2.8.0");
	    sparkConf.set("LD_LIBRARY_PATH", "/home/ubt/hadoop_bin/hadoop-2.8.0/lib/native/");
	    
	    sparkConf.set("fs.hdfs.impl", "org.apache.hadoop.hdfs.DistributedFileSystem");
	    sparkConf.set("fs.file.impl", "org.apache.hadoop.fs.LocalFileSystem");
	    
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
}
