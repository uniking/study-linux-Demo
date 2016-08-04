/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.wzl.spark.base

import org.apache.spark.{SparkConf, SparkContext}
import org.apache.spark.sql._
import org.apache.spark.sql.hive.HiveContext

object HiveFromSpark {

	def main(args: Array[String])
	{
		val sparkConf = new SparkConf().setAppName("HiveFromSpark")
		val sc = new SparkContext(sparkConf)

		val hiveContext = new HiveContext(sc)
		import hiveContext.implicits._
		import hiveContext.sql

		sql("create table qwer(a int ,b int) row format delimited fields terminated by ','")
		sql("load data inpath \"/test.txt\" into table qwer")
		sql("select * from qwer").collect().foreach(println)

		sql("insert into table qwer select id1,id2 from mytable")

		sc.stop()
	}
}
