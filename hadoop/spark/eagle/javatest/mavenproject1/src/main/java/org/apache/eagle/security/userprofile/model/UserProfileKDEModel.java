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

import org.apache.commons.math3.linear.RealVector;

import java.io.Serializable;


public class UserProfileKDEModel implements Serializable
{
	private RealVector[] ppc;
	
	private final Long m_version;
	private final String m_site;
	private final String m_user;
        private final UserCommandStatistics[] m_listStats;
        private final Double m_minProbabilityEstimate;
        private final Double m_maxProbabilityEstimate;
        private final Double m_nintyFivePercentileEstimate;
        private final Double m_medianProbabilityEstimate;

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

	public void print()
	{
		System.out.println("KDEModel: version=" + m_version + " site:" + m_site + " user:" + 
									m_user + " minProbabilityEstimate:"+m_minProbabilityEstimate +" maxProbabilityEstimate:"+m_maxProbabilityEstimate 
					+ " nintyFivePercentileEstimate:"+m_nintyFivePercentileEstimate + " medianProbabilityEstimate:"+m_medianProbabilityEstimate);
		for(UserCommandStatistics x:m_listStats)
		{
			x.print();
		}
	}
}
