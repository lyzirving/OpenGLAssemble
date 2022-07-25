package com.lyzirving.opengl.assemble.renderer;

/**
 * @author lyzirving
 */
public class Scene3d extends RendererContext{

    protected Scene3d(long address) {
        super(address);
    }

    public void adjustFov(float ratio) {
        if(mAddress != INVALID_ADDRESS)
            nAdjustFov(mAddress, ratio);
    }

    public void liftUpVision(float ratio) {
        if(mAddress != INVALID_ADDRESS)
            nLiftUpVision(mAddress, ratio);
    }

    public void rotateModel(int angle) {
        if(mAddress != INVALID_ADDRESS)
            nRotateModel(mAddress, angle);
    }

    private static native void nAdjustFov(long address, float ratio);
    private static native void nLiftUpVision(long address, float ratio);
    private static native void nRotateModel(long address, int angle);
}
