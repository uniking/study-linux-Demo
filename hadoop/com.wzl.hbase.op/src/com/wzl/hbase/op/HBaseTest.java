package com.wzl.hbase.op;


import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HConstants;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.Admin;
import org.apache.hadoop.hbase.client.Connection;
import org.apache.hadoop.hbase.client.ConnectionFactory;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Table;
import org.apache.hadoop.hbase.io.compress.Compression.Algorithm;
import org.apache.hadoop.hbase.util.Bytes;

public class HBaseTest {

	private static final String TABLE_NAME = "MY_TABLE_NAME_TOO";
	private static final String CF_DEFAULT = "DEFAULT_COLUMN_FAMILY";


	/*
	* 为表添加数据（适合知道有多少列族的固定表）
	* 
	* @rowKey rowKey
	* 
	* @tableName 表名
	* 
	* @column1 第一个列族列表
	* 
	* @value1 第一个列的值的列表
	* 
	* @column2 第二个列族列表
	* 
	* @value2 第二个列的值的列表
	*/
	public static void addData(Configuration config, String rowKey, String tableName, String[] column1, String[] value1)
	throws IOException 
	{
		try (Connection connection = ConnectionFactory.createConnection(config);
		Admin admin = connection.getAdmin()) 
		{
			HTableDescriptor table = new HTableDescriptor(TableName.valueOf(tableName));

			Table tb = connection.getTable(table.getTableName());

			Put put = new Put(Bytes.toBytes(rowKey));// 设置rowkey

			//HTable table = new HTable(conf, Bytes.toBytes(tableName));// HTabel负责跟记录相关的操作如增删改查等//
					  // 获取表
			HColumnDescriptor[] columnFamilies = tb.getTableDescriptor() // 获取所有的列族
			.getColumnFamilies();

			for (int i = 0; i < columnFamilies.length; i++)
			{
				String familyName = columnFamilies[i].getNameAsString(); // 获取列族名
				if (familyName.equals(CF_DEFAULT))
				{ // article列族put数据
					for (int j = 0; j < column1.length; j++)
					{
						put.addColumn(Bytes.toBytes(familyName),
						Bytes.toBytes(column1[j]), Bytes.toBytes(value1[j]));
					}
				}
			}
			//table.put(put);
			tb.put(put);
			System.out.println("add data Success!");
		}
	}

	public static void createOrOverwrite(Admin admin, HTableDescriptor table) throws IOException
	{
		if (admin.tableExists(table.getTableName()))
		{
			System.out.println("disable and delete table");
			admin.disableTable(table.getTableName());
			admin.deleteTable(table.getTableName());
		}
		else
		{
			System.out.println("table is't exists");
		}
		admin.createTable(table);
	}

	public static void createSchemaTables(Configuration config) throws IOException
	{
		try (Connection connection = ConnectionFactory.createConnection(config);
		Admin admin = connection.getAdmin()) 
		{
			HTableDescriptor table = new HTableDescriptor(TableName.valueOf(TABLE_NAME));

			//table.addFamily(new HColumnDescriptor(CF_DEFAULT).setCompressionType(Algorithm.SNAPPY));
			table.addFamily(new HColumnDescriptor(CF_DEFAULT));

			System.out.print("Creating table. ");
			createOrOverwrite(admin, table);
			System.out.println(" Done.");
		}
	}

	public static void modifySchema (Configuration config) throws IOException
	{
		try (Connection connection = ConnectionFactory.createConnection(config);
		Admin admin = connection.getAdmin())
		{
			TableName tableName = TableName.valueOf(TABLE_NAME);
			if (!admin.tableExists(tableName))
			{
				System.out.println("Table does not exist.");
				System.exit(-1);
			}

			HTableDescriptor table = new HTableDescriptor(tableName);

			// Update existing table
			HColumnDescriptor newColumn = new HColumnDescriptor("NEWCF");
			newColumn.setCompactionCompressionType(Algorithm.GZ);
			newColumn.setMaxVersions(HConstants.ALL_VERSIONS);
			admin.addColumn(tableName, newColumn);

			// Update existing column family
			HColumnDescriptor existingColumn = new HColumnDescriptor(CF_DEFAULT);
			existingColumn.setCompactionCompressionType(Algorithm.GZ);
			existingColumn.setMaxVersions(HConstants.ALL_VERSIONS);
			table.modifyFamily(existingColumn);
			admin.modifyTable(tableName, table);

			// Disable an existing table
			admin.disableTable(tableName);

			// Delete an existing column family
			admin.deleteColumn(tableName, CF_DEFAULT.getBytes("UTF-8"));

			// Delete a table (Need to be disabled first)
			admin.deleteTable(tableName);
		}
	}

	public static void main(String... args) throws IOException
	{
		Configuration config = HBaseConfiguration.create();

		//Add any necessary configuration files (hbase-site.xml, core-site.xml)
		//config.addResource(new Path(System.getenv("HBASE_CONF_DIR"), "hbase-site.xml"));
		//config.addResource(new Path(System.getenv("HADOOP_CONF_DIR"), "core-site.xml"));
		//config.addResource(new Path("d:\\cloud\\hbase-site.xml"));
		//config.addResource(new Path("d:\\cloud\\core-site.xml"));

		config.set("hbase.zookeeper.quorum", "node1,node2,node3,node4,node5");
		config.set("hbase.rootdir", "hdfs://node1:9000/hbase");
		config.set("hbase.zookeeper.property.clientPort", "2181");
		config.set("hbase.table.sanity.checks", "false");
		//createSchemaTables(config);
		//modifySchema(config);

		String[] rol = {CF_DEFAULT};
		String[] val = {"99"};
		addData(config, "w", TABLE_NAME, rol, val);
	}
}