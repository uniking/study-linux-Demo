/*
add by wzl
 */

package com.hansight.algorithms.mllib

import org.apache.spark.{SparkConf, SparkContext}
import org.apache.spark.mllib.linalg.Vectors
import com.hansight.algorithms.mllib.ParallelOptics
import scala.collection.mutable.ListBuffer
import scala.collection.mutable.ArrayBuffer

/**
 * An example Gaussian Mixture Model EM app. Run with
 * {{{
 * ./bin/run-example mllib.DenseGaussianMixture <input> <k> <convergenceTol>
 * }}}
 * If you use it as a template to create your own app, please use `spark-submit` to submit your app.
 */
object ParallelOpticsTest {

	class P[T](flag_ : Int, data_ : T) {
		val flag     = flag_
		val data     = data_
	}

	def main(args: Array[String]): Unit =
	{
		if (args.length < 2)
		{
			println("usage: DenseGmmEM <input file> <r>")
		}
		else
		{
			val maxIterations = if (args.length > 3) args(3).toInt else 100
			run(args(0), args(1).toInt, args(2).toDouble)
		}
	}





	private def run(inputFile: String, minPts: Int, radius:Double)
	{
		val conf = new SparkConf().setAppName("Gaussian Mixture Model EM example")
		val ctx  = new SparkContext(conf)
		ctx.setCheckpointDir("checkpoint")

		val data = ctx.textFile(inputFile)
		val line = data.collect

		val mat:ArrayBuffer[Array[Double]] = new ArrayBuffer()
		for(elem <- line)
			mat += elem.split(" ").map(_.toDouble)
		test(ctx, mat.toArray, minPts, radius)
	}



	private def test(sc:SparkContext, data:Array[Array[Double]], minPts: Int, radius:Double)
	{

		for(elem <- data)
			elem.foreach(println _)


		val data_ = sc.parallelize(data)

		// noise must be 0, cluster must be 1
		// lb = [[cluster_1]]
		//var opt = new ParallelOptics(1, Double.MaxValue, "EuclideanDistance")
		var opt = new ParallelOptics(minPts, radius, "EuclideanDistance")
		var out = opt.run(data_).collect()
		for (elem <- out)
		{
			println("id:"+elem.id + " cd:"+elem.coreDis + " rd:"+elem.reachDis + " notCore:"+elem.notCore)
		}

		var clu = getCluster(out, data, 0)
		//assert(clu._1           == 1)
		//assert(clu._2           == 0)
		//assert(clu._3.length    == 1)
		//assert(clu._3(0).length == 3)
		//assert(clu._3(0).forall(x => x.flag == 1))

		// noise must be 3, cluster must be 0
		// lb = [[noise]]
		//opt = new ParallelOptics(1, -2, "EuclideanDistance")
		//out = opt.run(data_).collect()
		//out.foreach(println (_.id, _.reachDis, _.coreDis) )

		//clu = getCluster(out, data, 0)
		//assert(clu._1           == 0)
		//assert(clu._2           == 3)
		//assert(clu._3.length    == 1)
		//assert(clu._3(0).length == 3)
		//assert(clu._3(0).forall(x => x.flag == -1))
		//opt = new ParallelOptics(Int.MaxValue, Double.MaxValue, "EuclideanDistance")
		//out = opt.run(data_).collect()
		//clu = getCluster(out, data, 99999.0)
		//assert(clu._1           == 0)
		//assert(clu._2           == 3)
		//assert(clu._3.length    == 1)
		//assert(clu._3(0).length == 3)
		//assert(clu._3(0).forall(x => x.flag == -1))

	}


	def getCluster[T]( optResult  : Array[_<:ParallelOptics#Point],
							data       : Array[T],
							r          : Double ) : (Int, Int, ListBuffer[ListBuffer[P[T]]]) = 
	{
		var flag  = 0
		var noise = 0
		val lb    = ListBuffer[ListBuffer[P[T]]]()
		val lbn   = ListBuffer[P[T]]()
		val lb_   = ListBuffer[P[T]]()
		for ( i <- optResult ) 
		{
			if (i.reachDis > r) 
			{
				if (i.coreDis > r) 
				{
					noise += 1
					lbn   += new P(-1, data(i.id.toInt))
				}
				else 
				{
					flag += 1
					if (lb_.length > 0)
						lb += lb_.clone()
					lb_ --= lb_
					lb_  += new P(flag, data(i.id.toInt))
				}
			}
			else
				lb_  += new P(flag, data(i.id.toInt))
		}
		if (lb_.length > 0)
			lb += lb_.clone()
		if (lbn.length > 0)
			lb += lbn
		(flag, noise, lb)
	}

}
