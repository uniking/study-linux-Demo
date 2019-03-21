import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.Text;

public class myObject implements Serializable, Writable{
	String name;
	String age;
	private ArrayWritable m_three = new ArrayWritable(IntWritable.class);
	ArrayWritable m_course = new ArrayWritable(Text.class);
	
	myObject()
	{
		name = "wa";
		age = "25";
		
		IntWritable[] aw= new IntWritable[1];
		aw[0] = new IntWritable(2);
		m_three.set(aw);
		
		Text[] tt= new Text[1];
		tt[0] = new Text("yuwen");
		m_course.set(tt);
		
	}
	void setName(String n)
	{
		System.out.println("mo:setname:"+n);
		name = n;
	}
	
	String getName()
	{
		System.out.println("mo:getname:"+name);
		return name;
	}
	
	void setThree()
	{
		Writable[] ab = new Writable[3];
		ab[0] = new IntWritable(2);
		ab[1] = new IntWritable(3);
		ab[2] = new IntWritable(4);
		
		this.m_three.set(ab);
	}
	
	ArrayWritable getThree()
	{
		return m_three;
	}
	
	public List<String> getCourse()
	{
		Text[] ttx = (Text[])m_course.toArray();
		
		List<String> rt = new ArrayList<String>();
		int i=0;
		while(i < ttx.length)
		{
			rt.add(new String(ttx[i].getBytes()));
			++i;
		}
		
		return rt;
	}
	
	public void setCourse()
	{
		Text[] tt = new Text[3];
		tt[0] = new Text("shuxue");
		tt[1] = new Text("yuwen");
		tt[2] = new Text("yingyu");
		m_course.set(tt);
	}
	

	@Override
	public void write(DataOutput out) throws IOException {
		// TODO Auto-generated method stub
		System.out.println("mo:write:"+name);
		out.writeUTF(name);
		out.writeUTF(age);
		this.m_three.write(out);
		m_course.write(out);
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		// TODO Auto-generated method stub
		name=in.readUTF();
		age=in.readUTF();
		m_three.readFields(in);
		m_course.readFields(in);
		System.out.println("mo:read:"+name);
	}
}
