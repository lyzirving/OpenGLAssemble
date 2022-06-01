package com.lyzirving.opengl.assemble.gui;

import android.graphics.PointF;

public class Vector2d {
    public int mDx;
    public int mDy;

    public Vector2d(PointF start, PointF end) {
        mDx = (int)(end.x) - (int)(start.x);
        mDy = (int)(end.y) - (int)(start.y);
    }

    public float slope() {
        if(mDx == 0) {
            return Float.MAX_VALUE;
        } else if (mDy == 0) {
            return 0;
        } else {
            return -(float) (mDy) / (float) (mDx);
        }
    }

    public double length() {
        return Math.sqrt(mDx * mDx + mDy * mDy);
    }
}
