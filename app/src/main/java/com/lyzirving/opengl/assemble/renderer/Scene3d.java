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

    public void liftUpVision(float zDist, int angle) {
        if(mAddress != INVALID_ADDRESS)
            nLiftUpVision(mAddress, zDist, angle);
    }

    private static native void nLiftUpVision(long address, float zDist, int angle);
    private static native void nRotateModel(long address, int angle);
}
