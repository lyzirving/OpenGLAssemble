package com.lyzirving.opengl.assemble.utils;

import android.util.Log;

/**
 * @author lyzirving
 */
public class LogUtil {
    private static final String TAG = "Assembler";

    public static void logD(String tag, String msg) {
        Log.d(TAG, tag + "__" + msg);
    }

    public static void logI(String tag, String msg) {
        Log.i(TAG, tag + "__" + msg);
    }

    public static void logW(String tag, String msg) {
        Log.w(TAG, tag + "__" + msg);
    }

    public static void logE(String tag, String msg) {
        Log.e(TAG, tag + "__" + msg);
    }
}
