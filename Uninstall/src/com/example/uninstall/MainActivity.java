package com.example.uninstall;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import com.cmmobi.uninstall.UninstallObserver;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;

public class MainActivity extends Activity {
	private static final String ASSET_FILENAME = "uninstall_watcher";
	private static final String FILENAME = "uninstall_watcher_com.md5";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		String packageName = this.getApplicationContext().getPackageName();
		String watch_path = "/data/data/" + packageName + "/lib";
		String command_path = "/data/data/" + packageName + "/files";
		String url = "http://www.baidu.com";
		int version = android.os.Build.VERSION.SDK_INT;
		
		copyAssertToData();
		
		chmod(null, command_path + "/" + FILENAME);
		
		Log.e("Uninstall", "Uninstall watch_path:" + watch_path + ", " + "url:" + url + ", version:" + version);
		UninstallObserver.startWork(command_path, FILENAME, watch_path, url, version);

	}
	
	public static void chmod(String permission, String path) {
	    try {
	        //String command = "chmod +x " + permission + " " + path;
	        String command = "chmod 777 " + path;
	        
	        Runtime runtime = Runtime.getRuntime();
	        runtime.exec(command);
	        
	    } catch (IOException e) {
	        e.printStackTrace();
	    }
	}
	
	private void copyAssertToData(){
		boolean hasFile = false;
		try {
			FileInputStream input = openFileInput(FILENAME);
			hasFile = true;
			input.close();
		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		if(hasFile){
			return;
		}
		
		try {

			//Access application assets
			AssetManager manager = getAssets();

			//Open input and output data file
			InputStream mInput = manager.open(ASSET_FILENAME);
			FileOutputStream mOutput = openFileOutput(FILENAME, Activity.MODE_PRIVATE);

			//Read data in
			byte[] data = new byte[1024];
			int len = 0;
			while((len = mInput.read(data))>0){
				mOutput.write(data,0, len);
			}
			
			mInput.close();
			mOutput.close();

			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}


	}

	
    

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
