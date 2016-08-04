#!/bin/bash
/home/sxis/Eagle/spark-1.3.1-bin-hadoop2.6/bin/spark-submit \
--name extract_sequence \
--class org.wzl.spark.base.extract_sequence \
--master spark://127.0.0.1:7077 \
--executor-memory 512m \
--total-executor-cores 1 \
extract_sequence-1.0-SNAPSHOT.jar \
hdfs://127.0.0.1:8020/test.txt
