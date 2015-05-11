/*  This file is part of Wifi Seeker.
 *
 *  Copyright (C) 2015  Niklas Bergh, Rasmus Linusson, Araxi Mekhitarian,
 *                      Sebastian Törnqvist, Max von Zedtwitz-Liebenstein
 *
 *  Wifi Seeker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Wifi Seeker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Wifi Seeker.  If not, see <http://www.gnu.org/licenses/>.
 */

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
            if (state)
                wifimanager.setWifiEnabled(false);
            else
                wifimanager.setWifiEnabled(true);

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




