import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.io.Serializable;

import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;

public class ContainMyObj implements Serializable, Writable{
	
	ArrayWritable m_course = new ArrayWritable(myObject.class);

	@Override
	public void write(DataOutput out) throws IOException {
		// TODO Auto-generated method stub
		m_course.write(out);
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		// TODO Auto-generated method stub
		m_course.readFields(in);
	}
	
	public void init_cmo(myObject mo)
	{
		myObject[] arr= new myObject[1];
		arr[0] = mo;
		m_course.set(arr);
	}
	
	public myObject get_mo(int i)
	{
		myObject[] arr = (myObject[])m_course.toArray();
		return arr[i];
	}

}
