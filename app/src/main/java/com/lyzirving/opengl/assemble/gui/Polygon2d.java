package com.lyzirving.opengl.assemble.gui;

import android.graphics.PointF;

/**
 * @author lyzirving
 */
public class Polygon2d {
    public PointF mLeftTop;
    public PointF mLeftBottom;
    public PointF mRightTop;
    public PointF mRightBottom;

    public Polygon2d () {
        mLeftTop = new PointF();
        mLeftBottom = new PointF();
        mRightTop = new PointF();
        mRightBottom = new PointF();
    }
}
