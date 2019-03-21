package com.suninfo.cloud.ndlp.discover.fdaemond;

import java.util.List;
import java.util.Properties;

import org.apache.commons.lang.time.StopWatch;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;

import com.suninfo.cloud.ndlp.discover.client.DisListString;

public class FileConsumer {
	KafkaConsumer<String, String> consumer = null;
	
	FileConsumer(String groupId, List<String> topics)
	{
		Properties props = new Properties();
		props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "node1:9092");
		props.put(ConsumerConfig.GROUP_ID_CONFIG, "discover" + groupId);
		props.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "true");
		props.put(ConsumerConfig.AUTO_COMMIT_INTERVAL_MS_CONFIG, "1000");
		props.put(ConsumerConfig.SESSION_TIMEOUT_MS_CONFIG, "30000");
		//props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.IntegerDeserializer");
		props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
		props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
		
		consumer = new KafkaConsumer<>(props);
		consumer.subscribe(topics);
	}
	
	void getMessage(DisListString message, long linger)
	{
		StopWatch sw = new StopWatch();
		sw.start();
		while(sw.getTime() <= linger)
		{
			ConsumerRecords<String, String> records = consumer.poll(1000);
			for (ConsumerRecord<String, String> record : records)
			{
			    //System.out.println("Received message: (" + record.key() + ", " + record.value() + ") at offset " + record.offset());
				message.insertString(record.value());
			}
		}
		sw.stop();
	}
}

