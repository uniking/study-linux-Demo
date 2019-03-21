package com.wzl.kafka.producer;

import java.util.Arrays;
import java.util.Collections;
import java.util.Properties;
import java.util.Set;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.common.TopicPartition;

public class TestConsumer {
    
    public void test()
    {
        Properties props = new Properties();
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "node1:9092");
        props.put(ConsumerConfig.GROUP_ID_CONFIG, "discover1");
        props.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "true");
        props.put(ConsumerConfig.AUTO_COMMIT_INTERVAL_MS_CONFIG, "1000");
        props.put(ConsumerConfig.SESSION_TIMEOUT_MS_CONFIG, "30000");
        props.put("linger.ms", 10);
        //props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.IntegerDeserializer");
        props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
        props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");

        KafkaConsumer<String, String> consumer = new KafkaConsumer<>(props);
        consumer.subscribe(Arrays.asList("topic_plain_text"));
        
        while(true)
        {
	        ConsumerRecords<String, String> records = consumer.poll(10000);
	        System.out.println("records count:" + records.count());
	        for (ConsumerRecord<String, String> record : records)
	        {
	            System.out.println("Received message: (" + record.key() + ", " + record.value() + ") at offset " + record.offset());
	        }
        }
    }
    
    public static void main(String[] args) throws InterruptedException
    {
    	new TestConsumer().test();
    	System.out.println("main exit");
    }
}
