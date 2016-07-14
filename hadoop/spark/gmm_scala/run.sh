#!/bin/bash
/home/sxis/Eagle/spark-1.3.1-bin-hadoop2.6/bin/spark-submit \
--name gmm \
--class com.wzl.spark.base.DenseGaussianMixture \
--master spark://127.0.0.1:7077 \
--executor-memory 1G \
--total-executor-cores 1 \
gmm-scala-1.0-SNAPSHOT.jar \
hdfs://127.0.0.1:8020/ip.txt \
6 \
0.0001
