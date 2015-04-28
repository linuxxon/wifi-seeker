package com.example.robot_controller;

import android.content.*;
import android.net.wifi.*;
import java.lang.reflect.*;

public class WifiApManager {
    // Turn wifi hotspot on or off
    public static boolean setHotspotState(Context context, boolean state) {
        WifiManager wifimanager = (WifiManager) context.getSystemService(context.WIFI_SERVICE);
        WifiConfiguration wificonfiguration = null;
        try {
            //turn Wifi off
            wifimanager.setWifiEnabled(false);

            //enable/disable Wifi hotspot
            Method method = wifimanager.getClass().getMethod("setWifiApEnabled", WifiConfiguration.class, boolean.class);
            method.invoke(wifimanager, wificonfiguration, state);
            return true;
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }
}




