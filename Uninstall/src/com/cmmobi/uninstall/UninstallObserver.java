package com.cmmobi.uninstall;

/**
 * 
 * 
 */

public class UninstallObserver {

	static{
		System.loadLibrary("observer");
	}
	
	//command_path: /data/data/[packageNmae]/files
	//watch_path:  /data/data/[packageNmae]/lib
	//processName com.cmmobi.looklook
	//url:跳转的页面，需要http://或https://开头
	public static native String startWork(String command_path, String command_name, String watch_path, String url, int version);
}
