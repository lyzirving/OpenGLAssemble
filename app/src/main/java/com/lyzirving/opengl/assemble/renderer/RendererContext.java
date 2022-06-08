package com.lyzirving.opengl.assemble.renderer;

import android.content.Context;
import android.content.res.AssetManager;
import android.view.Surface;

import com.lyzirving.opengl.assemble.utils.LogUtil;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

import androidx.annotation.IntDef;

/**
 * @author lyzirving
 */
public class RendererContext {
    private static final long INVALID_ADDRESS = -1;
    private static final String TAG = "RendererContext";
    static { System.loadLibrary("lib-assembler"); }

    private static final int BASE = 1;
    private static final int CURVE = 2;

    @IntDef({BASE, CURVE})
    @Retention(RetentionPolicy.SOURCE)
    public @interface ContextType {}


    private long mAddress = INVALID_ADDRESS;
    private String mName;

    protected RendererContext() {}

    protected RendererContext(long address, String name) {
        mAddress = address;
        mName = name;
    }

    public static RendererContext createContext(Context context,
                                                String className,
                                                String name) {
        LogUtil.logI(TAG, "createContext: class " + className);
        if(RendererContext.class.getName().equals(className)) {
            return new RendererContext(nCreateContext(name, context.getAssets(), BASE), name);
        } else if(CurveRendererContext.class.getName().equals(className)) {
            return new CurveRendererContext(nCreateContext(name, context.getAssets(), CURVE), name);
        } else {
            return null;
        }
    }

    public static void releaseContext(RendererContext context) {
        if(context.mAddress != INVALID_ADDRESS) {
            nRelease(context.mAddress);
            context.mAddress = INVALID_ADDRESS;
        }
    }

    public void registerWindow(String name, Surface window) {
        if(mAddress != INVALID_ADDRESS) {
            nRegisterWindow(mAddress, name, window);
        }
    }

    public void removeWindow(String name) {
        if(mAddress != INVALID_ADDRESS) {
           nRemoveWindow(mAddress, name);
        }
    }

    public void sendMessage(@RendererConstant.Message int what, int arg0, int arg1) {
        if(mAddress != INVALID_ADDRESS) {
            nSendMessage(mAddress, what, arg0, arg1);
        }
    }

    private static native long nCreateContext(String name, AssetManager assetManager, int type);
    private static native void nSendMessage(long address, int what, int arg0, int agr1);
    private static native void nRelease(long address);
    private static native void nRegisterWindow(long address, String name, Surface window);
    private static native void nRemoveWindow(long address, String name);
}
