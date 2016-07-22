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
import org.apache.commons.math3.linear.RealVector;
import org.apache.commons.math3.stat.descriptive.moment.Mean;
import org.apache.commons.math3.stat.descriptive.moment.StandardDeviation;

import org.apache.eagle.security.userprofile.model.UserCommandStatistics;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.io.Serializable;

import scala.collection.JavaConversions;

public class UserProfileKDEModel implements Serializable
{
	private RealVector[] ppc;
	
	private Long m_version;
	private String m_site;
	private String m_user;
   private UserCommandStatistics[] m_listStats;
   private Double m_minProbabilityEstimate;
   private Double m_maxProbabilityEstimate;
   private Double m_nintyFivePercentileEstimate;
   private Double m_medianProbabilityEstimate;

	public UserProfileKDEModel (
	   Long version,
		String site,
		String user,
	   UserCommandStatistics[] statistics,
	   Double minProbabilityEstimate,
	   Double maxProbabilityEstimate,
	   Double nintyFivePercentileEstimate,
	   Double medianProbabilityEstimate
	)
	{
		m_version = version;
		m_site = site;
		m_user = user;
		m_listStats = statistics;
		m_minProbabilityEstimate = minProbabilityEstimate;
		m_maxProbabilityEstimate = maxProbabilityEstimate;
		m_nintyFivePercentileEstimate = nintyFivePercentileEstimate;
		m_medianProbabilityEstimate = medianProbabilityEstimate;
	}

	 public UserCommandStatistics[] statistics()
	{
		return m_listStats;
	}

	public double maxProbabilityEstimate()
	{
		return m_maxProbabilityEstimate;
	}

	public RealVector[] principalComponents()
	{
		return ppc;
	}
}
