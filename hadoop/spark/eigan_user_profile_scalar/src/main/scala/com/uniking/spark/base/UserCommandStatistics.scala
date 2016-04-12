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
package com.uniking.spark.base;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

class UserCommandStatistics implements Serializable {
    private val serialVersionUID = -7145615521424420913L;
    private val commandName:String;
    private val mean:String;
    private val stddev:double;
    private val isLowVariant:boolean;

    def UserCommandStatistics(){}

    def UserCommandStatistics(Map[String, Object] rawMap)
    {
        if(rawMap.get(COMMAND_NAME)!=null) this.commandName = (String) rawMap.get(COMMAND_NAME);
        if(rawMap.get(MEAN)!=null)this.mean = (double) rawMap.get(MEAN);
        if(rawMap.get(STDDEV)!=null) this.stddev = (double) rawMap.get(STDDEV);
        if(rawMap.get(IS_LOW_VARIANT)!=null) this.isLowVariant = (boolean) rawMap.get(IS_LOW_VARIANT);
    }

    def getCommandName() {
        return commandName;
    }
    def setCommandName(commandName:String) {
        this.commandName = commandName;
    }

    def getMean() {
        return mean;
    }

    def setMean(mean:double) {
        this.mean = mean;
    }

    def getStddev() {
        return stddev;
    }

    def setStddev(stddev:double) {
        this.stddev = stddev;
    }

    def isLowVariant() {
        return isLowVariant;
    }

    def setLowVariant(isLowVariant:boolean) {
        this.isLowVariant = isLowVariant;
    }

    var COMMAND_NAME = "commandName";
    var MEAN = "mean";
    var STDDEV = "stddev";
    var IS_LOW_VARIANT = "isLowVariant";

    def Map[String,Object] toMap(){
        Map[String,Object] map = new HashMap<>();
        map.put(COMMAND_NAME,this.commandName);
        map.put(MEAN,this.mean);
        map.put(STDDEV,this.stddev);
        map.put(IS_LOW_VARIANT,this.isLowVariant);
        return map;
    }
}
