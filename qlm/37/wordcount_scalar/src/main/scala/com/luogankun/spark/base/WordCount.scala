package com.luogankun.spark.base

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._

/**
 * 统计字符出现次数
 */
object WordCount {
  def main(args: Array[String]) {
    if (args.length < 1) {
      System.err.println("Usage: <file>")
      System.exit(1)
    }

    val conf = new SparkConf()
    val sc = new SparkContext(conf)
    val line = sc.textFile(args(0))

    val wcRDD = line.flatMap(_.split("\t")).map((_, 1)).reduceByKey(_+_)
    //wcRDD.filter(_._2 > 5).map(t=>(t._2, t._1)).sortByKey().saveAsTextFile("hdfs://127.0.0.1:8020/tmp/hello")
    wcRDD.map(t=>(t._2, t._1)).sortByKey().saveAsTextFile("hdfs://127.0.0.1:8020/tmp/hello")
    
    sc.stop
  }
}
