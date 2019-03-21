package myRDD;

import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.regex.Pattern;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.function.Function2;
import org.apache.spark.api.java.function.PairFunction;

import scala.Tuple2;

public class MyRDD {
	private static final Pattern SPACE = Pattern.compile(" ");
	
	public static void main(String[] args)
	{

	    if (args.length < 1) {
	      System.err.println("Usage: JavaWordCount <file>");
	      System.exit(1);
	    }

	    SparkConf sparkConf = new SparkConf().setAppName("wzl-JavaWordCount");
	    JavaSparkContext ctx = new JavaSparkContext(sparkConf);
	    JavaRDD<String> lines = ctx.textFile(args[0], 1);
	    
	    JavaRDD<RespondObj> words = lines.map(new DiscountRDD());
	    
	    JavaRDD<RequireObj> rq = words.map(new RDDrequire());
	    
	    List<RequireObj> rol = rq.collect();
	    
	    ctx.stop();
	  }
}