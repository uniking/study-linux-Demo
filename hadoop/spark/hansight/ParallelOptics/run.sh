#!/bin/bash
/home/sxis/Eagle/spark-1.3.1-bin-hadoop2.6/bin/spark-submit \
--name gmm \
--class com.hansight.algorithms.mllib.ParallelOpticsTest \
--master spark://127.0.0.1:7077 \
--executor-memory 1G \
--total-executor-cores 1 \
hansight-algorithms-1.0-SNAPSHOT.jar \
hdfs://127.0.0.1:8020/test_optics.txt \
3 \
1
