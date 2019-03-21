import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

public class ttime {
	
	public static void main(String[] args) throws ParseException
	{
		long time = 1432710115000L;
		Date date = new Date(time);
		
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		
		String d = df.format(date);
		System.out.println(d);
		
		
		Date nd = df.parse("1990-02-22 12:00:52");
		long nt = nd.getTime();
		System.out.println(nt);
		
		Date nowd = new Date();
		long nowt = nowd.getTime();
		System.out.println(df.format(nowd));
		
		
		//UTC时间
		SimpleDateFormat udf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
		udf.setTimeZone(TimeZone.getTimeZone("UTC"));
		System.out.println(udf.parse("2014-08-23T09:20:05Z").toString());
		
	}

}
