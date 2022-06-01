package com.lyzirving.opengl.assemble.gui;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import androidx.annotation.Nullable;

/**
 * @author lyzirving
 */
public class LineTestView extends View {
    private PointF mPt1;
    private PointF mPt2;
    private PointF mPt3;
    private Paint mPaint;

    private float mPolygon1[];
    private float mPolygon2[];

    private PointF mLeftTop;
    private PointF mLeftBottom;
    private PointF mRightTop;
    private PointF mRightBottom;

    private int mLineWidth = 100;

    public LineTestView(Context context) {
        this(context, null);
    }

    public LineTestView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public LineTestView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mPt1 = new PointF();
        mPt2 = new PointF();
        mPt3 = new PointF();
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);

        mPolygon1 = new float[8];
        mPolygon2 = new float[8];
    }


    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        int width = getMeasuredWidth();
        int height = getMeasuredHeight();
        mPt1.x = width / 2.f - width / 3.f;
        mPt1.y = height / 2.f + height / 3.f;
        mPt2.x = width / 2.f - width / 3.f + width / 5.f;
        mPt2.y = height / 2.f + height / 4.f;;
        mPt3.x = width / 2.f + width / 4.f;
        mPt3.y = height / 2.f - height / 3.f;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        polygon2();

        mPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        mPaint.setColor(Color.BLACK);
        mPaint.setStrokeWidth(20);
        canvas.drawPoint(mPt1.x, mPt1.y, mPaint);
        canvas.drawPoint(mPt2.x, mPt2.y, mPaint);
        canvas.drawPoint(mPt3.x, mPt3.y, mPaint);

        mPaint.setColor(Color.BLUE);
        mPaint.setStyle(Paint.Style.FILL);
        mPaint.setStrokeWidth(3);
        canvas.drawLine(mPt1.x, mPt1.y, mPt2.x, mPt2.y, mPaint);
        canvas.drawLine(mPt2.x, mPt2.y, mPt3.x, mPt3.y, mPaint);
    }

    private void polygon2() {
        Vector2d leftVec = new Vector2d(mPt2, mPt1);
        Vector2d rightVec = new Vector2d(mPt2, mPt3);
        //vector dot to calculate the value of cos(2 * angle)
        double cos2Angle = (leftVec.mDx * rightVec.mDx + leftVec.mDy * rightVec.mDy) / (leftVec.length() * rightVec.length());
        double cosAngle = Math.sqrt((cos2Angle + 1) / 2f);
        double angle = (180.f / Math.PI) * Math.acos(cosAngle);
        Log.i("test", "half angle = " + angle);
    }

    private void polygon() {
        if (mLeftTop == null) {
            float slope = -(mPt2.y - mPt1.y) / (mPt2.x - mPt1.x);
            float pSlope = -1.f / slope;
            PointF realSt = new PointF();
            PointF realEnd = new PointF();
            if (mPt1.x < mPt2.x) {
                realSt.set(mPt1.x, mPt1.y);
                realEnd.set(mPt2.x, mPt2.y);
            } else {
                realSt.set(mPt2.x, mPt2.y);
                realEnd.set(mPt1.x, mPt1.y);
            }
            if(pSlope < 0) {
                double angle = Math.atan(-pSlope);
                mLeftTop = new PointF();
                mLeftTop.x = (float) (realSt.x - mLineWidth / 2.f * Math.cos(angle));
                mLeftTop.y = (float) (realSt.y - mLineWidth / 2.f * Math.sin(angle));

                mLeftBottom = new PointF();
                mLeftBottom.x = (float) (realSt.x + mLineWidth / 2.f * Math.cos(angle));
                mLeftBottom.y = (float) (realSt.y + mLineWidth / 2.f * Math.sin(angle));

                mRightTop = new PointF();
                mRightTop.x = (float) (realEnd.x - mLineWidth / 2.f * Math.cos(angle));
                mRightTop.y = (float) (realEnd.y - mLineWidth / 2.f * Math.sin(angle));

                mRightBottom = new PointF();
                mRightBottom.x = (float) (realEnd.x + mLineWidth / 2.f * Math.cos(angle));
                mRightBottom.y = (float) (realEnd.y + mLineWidth / 2.f * Math.sin(angle));
            } else {
                double angle = Math.atan(pSlope);
                mLeftTop = new PointF();
                mLeftTop.x = (float) (realSt.x + mLineWidth / 2.f * Math.cos(angle));
                mLeftTop.y = (float) (realSt.y - mLineWidth / 2.f * Math.sin(angle));

                mLeftBottom = new PointF();
                mLeftBottom.x = (float) (realSt.x - mLineWidth / 2.f * Math.cos(angle));
                mLeftBottom.y = (float) (realSt.y + mLineWidth / 2.f * Math.sin(angle));

                mRightTop = new PointF();
                mRightTop.x = (float) (realEnd.x + mLineWidth / 2.f * Math.cos(angle));
                mRightTop.y = (float) (realEnd.y - mLineWidth / 2.f * Math.sin(angle));

                mRightBottom = new PointF();
                mRightBottom.x = (float) (realEnd.x - mLineWidth / 2.f * Math.cos(angle));
                mRightBottom.y = (float) (realEnd.y + mLineWidth / 2.f * Math.sin(angle));
            }
        }
    }
}
