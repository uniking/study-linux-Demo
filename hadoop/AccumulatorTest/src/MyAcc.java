import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.io.Serializable;

import org.apache.spark.util.AccumulatorV2;

@SuppressWarnings("serial")
public class MyAcc extends AccumulatorV2{
	int count;

	@Override
	public void add(Object arg0) {
		// TODO Auto-generated method stub
		count = count + ((Integer)arg0).intValue();
	}

	@Override
	public AccumulatorV2 copy() {
		// TODO Auto-generated method stub
		MyAcc myAcc = new MyAcc();
		myAcc.count = count;
		return myAcc;
	}

	@Override
	public boolean isZero() {
		// TODO Auto-generated method stub
		if(count == 0)
			return true;
		return false;
	}

	@Override
	public void merge(AccumulatorV2 arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void reset() {
		// TODO Auto-generated method stub
		count=0;
	}

	@Override
	public Object value() {
		// TODO Auto-generated method stub
		return new Integer(count);
	}
	
}