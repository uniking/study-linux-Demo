#!/bin/bash
/home/uniking/Eagle/spark-1.3.1-bin-hadoop2.6/bin/spark-submit \
--name WordCount \
--class com.luogankun.spark.base.WordCount \
--master spark://127.0.0.1:7077 \
--executor-memory 512m \
--total-executor-cores 1 \
wordcount-scalar-1.0-SNAPSHOT.jar \
hdfs://127.0.0.1:8020/ip.txt
