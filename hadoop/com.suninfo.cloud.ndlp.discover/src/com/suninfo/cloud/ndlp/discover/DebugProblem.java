package com.suninfo.cloud.ndlp.discover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DebugProblem {
	
	public static void main(String[] args) throws Exception
	{
		String meta = new String();
		if(args.length == 0)
			meta= "/user/wzl/file3.meta";
		else
			meta = args[0];
		
		PolicyManager pm = new PolicyManager();
		pm.load_policy("/user/wzl/policy");
		
		Map dlog = new HashMap<Object, Object>();
		if(pm.discover(meta, dlog))
		{
			String strLog = GJson.Map2String(dlog);
			System.out.println(strLog);
		}
		
		System.out.println("exit");
	}
}
