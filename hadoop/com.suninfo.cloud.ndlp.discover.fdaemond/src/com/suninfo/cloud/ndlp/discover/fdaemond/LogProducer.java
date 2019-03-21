package com.suninfo.cloud.ndlp.discover.fdaemond;

import java.util.Properties;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerRecord;

public class LogProducer {
	Producer<String, String> producer = null;
	String m_topic = new String();
	
	LogProducer(String topic)
	{
		Properties props = new Properties();
		 props.put("bootstrap.servers", "node1:9092");
		 props.put("acks", "all");
		 props.put("retries", 0);
		 props.put("batch.size", 16384);
		 props.put("linger.ms", 1);
		 props.put("buffer.memory", 33554432);
		 props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
		 props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");
		 
		 producer = new KafkaProducer<>(props);
		 m_topic = topic;
	}
	
	public void SendLog(String log)
	{
		producer.send(new ProducerRecord<String, String>(m_topic, null, log));
    }
}
