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
package org.apache.eagle.security.userprofile.model;

import org.apache.commons.math3.linear.RealMatrix;
import org.apache.commons.math3.linear.Array2DRowRealMatrix; 

import scala.collection.JavaConversions;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @since  7/31/15
 */
/*
case class UserActivityAggModel(user:String,matrix:RealMatrix,cmdTypes:Seq[String],site:String,timestamp:Long)
  extends EntityConversion[UserActivityAggModelEntity]{
  override def toEntity: UserActivityAggModelEntity = UserActivityAggModelEntity.fromModel(this)
  override def fromEntity(entity: UserActivityAggModelEntity): Unit = UserActivityAggModelEntity.toModel(entity)
}
*/

public class UserActivityAggModel
{

	private List<String> cmdTypes;

	public UserActivityAggModel(String user,RealMatrix matrix,String[] cmdTypes,String site,Long timestamp)
	  //extends EntityConversion[UserActivityAggModelEntity]
	{
	  //override def toEntity: UserActivityAggModelEntity = UserActivityAggModelEntity.fromModel(this)
	  //override def fromEntity(entity: UserActivityAggModelEntity): Unit = UserActivityAggModelEntity.toModel(entity)
	}

	public RealMatrix matrix()
	{
		double[][] coefficientsData = { { 3, 1 }, { 1, 2 } };

		RealMatrix coefficients = new Array2DRowRealMatrix(coefficientsData);
		return coefficients;
	}

	public List<String> getCmdTypes() {
		return cmdTypes;
	}


/*
	public Seq<String> CmdTypes() {
		Seq<String> a = null;
		return a;
	}
*/

	public long timestamp()
	{
		long timestamp = 0;
		return timestamp;
	}
}
