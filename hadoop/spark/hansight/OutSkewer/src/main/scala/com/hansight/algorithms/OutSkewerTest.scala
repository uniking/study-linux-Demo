/*
add by wzl
 */

package com.hansight.algorithms

import org.apache.spark.{SparkConf, SparkContext}
import org.apache.spark.mllib.linalg.Vectors
import scala.collection.mutable.ListBuffer
import com.hansight.algorithms.OutSkewer

/**
 * An example Gaussian Mixture Model EM app. Run with
 * {{{
 * ./bin/run-example mllib.DenseGaussianMixture <input> <k> <convergenceTol>
 * }}}
 * If you use it as a template to create your own app, please use `spark-submit` to submit your app.
 */
object OutSkewerTest {
  def main(args: Array[String]): Unit = {
    if (args.length < 3) {
      println("usage: DenseGmmEM <input file> <k> <convergenceTol> [maxIterations]")
    } else {
      val maxIterations = if (args.length > 3) args(3).toInt else 100
      run(args(0), args(1).toInt, args(2).toDouble, maxIterations)
    }
  }

  private def run(inputFile: String, k: Int, convergenceTol: Double, maxIterations: Int) {
    val conf = new SparkConf().setAppName("Gaussian Mixture Model EM example")
    val ctx  = new SparkContext(conf)
    
    val data = ctx.textFile(inputFile).map { line =>
      Vectors.dense(line.trim.split(' ').map(_.toDouble))
    }

	val data_l = data.collect.toList.to[ListBuffer]
    val os = new OutSkewer
    os detectOutliers data_l  // ListBuffer(0.73, 0.73, 0.73) yi wei, wo men de shi er wei.
    assert(os.getNo(0) == 1 & os.getNo(1) == 0 & os.getNo(2) == 1)
    assert(os.getMaybe(0) == 0 & os.getMaybe(1) == 1 & os.getMaybe(2) == 0)
    assert(os.getYes.forall(i => i == 0))
    os clear()
  }
}
