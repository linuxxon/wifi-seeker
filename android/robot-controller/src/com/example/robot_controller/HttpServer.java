package com.example.robot_controller;

import java.io.IOException;
import java.util.Map;
import java.util.Properties;

public class HttpServer extends NanoHTTPD{
    public HttpServer() throws IOException{
        super (null,8080 );
    }

    @Override
    public Response serve(String uri, Method method, Map<String, String> headers, Map<String, String> parms,
                          Map<String, String> files) {
        return new NanoHTTPD.Response(Response.Status.OK, NanoHTTPD.MIME_HTML, "works");
    }
}
