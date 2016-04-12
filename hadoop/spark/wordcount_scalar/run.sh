#!/bin/bash

set -x

cd $SPARK_HOME/bin

spark-submit \
--name WordCount \
--class com.luogankun.spark.base.WordCount \
--master spark://hadoop000:7077 \
--executor-memory 1G \
--total-executor-cores 1 \
/home/spark/lib/spark.jar \
hdfs://hadoop000:8020/hello.txt
