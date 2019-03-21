package com.suninfo.cloud.ndlp.discover.client;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.List;

import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.Text;

public class DisListString implements Writable{
	ArrayWritable logList = new ArrayWritable(Text.class);
	
	@Override
	public void write(DataOutput out) throws IOException {
		// TODO Auto-generated method stub
		logList.write(out);
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		// TODO Auto-generated method stub
		logList.readFields(in);
	}
	
	public DisListString()
	{
		Text[] nll = new Text[1];
		nll[0] = new Text("null");
		logList.set(nll);
	}
	
	public void insertString(String str)
	{
		Text[] ll = (Text[])logList.toArray();
		Text[] nll;

		nll = new Text[ll.length + 1];
		for(int i=0; i< ll.length; ++i)
			nll[i] = ll[i];
		nll[ll.length] = new Text(str);
		
		logList.set(nll);
	}
	
	public int getSize()
	{
		return logList.get().length -1;
	}
	
	public boolean getString(List<String> str)
	{
		int i=0;
		Text[] ll = (Text[])logList.toArray();
		if(ll.length <= 1)
			return false;
		
		for(i=1;i<ll.length;++i)
		{
			str.add(new String(ll[i].getBytes()));
		}
		
		return true;
	}
}