package com.example.robot_controller;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	
	WifiManager wifi;
	TimerTask timerListenToRobot;
	Timer timer;	
    TextView debugOut;
    StringBuilder logger;
    SimpleDateFormat timeString;
    
    boolean logAP=false;
    
    // USB data
    private Boolean mIsReceiving;
    private ArrayList<ByteArray> mTransferedDataList = new ArrayList<ByteArray>();
    private ArrayAdapter<ByteArray> mDataAdapter;
	// End of USB data
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);		
		
		logger = new StringBuilder();
		timeString = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		
		setContentView(R.layout.activity_main);
		debugOut = (TextView) findViewById(R.id.debug_window);
		
		wifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        
        // Turn on wifi if it is turned off
        if (wifi.isWifiEnabled() == false) {
        	// If wifi disabled then enable it
            Toast.makeText(getApplicationContext(), "Wifi is turned off, turning it on",
            Toast.LENGTH_LONG).show();                  
            wifi.setWifiEnabled(true);
        } 
        
        // Set up receiver
        class WifiAndUsbReceiver extends BroadcastReceiver {
        	
        	private void handleTransferedData(Intent intent, boolean receiving) {
        		if (mIsReceiving == null || mIsReceiving != receiving) {
                    mIsReceiving = receiving;
                    mTransferedDataList.add(new ByteArray());
                }

                 final byte[] newTransferedData = intent.getByteArrayExtra(ComService.DATA_EXTRA);

                 ByteArray transferedData = mTransferedDataList.get(mTransferedDataList.size() - 1);
                 if (!"primavera.arduino.intent.action.SEND_DATA".equals(intent.getAction()))
                     transferedData.add(newTransferedData);

                 mTransferedDataList.set(mTransferedDataList.size() - 1, transferedData);
                 mDataAdapter.notifyDataSetChanged();
        	}        	
        	
        	@Override   
        	public void onReceive(Context c, Intent intent) {      		
        		final String action = intent.getAction();        		
        		
        		if (ComService.DATA_RECEIVED_INTENT.equals(action)) {
                    handleTransferedData(intent, true);
                } else if (ComService.DATA_SENT_INTERNAL_INTENT.equals(action)) {
                    handleTransferedData(intent, false);
                } else if ("primavera.arduino.intent.action.REQUEST_RESPONSE".equals(action)) {
                    // USB command received
//                	Toast.makeText(getBaseContext(), "Data Receieved"+intent.getStringArrayListExtra("primavera.arduino.intent.extra.DATA").get(0), Toast.LENGTH_SHORT).show();                    
                    List<String> commands = intent.getStringArrayListExtra("primavera.arduino.intent.extra.DATA");                   
              
                    debugOut.append("Command received: " + commands.get(0) + "\n");
                    debugOut.append("Command buffer: ");
                    for (String command : commands) {
                    	debugOut.append(command + " ");
                    }
                    debugOut.append("\n");
                    
                    if (commands.get(0).equals("wifi-scan-plz")) {
                		if (!logAP) {
//                			debugOut.append("Command received: 'button wifi scan'\n");
                			wifi.startScan();
                			logger.append("Button scan " + timeString.format(Calendar.getInstance().getTime()) + "\n");
                			logAP=true;
                		}	
                    }                    
                }       		
                else { 
                	// Wifi receiving
                	if (logAP){
            			debugOut.append("Wifi scanned, logging\n");  
            			
            			List<android.net.wifi.ScanResult> scanResults = wifi.getScanResults();
            			
            			for(android.net.wifi.ScanResult result: scanResults){
            				logger.append(result.SSID + " " + result.BSSID + " " + result.level + " " + result.frequency + "\n");
            		    }
            			logger.append("\n");
            			
            			logAP=false; // Stop logging until log is true
            		}        		
            		else {
            			debugOut.append("Wifi scanned, not logging\n");  
            		}
                }        		
        	}
        }
        
        WifiAndUsbReceiver wifiAndUsbReceiver = new WifiAndUsbReceiver();
//        registerReceiver(wifiAndUsbReceiver, new IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));
        
        IntentFilter filter = new IntentFilter();
        filter.addAction(ComService.DATA_RECEIVED_INTENT);
        filter.addAction(ComService.DATA_SENT_INTERNAL_INTENT);
        filter.addAction("primavera.arduino.intent.action.REQUEST_RESPONSE");
        filter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION); // Wifi "action"
        registerReceiver(wifiAndUsbReceiver, filter);

        mDataAdapter = new ArrayAdapter<ByteArray>(this, android.R.layout.simple_list_item_1, mTransferedDataList);
//        setListAdapter(mDataAdapter); // Potential problem

        byte[] arr = {0x56,0x45,0x32};
        ByteArray tD = new ByteArray();
        tD.add(arr);
        //ByteArray transferedData = new ByteArray().add(arr);
        mTransferedDataList.add(tD);
        mDataAdapter.notifyDataSetChanged();        
	}
	
	public void listenToRobot(View view) {
		final Handler handler = new Handler(); 
		timer = new Timer();
		timerListenToRobot = new TimerTask(){
			@Override
			public void run() {
				handler.post(new Runnable() {
					public void run() {
//						debugOut.append("Listening\n");
					     Intent readIntent = new Intent("primavera.arduino.intent.action.GET_REQUEST");
					     sendBroadcast(readIntent);						
					}
				});
			}			
		};
		
		timer.schedule(timerListenToRobot,0,500);
	}	
	
	public void checkConnection(View view) {
	    final int ARDUINO_USB_VENDOR_ID = 0x2341;
	    final int ARDUINO_UNO_USB_PRODUCT_ID = 0x01;
	    final int ARDUINO_UNO_R3_USB_PRODUCT_ID = 0x43;
		
		UsbManager usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
	    UsbDevice usbDevice = null;
	    HashMap<String, UsbDevice> usbDeviceList = usbManager.getDeviceList();
	    debugOut.append("Number of devices: " + usbDeviceList.size() + "\n");
	    Iterator<UsbDevice> deviceIterator = usbDeviceList.values().iterator();
	    if (deviceIterator.hasNext()) {	    	
	        UsbDevice tempUsbDevice = deviceIterator.next();

	        if (tempUsbDevice.getVendorId() == ARDUINO_USB_VENDOR_ID) {
	        	debugOut.append("Arduino device found!\n");

	            switch (tempUsbDevice.getProductId()) {
	            case ARDUINO_UNO_USB_PRODUCT_ID:
//	            	Toast.makeText(getBaseContext(), "Arduino Uno " + getString(R.string.found), Toast.LENGTH_SHORT).show();
	                usbDevice = tempUsbDevice;
	                break;	                
	             case ARDUINO_UNO_R3_USB_PRODUCT_ID:
//	            	 Toast.makeText(getBaseContext(), "Arduino Uno R3 " + getString(R.string.found), Toast.LENGTH_SHORT).show();
	                 usbDevice = tempUsbDevice;
	                 break;
	            }            
	        }
	    }
	    
	    if (usbDevice == null) {
	    	debugOut.append("No device found!\n");
//	        Toast.makeText(getBaseContext(), getString(R.string.no_device_found), Toast.LENGTH_LONG).show();
	        
	    } else {
	    	debugOut.append("Device found!\n");
	        Intent startIntent = new Intent(getApplicationContext(), ComService.class);
	        PendingIntent pendingIntent = PendingIntent.getService(getApplicationContext(), 0, startIntent, 0);
	        usbManager.requestPermission(usbDevice, pendingIntent);
	    }		
	}
	
	public void stopRobot(View view) {
		
	}
	
	public void manualScan(View view) {
		if (!logAP) {
			debugOut.append("Scanning manually\n");
			wifi.startScan();
			logger.append("Manual scan " + timeString.format(Calendar.getInstance().getTime()) + "\n");
			logAP=true;
		}		
	}
	
	public void saveLog(View view) {
		try {
			File myFile = new File("/sdcard/wifiLog.txt");
	        myFile.createNewFile();
	        FileOutputStream fOut = new FileOutputStream(myFile);
	        OutputStreamWriter myOutWriter =new OutputStreamWriter(fOut);
	        myOutWriter.write(logger.toString());
	        myOutWriter.close();
	        fOut.close();
	       
		} catch (IOException ioe) {ioe.printStackTrace();}
		
		debugOut.append("Data saved\n");
	}

    public void hotspotOn(View view) {
        WifiApManager.setHotspotState(this,true);
        debugOut.append("Hotspot turning on\n");
    }
    public void hotspotOff(View view) {
        WifiApManager.setHotspotState(this, false);
        debugOut.append("Hotspot turning off\n");
    }
}
 