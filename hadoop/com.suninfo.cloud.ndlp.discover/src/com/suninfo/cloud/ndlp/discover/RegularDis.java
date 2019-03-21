package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.function.Function2;
import org.apache.spark.api.java.function.PairFunction;

import scala.Tuple2;


/*
 * file size > 1M
 * regular for distribution
 */
public class RegularDis {
	boolean ExtractAllData(PolicyObject policy, String FilePath, List<String> Content, Map log)
	{
		SparkConf sparkConf = new SparkConf().setAppName("wzl-regular-distribution");
		JavaSparkContext ctx = new JavaSparkContext(sparkConf);
		JavaRDD<String> lines = ctx.textFile(FilePath, 1);
		JavaRDD<String> words = lines.flatMap(new FlatMapFunction<String, String>(){
			private int group;
			private Pattern pattern;
			@Override
			public Iterator<String> call(String s) 
			{
				group = 0;
				pattern = Pattern.compile(policy.m_regular);
				List<String> ar = new ArrayList<String>();
				Matcher matcher = pattern.matcher(s);
				while (matcher.find())
				ar.add(matcher.group(group));
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