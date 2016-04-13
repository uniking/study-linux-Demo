package com.uniking.spark.base

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._

/**
 * 统计字符出现次数
 */
object RDDtransform{
  def main(args: Array[String]) {
    if (args.length < 1) {
      System.err.println("Usage: <file>")
      System.exit(1)
    }

    val conf = new SparkConf()
    val sc = new SparkContext(conf)
    val line = sc.textFile(args(0))

	println("--------------line--")
    println( "--------------line rdd count=" + line.count ) //是行的数目，也就是说，sc.textFile后得到的是一行一行的rdd
	line.collect.foreach(println)

    //line.flatMap(_.split("\t")).map((_, 1)).reduceByKey(_+_).collect.foreach(println)
	val nline = line.flatMap(_.split(" ")) //对每行做空格切分
	println("-----nline-----")
	nline.collect.foreach(println)
	
	val fword = nline.filter( _ != "zhong") //返回true的保留了，返回false被过滤了，感觉理解反了。
	println("-------fword------")
	fword.collect.foreach(println)
	
	val words = fword.map((_,1))
	println("----------key value--------")
	words.collect.foreach(println)
    
	val cword = words.reduceByKey(_+_)
	println("--------word count--------")
	cword.collect.foreach(println)
	
	/*
	测试2
	*/
	val words2 = Array("one", "two", "two", "three","three", "three");
	val wordPairsRDD = sc.parallelize(words2).map(word => (word, 1))
	val wordGroup = wordPairsRDD.groupByKey()
	println("------------------word group count=" + wordGroup.count)
	val mapGroup = wordGroup.map(t => (t._1, t._2.sum))
	mapGroup.collect.foreach(println)
	
	/*
	测试3
	*/
	val num3 = List(1, 2, 6,56,12,3,6,11,34,67,33,56,44,23,11,9)
	val num3RDD = sc.parallelize(num3)
	val s2l = num3RDD.sortBy(x=>x)
	s2l.collect.foreach(println)
	
	sc.stop
  }
}
