package com.example.robot_controller;

import java.io.BufferedReader;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.FileNameMap;
import java.net.URLConnection;
import java.util.Map;
import java.util.Random;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.util.Scanner;


public class HttpServer extends NanoHTTPD{
    public HttpServer(Context c) throws IOException{
        super (null,8080 );
        context = c;
    }

    Context context;
	
    String currentCoords = null;

    public void saveCoordinates(Map<String, String> params) {
        try {
            if (params.get("x") == null)
                throw new IOException("No coordinates sent");

            File myFile = new File("/sdcard/www/coordinates.txt");
            myFile.createNewFile();
            FileOutputStream fOut = new FileOutputStream(myFile);
            OutputStreamWriter myOutWriter = new OutputStreamWriter(fOut);
            String coords = params.get("x") + "," + params.get("y");

            // Update class-wide variable of current coordinates
            currentCoords =coords;
            myOutWriter.write(coords);
            myOutWriter.close();
            fOut.close();

        } catch (IOException ioe) {
            ioe.printStackTrace();
            currentCoords = "-1,-1";
        }
    }

    public static String editFileRow(FileInputStream fis, String ref, String s) throws IOException
    {
        BufferedReader br = new BufferedReader(new InputStreamReader(fis));
        try {
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = br.readLine()) != null) {
                if(line.contains(ref)) {

                    sb.append(s);
                    sb.append('\n');
                }
                else {
                    sb.append(line);
                    sb.append('\n');
                }
            }
            return sb.toString();
        } finally {
            br.close();
        }
    }

    @Override
    public Response serve(String uri, Method method, Map<String, String> headers, Map<String, String> parms,
                          Map<String, String> files) {

        //return new NanoHTTPD.Response(Response.Status.OK, NanoHTTPD.MIME_HTML, "works");

        final StringBuilder buf = new StringBuilder();
        InputStream mbuffer = null;

        try {
            if(uri!=null){
                if(uri.contains(".js")){
                    //mbuffer = mContext.getAssets().open(uri.substring(1));
                    mbuffer = new FileInputStream("/sdcard/www/"+uri.substring(1));
                    return new NanoHTTPD.Response(Response.Status.OK, MIME_JS, mbuffer);
                }else if(uri.contains(".css")){
                    //mbuffer = mContext.getAssets().open(uri.substring(1));
                    mbuffer = new FileInputStream("/sdcard/www/"+uri.substring(1));
                    return new NanoHTTPD.Response(Response.Status.OK, MIME_CSS, mbuffer);

                }else if(uri.contains(".png")) {
                    //mbuffer = mContext.getAssets().open(uri.substring(1));
                    mbuffer = new FileInputStream("/sdcard/www/" + uri.substring(1));
                    // Response.Status.OK = "200 OK" or Response.Status.OK = Status.OK;(check comments)
                    return new NanoHTTPD.Response(Response.Status.OK, MIME_PNG, mbuffer);
                }else if(uri.contains(".bmp")){
                    //mbuffer = mContext.getAssets().open(uri.substring(1));
                    mbuffer = new FileInputStream("/sdcard/www/"+uri.substring(1));
                    // Response.Status.OK = "200 OK" or Response.Status.OK = Status.OK;(check comments)
                    return new NanoHTTPD.Response(Response.Status.OK, MIME_BMP, mbuffer);
                }else if(uri.contains(".txt")){
                    //mbuffer = mContext.getAssets().open(uri.substring(1));
                    mbuffer = new FileInputStream("/sdcard/www/"+uri.substring(1));
                    // Response.Status.OK = "200 OK" or Response.Status.OK = Status.OK;(check comments)
                    return new NanoHTTPD.Response(Response.Status.OK, MIME_PLAINTEXT, mbuffer);
                }else if (uri.contains("/mnt/sdcard")){
                    File request = new File(uri);
                    //mbuffer = new FileInputStream(request);
                    FileNameMap fileNameMap = URLConnection.getFileNameMap();
                    String mimeType = fileNameMap.getContentTypeFor(uri);

                    Response streamResponse = new Response(Response.Status.OK, mimeType, mbuffer);
                    Random rnd = new Random();
                    String etag = Integer.toHexString( rnd.nextInt() );
                    streamResponse.addHeader( "ETag", etag);
                    streamResponse.addHeader( "Connection", "Keep-alive");
                    return streamResponse;
                }else{
                    //mbuffer = mContext.getAssets().open("index.html");
                    mbuffer = new FileInputStream("/sdcard/www/index.html");
                    if(parms.get("cmd" ) != null) {
                        if (parms.get("cmd").equals("clearResult")) {
                            Intent clearResult = new Intent("com.example.robot_controller.CLEARRESULT");
                            context.sendBroadcast(clearResult);
                        }
                        else if (parms.get("cmd").equals("scanWifi")){
                            Intent intent = new Intent("com.example.robot_controller.WEBSCAN");
                            context.sendBroadcast(intent);
                        }
					}	
                    else if (parms.get("x") != null && parms.get("y") != null){
                        saveCoordinates(parms);
                    }

                    String data = new Scanner(new File("/sdcard/www/wifiLog.txt")).useDelimiter("\\A").next();
                    String mbufferString = "";
                    if(currentCoords != null){
                        mbuffer = new FileInputStream("/sdcard/www/index.html");
                        // insert coordinates into html-code
                        mbufferString= editFileRow((FileInputStream) mbuffer, "coords =", "coords =[" +currentCoords+"]");
                        mbufferString = mbufferString.replaceFirst("data = \\[\\];", "data = "+data+";" );
                    }
                    else {
                        mbuffer = new FileInputStream("/sdcard/www/index.html");
                        mbufferString = editFileRow((FileInputStream) mbuffer, "data = [];", "data = "+data+";");
                    }
                    return new NanoHTTPD.Response(Response.Status.OK, MIME_HTML, mbufferString);
                }

                //return new NanoHTTPD.Response(Response.Status.OK, MIME_HTML, mbuffer);
            }



        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;

    }

    // Get coordinates, if no coordinates are found, load from file.
    public String getCoordinates() {
        if (currentCoords == null) {
            try {
                currentCoords = new Scanner(new File("/sdcard/www/coordinates.txt")).useDelimiter("\\A").next();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                return null;
            }
        }
        return currentCoords;
    }
}