package com.lyzirving.opengl.assemble.renderer;

/**
 * @author lyzirving
 */
public class RendererContext {
    private static final long INVALID_ADDRESS = -1;
    static { System.loadLibrary("lib-assembler"); }


    private long mAddress = INVALID_ADDRESS;
    private String mName;

    private RendererContext(long address, String name) {
        mAddress = address;
        mName = name;
    }

    public static RendererContext createContext(String name) {
        return new RendererContext(nCreateContext(name), name);
    }

    public static void releaseContext(RendererContext context) {
        if(context.mAddress != INVALID_ADDRESS) {
            nRelease(context.mAddress);
            context.mAddress = INVALID_ADDRESS;
        }
    }

    public void sendMessage(int what) {
        if(mAddress != INVALID_ADDRESS) {
            nSendMessage(mAddress, what);
        }
    }

    private static native long nCreateContext(String name);
    private static native void nSendMessage(long address, int what);
    private static native void nRelease(long address);
}
