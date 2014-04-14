package com.example.uninstall;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;

public class MainActivity extends Activity {

	private static final String FILENAME = "uninstall_watcher";

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
		UninstallObserver.startWork(command_path, watch_path, /*packageName,*/ url, version);

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
			InputStream mInput = manager.open(FILENAME);
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

	
	private static final byte[] testString = new byte[] {3,1,4};
	
    private boolean compareBuffers(byte[] buffer1, byte[] buffer2, int length){
        for (int i = 0; i < length; i++) {
            if (buffer1[i] != buffer2[i]) {
                //throw new Exception("readBytes did not read back what writeBytes wrote");
            	return false;
            }
        }
        
        Log.e("Main", "compareBuffers same!");
        return true;
    }
    
	private boolean checkFirstRun(){/*
		try {
			byte[] buffer = new byte[testString.length];
			
			MemoryFile file = new MemoryFile("MemoryFileTest", 100);
			file.readBytes(buffer, 0, 0, testString.length);
			boolean ret = compareBuffers(testString, buffer, testString.length);
			if(ret){
				//arleady run
				return false;
			}else{
				file.writeBytes(testString, 0, 0, testString.length);
				return true;
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return false;
		}
	*/
		return true;
		}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
