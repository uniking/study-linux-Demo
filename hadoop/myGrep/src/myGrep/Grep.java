package myGrep;

import java.io.IOException;

/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.*;
import org.apache.hadoop.mapreduce.Mapper.Context;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.map.InverseMapper;
import org.apache.hadoop.mapreduce.lib.map.RegexMapper;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.mapreduce.lib.reduce.LongSumReducer;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

/* Extracts matching regexs from input files and counts them. */
public class Grep extends Configured implements Tool 
{
	private static FSDataOutputStream osLog;
	private static  Configuration conf_log;
		
	private static   FileSystem hdfs_log;	 
	
	private static  Path dfs_log;
	  
	  
  private Grep() {}                               // singleton
  
  public static int init_log() throws Exception
  {
	  conf_log=new Configuration();
	
	  hdfs_log=FileSystem.get(conf_log);	 
	
	  dfs_log=new Path("/user/ubt/WordCount/log");
	
	  osLog=hdfs_log.create(dfs_log);
	  return 0;
  }
  
  public static int write_log(String log) throws Exception
  {
	  osLog.writeChars(log);
	  return 0;
  }
  
  public static class myRegMapper<K> extends Mapper<K, Text, Text, LongWritable>
  {
	  public static String PATTERN = "mapreduce.mapper.regex";
	  public static String GROUP = "mapreduce.mapper.regexmapper..group";
	  private Pattern pattern;
	  private int group;

	  public void setup(Context context) {
	    Configuration conf = context.getConfiguration();
	    pattern = Pattern.compile(conf.get(PATTERN));
	    group = conf.getInt(GROUP, 0);
	  }
	  
	  public void map(K key, Text value,
              Context context)
			throws IOException, InterruptedException 
	  {
			String text = value.toString();
			Matcher matcher = pattern.matcher(text);
			while (matcher.find()) {
				try {
					System.out.println(matcher.group(group));
					//this is reg get.
					//write_log( matcher.group(group) );
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			  context.write(new Text(matcher.group(group)), new LongWritable(1));
			}
	  }
  }

  public int run(String[] args) throws Exception 
  {
    if (args.length < 3) 
    {
      System.out.println("Grep <inDir> <outDir> <regex> [<group>]");
      ToolRunner.printGenericCommandUsage(System.out);
      return 2;
    }
    System.out.println("0:"+args[0]+" 1:"+args[1]+" 2:"+args[2]);

    Path tempDir =
      new Path("grep-temp-"+
          Integer.toString(new Random().nextInt(Integer.MAX_VALUE)));

    Configuration conf = getConf();
    conf.set(RegexMapper.PATTERN, args[2]);
    if (args.length == 4)
      conf.set(RegexMapper.GROUP, args[3]);

    Job grepJob = Job.getInstance(conf);
    
    try 
    {
    	init_log();
      
      grepJob.setJobName("wzl-grep-search");
      grepJob.setJarByClass(Grep.class);

      FileInputFormat.setInputPaths(grepJob, args[0]);

      //grepJob.setMapperClass(RegexMapper.class);
      grepJob.setMapperClass(myRegMapper.class);
      
      //(string, int) -> sub int -> (string , sum)
      grepJob.setCombinerClass(LongSumReducer.class);
      grepJob.setReducerClass(LongSumReducer.class);

      //FileOutputFormat.setOutputPath(grepJob, tempDir);
      FileOutputFormat.setOutputPath(grepJob, new Path(args[1]));
      grepJob.setOutputFormatClass(SequenceFileOutputFormat.class);
      grepJob.setOutputKeyClass(Text.class);
      grepJob.setOutputValueClass(LongWritable.class);

      grepJob.waitForCompletion(true);
      
      /*

      Job sortJob = Job.getInstance(conf);
      sortJob.setJobName("wzl-grep-sort");
      sortJob.setJarByClass(Grep.class);

      FileInputFormat.setInputPaths(sortJob, tempDir);
      sortJob.setInputFormatClass(SequenceFileInputFormat.class);

      sortJob.setMapperClass(InverseMapper.class);

      sortJob.setNumReduceTasks(1);                 // write a single file
      FileOutputFormat.setOutputPath(sortJob, new Path(args[1]));
      sortJob.setSortComparatorClass(          // sort by decreasing freq
        LongWritable.DecreasingComparator.class);

      sortJob.waitForCompletion(true);
      */
    }
    finally 
    {
      FileSystem.get(conf).delete(tempDir, true);
    }
    return 0;
  }

  public static void main(String[] args) throws Exception 
  {
    int res = ToolRunner.run(new Configuration(), new Grep(), args);
    System.exit(res);
  }

}
