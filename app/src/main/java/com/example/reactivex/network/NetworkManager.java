package com.example.reactivex.network;

import android.content.Context;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import rx.Single;
import rx.schedulers.Schedulers;

public class NetworkManager {

    static {
        System.loadLibrary("reactivex");
    }

    public Single<String> sendPostRequest(String url) {
        return Single.<String> create(singleEmitter -> {
            try{
                singleEmitter.onSuccess(sendNativePostRequest(url));
            }catch(Exception e){
                singleEmitter.onError(e);
            }
        }).subscribeOn(Schedulers.io());
    }

    public Single<String> sendGetRequest(String url) {
        return Single.<String> create(singleEmitter -> {
            try{
                singleEmitter.onSuccess(sendNativeGetRequest(url));
            }catch(Exception e){
                singleEmitter.onError(e);
            }
        }).subscribeOn(Schedulers.io());
    }

    public native String sendNativePostRequest(String url);

    public native String sendNativeGetRequest(String url);

}
