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

public class UserProfileEigenModel implements Serializable
{
	private UserCommandStatistics[] listStats;
	private double maxPro;
	private RealVector[] ppc;


   private Long m_version;
	private String m_site;
	private String m_user;
	private RealMatrix m_uMatrix;
	private RealMatrix m_diagonalMatrix;
	private int m_dimension;
	private RealVector m_minVector;
	private RealVector m_maxVector;
	private RealVector[] m_principalComponents;
	private RealVector m_maximumL2Norm;
	private RealVector m_minimumL2Norm;
   private UserCommandStatistics[] m_statistics;


	public UserProfileEigenModel (
	   Long version,
		String site,
		String user,
		RealMatrix uMatrix,
		RealMatrix diagonalMatrix,
		int dimension,
		RealVector minVector,
		RealVector maxVector,
		RealVector[] principalComponents,
		RealVector maximumL2Norm,
		RealVector minimumL2Norm,
	   UserCommandStatistics[] statistics
	)
	{
	   m_version = version;
		m_site = site;
		m_user = user;
		m_uMatrix = uMatrix;
		m_diagonalMatrix = diagonalMatrix;
		m_dimension = dimension;
		m_minVector = minVector;
		m_maxVector = maxVector;
		m_principalComponents = principalComponents;
		m_maximumL2Norm = maximumL2Norm;
		m_minimumL2Norm = minimumL2Norm;
	   m_statistics = statistics;
	}

	 public UserCommandStatistics[] statistics()
	{
		return m_statistics;
	}

	public RealVector maxProbabilityEstimate()
	{
		return m_maxVector;
	}

	public RealVector[] principalComponents()
	{
		return m_principalComponents;
	}

	public RealVector maximumL2Norm()
	{
		return m_maximumL2Norm;
	}
}
