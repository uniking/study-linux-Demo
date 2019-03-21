package com.wzl.kafka.producer;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;

import java.util.Collections;
import java.util.Properties;
import kafka.utils.ShutdownableThread;

public class SimpleConsumerDemo extends ShutdownableThread{
    private final KafkaConsumer<String, String> consumer;
    private final String topic;

    public SimpleConsumerDemo(String topic)
    {
        super("SimpleConsumerDemo", false);
        Properties props = new Properties();
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "node1:9092");
        props.put(ConsumerConfig.GROUP_ID_CONFIG, "discover1");
        props.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "true");
        //props.put(ConsumerConfig.AUTO_COMMIT_INTERVAL_MS_CONFIG, "1000");
        //props.put(ConsumerConfig.SESSION_TIMEOUT_MS_CONFIG, "30000");
        //props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.IntegerDeserializer");
        props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
        props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");

        consumer = new KafkaConsumer<String, String>(props);
        this.topic = topic;
    }

    @Override
    public void doWork()
    {
    	System.out.println("doWork");
        consumer.subscribe(Collections.singletonList(this.topic));
        ConsumerRecords<String, String> records = consumer.poll(10);
        if(records.count() == 0)
        	System.out.println("count == 0");
        for (ConsumerRecord<String, String> record : records)
        {
        	System.out.println("have one");
            System.out.println("Received message: (" + record.key() + ", " + record.value() + ") at offset " + record.offset());
        }
    }

    @Override
    public String name() {
        return null;
    }

    @Override
    public boolean isInterruptible() {
        //return false;
    	return true;
    }
    

}
