package com.example.robot_controller;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.FileNameMap;
import java.net.URLConnection;
import java.util.Map;
import java.util.Properties;
import java.util.Random;

public class HttpServer extends NanoHTTPD{
    public HttpServer() throws IOException{
        super (null,8080 );
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
                    return new NanoHTTPD.Response(Response.Status.OK, MIME_HTML, mbuffer);
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;

    }
}
