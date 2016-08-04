package org.wzl.spark.base

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._

/**
 * A B C D E F G H
=> AB BC CD EF FG GH
=> ABC BCD CDE DEF EFG FGH
extract syscall call string to feature sequence
 */
object extract_sequence
{

	def k(a:String):List[(Int,String)]=
	{
		val word=a.split(" ")
		var i=0
		var ret = List[(Int,String)]()
		for(e<-word)
		{
			i=i+1;
			ret = ret ++ List((i,e))
			println(i,e)
		}
		println(ret.size)
		ret
	}

	def g(c:Int, x:(Int,String)):Boolean =
	{
		if(x._1 == c)
			return true
		if(x._1 == c+1)
			return true
		return false
	}

	def main(args: Array[String]) {
	if (args.length < 1) {
	System.err.println("Usage: <file>")
	System.exit(1)
	}

	val conf = new SparkConf()
	val sc = new SparkContext(conf)
	val line = sc.textFile(args(0))

	val wcRDD = line.flatMap(k(_)) //(1, A)
	val i=wcRDD.count
	var j=0;
	while(j<i-1)
	{
		j = j+1
		val t = wcRDD.filter(g(j, _) )
		val seq = t.reduce((a,b)=>(a._1,a._2+b._2))._2
		println(seq) //insert hive
	}

	sc.stop
	}
}


