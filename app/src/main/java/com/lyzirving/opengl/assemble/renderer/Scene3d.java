package com.lyzirving.opengl.assemble.renderer;

/**
 * @author lyzirving
 */
public class Scene3d extends RendererContext{

    protected Scene3d(long address) {
        super(address);
    }

    public void rotateModel(int angle) {
        if(mAddress != INVALID_ADDRESS)
            nRotateModel(mAddress, angle);
    }

    private static native void nRotateModel(long address, int angle);
}
