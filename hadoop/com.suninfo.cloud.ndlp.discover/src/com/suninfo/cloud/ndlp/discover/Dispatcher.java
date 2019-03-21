package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

import org.apache.hadoop.io.IntWritable;
import org.apache.log4j.Logger;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.function.Function2;
import org.apache.spark.api.java.function.PairFunction;

import scala.Tuple2;

public class Dispatcher
{
	public static String GROUP = "mapreduce.mapper.discover..group";
	private static final Pattern SPACE = Pattern.compile(" ");

	static SparkConf sparkConf;
	static JavaSparkContext ctx;

	static Logger logger = Logger.getLogger(Dispatcher.class.getName());

	public static boolean init()
	{
		sparkConf = new SparkConf().setAppName("ndlp.discover");
		ctx = new JavaSparkContext(sparkConf);
		return true;
	}

	public void uninit()
	{
		ctx.stop();
	}


	public static boolean discover_file(List<String> meta_path, List<String> log) throws Exception
	{
		if(sparkConf == null)
		{
			System.out.println("reinit spark context");
			init();
		}

		JavaRDD<String> lines = ctx.parallelize(meta_path);
		JavaRDD<String> words = lines.flatMap(new FlatMapFunction<String, String>()
			{
				PolicyManager pm;

				@Override
				public Iterator<String> call(String s) 
				{
					if(pm == null)
					{
						pm = new PolicyManager();
						try {
							pm.load_policy("/user/wzl/policy");
						} catch (Exception e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}

					// List<Map> dlog = new ArrayList<Map>();
					List<String> lLog = new ArrayList<String>();
					try
					{
						Map dlog = new HashMap<Object, Object>();
						if(pm.discover(s, dlog))
						{
							String strLog = GJson.Map2String(dlog);
							lLog.add(strLog);
						}
					}
					catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}


					//return Arrays.asList(SPACE.split(s)).iterator();
					return lLog.iterator();
				}
			});


		JavaPairRDD<String, Integer> ones = words.mapToPair(new PairFunction<String, String, Integer>()
		{
			@Override
			public Tuple2<String, Integer> call(String s) {
				return new Tuple2<String, Integer>(s, 1);
				}
		});

		JavaPairRDD<String, Integer> counts = ones.reduceByKey(new Function2<Integer, Integer, Integer>()
		{
			@Override
			public Integer call(Integer i1, Integer i2) {
				return i1 + i2;
				}
		});

		List<Tuple2<String, Integer>> output = counts.collect();
		for (Tuple2<?,?> tuple : output)
		{
			//System.out.println(tuple._1() + ": " + tuple._2());

			//System.out.println( tuple.productPrefix() + tuple.productArity() );
			log.add( (String)tuple._1() );
			System.out.println("log size:" + log.size());
		}

		ctx.emptyRDD();

		if(log.size() > 0)
			return true;
		return false;
	}

}
