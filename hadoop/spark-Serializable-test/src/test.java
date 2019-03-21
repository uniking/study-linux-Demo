
public class test {
	public static void main(String[] args) throws Exception
	{
		sparkC sc = new sparkC();
		sc.init();
		
		String log = sc.wc("hdfs://node1:9000/user/ubt/WordCount/novel/novel.txt");
		System.out.println(log);
		
		System.out.println("sleep ....");
		Thread.sleep(3000);
		String log2 = sc.wc("hdfs://node1:9000/user/ubt/WordCount/novel/novel.txt");
		System.out.println(log2);
		
		sc.uninit();
	}

}
