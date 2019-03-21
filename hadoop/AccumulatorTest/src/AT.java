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

public class AT {
	
	public static String GROUP = "mapreduce.mapper.regexmapper..group";
  private static final Pattern SPACE = Pattern.compile(" ");
  
  
  private static void print(String s)
  {
	  System.out.print(s);
  }


  public static void main(String[] args) throws Exception {

    if (args.length < 1) {
      System.err.println("Usage: JavaWordCount <file>");
      System.exit(1);
    }

    SparkConf sparkConf = new SparkConf().setAppName("wzl-JavaWordCount");
    JavaSparkContext ctx = new JavaSparkContext(sparkConf);
    JavaRDD<String> lines = ctx.textFile(args[0], 1);
    
    MyAcc tma = new MyAcc();
    JavaRDD<String> words = lines.flatMap(new FlatMapFunction<String, String>() {
    	
    	private int group;
  	  private Pattern pattern;
      @Override
      public Iterator<String> call(String s) 
      {
    	  tma.add(new Integer(1));
    	  System.out.println("tma:" + tma.value());
        return Arrays.asList(SPACE.split(s)).iterator();
      }
    });

    JavaPairRDD<String, Integer> ones = words.mapToPair(new PairFunction<String, String, Integer>() {
      @Override
      public Tuple2<String, Integer> call(String s) {
        return new Tuple2<String, Integer>(s, 1);
      }
    });

    JavaPairRDD<String, Integer> counts = ones.reduceByKey(new Function2<Integer, Integer, Integer>() {
      @Override
      public Integer call(Integer i1, Integer i2) {
        return i1 + i2;
      }
    });

    List<Tuple2<String, Integer>> output = counts.collect();
    for (Tuple2<?,?> tuple : output) {
      System.out.println(tuple._1() + ": " + tuple._2());
    }
    
    System.out.println("tma:" + tma.value());
    ctx.stop();
  }
}
