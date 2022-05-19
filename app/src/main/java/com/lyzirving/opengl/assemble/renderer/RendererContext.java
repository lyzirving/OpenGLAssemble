package com.lyzirving.opengl.assemble.renderer;

/**
 * @author lyzirving
 */
public class RendererContext {
    private long mContext;
    private String mName;

    private RendererContext(long address, String name) {
        mContext = address;
        mName = name;
    }

    public static RendererContext createContext(String name) {
        return new RendererContext(nCreateContext(name), name);
    }

    private static native long nCreateContext(String name);
}
