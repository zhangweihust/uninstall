package com.example.uninstall;

/**
 * 
 * 
 * @Author ZGY
 * @Date:2013-12-13
 * @version 
 * @since
 * C代码参考网上资料进行修改的，国外开源代码
 * 
 * 欢迎加群：88130145
 */

public class UninstallObserver {

	static{
		System.loadLibrary("observer");
	}
	
	//command_path: /data/data/[packageNmae]/files
	//watch_path:  /data/data/[packageNmae]/lib
	//processName com.cmmobi.looklook
	//url:跳转的页面，需要http://或https://开头
	public static native String startWork(String command_path, String watch_path, String url, int version);
}
