package use_third_package;

import third_pakage.MyAdd;

public class UseThird {
	
	public static void main(String[] args)
	{
		MyAdd ma = new MyAdd();
		System.out.println( ma.MyAdd(10, 20));
	}

}
