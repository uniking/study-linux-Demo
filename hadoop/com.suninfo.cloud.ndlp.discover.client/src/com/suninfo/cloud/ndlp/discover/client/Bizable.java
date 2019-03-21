package com.suninfo.cloud.ndlp.discover.client;

public interface Bizable {
	public static final long versionID = 1L;
	void ResetDispatcher();
	void closeServer();
	
	// return log
	DisListString discover(DisListString meta_path);
}