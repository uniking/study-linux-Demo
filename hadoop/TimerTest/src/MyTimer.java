import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.lang.time.StopWatch;

public class MyTimer {
	static boolean bRun = true;
	static void test() throws InterruptedException
	{
		Timer t = new Timer(true);
		t.schedule(new TimerTask(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				bRun = false;
			}}, 9000);
		
		while(bRun)
		{
			Thread.sleep(1000);
			System.out.println("hello");
		}
	}
	
	
	public static void main(String[] args) throws InterruptedException
	{
		//test();
		
		StopWatch sw = new StopWatch();
		sw.start();
		while(true)
		{
			Thread.sleep(1000);
			System.out.println( sw.getTime() );
		}
		//System.out.println("exit");
	}

}
