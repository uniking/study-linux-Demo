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
	line.collect.foreach(println)

    //line.flatMap(_.split("\t")).map((_, 1)).reduceByKey(_+_).collect.foreach(println)
	val nline = line.flatMap(_.split(" "));
	println("-----nline-----")
	nline.collect.foreach(println)
	
	val words = nline.map((_,1))
	println("----------key value--------")
	words.collect.foreach(println)
    
	val cword = words.reduceByKey(_+_)
	println("--------word count--------")
	cword.collect.foreach(println)
    

	sc.stop
  }
}
