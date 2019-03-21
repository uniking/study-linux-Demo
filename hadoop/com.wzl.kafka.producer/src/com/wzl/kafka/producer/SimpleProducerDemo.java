package com.wzl.kafka.producer;


import java.util.Properties;
import java.util.Random;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerRecord;

public class SimpleProducerDemo {
	
	public static String getRandomString(int length) { //length��ʾ�����ַ����ĳ���  
	    //String base = "abcdefghijklmnopqrstuvwxyz0123456789";
		String base = "abcdefghijk";
	    Random random = new Random();     
	    StringBuffer sb = new StringBuffer();     
	    for (int i = 0; i < length; i++) {     
	        int number = random.nextInt(base.length());     
	        sb.append(base.charAt(number));     
	    }
	    
	    return sb.toString();     
	 }

    public static void main(String[] args) throws Exception {
		Properties props = new Properties();
		 props.put("bootstrap.servers", "node1:9092");
		 props.put("acks", "all");
		 props.put("retries", 0);
		 props.put("batch.size", 16384);
		 props.put("linger.ms", 1);
		 props.put("buffer.memory", 33554432);
		 props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
		 props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");
		
		 Producer<String, String> producer = new KafkaProducer<>(props);
		 for(int i = 0; i < 100; i++)
		 {
		     producer.send(new ProducerRecord<String, String>("wzl", Integer.toString(i), getRandomString(6)));
		     Thread.sleep(50);
		 }
		
		 producer.close();
		 
		 System.out.println("exit producer");
    }
}