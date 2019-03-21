import java.io.Serializable;
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

public class sparkC
{

	public static String GROUP = "mapreduce.mapper.regexmapper..group";
	private static final Pattern SPACE = Pattern.compile(" ");

	static SparkConf sparkConf;
	static JavaSparkContext ctx;

	public static boolean init()
	{
		sparkConf = new SparkConf().setAppName("wzl-JavaWordCount");
		ctx = new JavaSparkContext(sparkConf);

		return true;
	}

	public void uninit()
	{
		ctx.stop();
	}

	public static String wc(String filepath) throws Exception
	{
		if(sparkConf == null)
		{
			System.out.println("reinit spark context");
			init();
		}

		JavaRDD<String> lines = ctx.textFile(filepath, 1);
		unserialize obj = new unserialize(); //can do
		obj.add(100, 10); //can do

		JavaRDD<String> words = lines.flatMap(new FlatMapFunction<String, String>()
			{
				//unserialize obj2 = new unserialize(); //can't new

				int tt = obj.add(100, 99);//can do!

				private int group;
				private Pattern pattern;
				@Override 
				public Iterator<String> call(String s) 
				{//分布式闭包的起点？，闭包之外属于driver， 闭包之内属于executor
					//obj.add(12, 43); //can't
					//obj2.add(33,44);//can't
					unserialize obj3 = new unserialize();
					obj3.add(100, 10);
					return Arrays.asList(SPACE.split(s)).iterator();
				}
			});
		unserialize obj4 = new unserialize(); //can do
		obj4.add(100, 10); //can do

		JavaPairRDD<String, Integer> ones = words.mapToPair(new PairFunction<String, String, Integer>()
			{
				@Override
				public Tuple2<String, Integer> call(String s)
				{
					return new Tuple2<String, Integer>(s, 1);
				}
			});

		JavaPairRDD<String, Integer> counts = ones.reduceByKey(new Function2<Integer, Integer, Integer>()
			{
				@Override
				public Integer call(Integer i1, Integer i2)
				{
					return i1 + i2;
				}
			});

		StringBuffer log = new StringBuffer();
		List<Tuple2<String, Integer>> output = counts.collect();
		for (Tuple2<?,?> tuple : output)
		{
			//System.out.println(tuple._1() + ": " + tuple._2());
			log.append(tuple._1() + " " + tuple._2() + " || ");
		}

		ctx.emptyRDD();

		return log.toString();
	}
}
