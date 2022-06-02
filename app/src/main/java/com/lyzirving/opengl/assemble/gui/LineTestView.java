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
    private PointF mPtHalf;
    private Paint mPaint;

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
        mPtHalf = new PointF();
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    }


    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        int width = getMeasuredWidth();
        int height = getMeasuredHeight();
        mPt1.x = width / 2.f - width / 3.f;
        mPt1.y = height / 2.f;
        mPt2.x = width / 2.f - width / 3.f + width / 5.f;
        mPt2.y = height / 2.f - height / 4.f;
        mPt3.x = width / 2.f - width / 3.f - width / 5.f;
        mPt3.y = height / 2.f - height / 4.f - height / 6.f;
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
        //canvas.drawPoint(mPtHalf.x, mPtHalf.y, mPaint);

        mPaint.setColor(Color.BLUE);
        mPaint.setStyle(Paint.Style.FILL);
        mPaint.setStrokeWidth(3);
        canvas.drawLine(mPt1.x, mPt1.y, mPt2.x, mPt2.y, mPaint);
        canvas.drawLine(mPt2.x, mPt2.y, mPt3.x, mPt3.y, mPaint);
    }

    private void polygon2() {
        //todo left vector's slope equals to right vector's slope
        Vector2d lhsVec = new Vector2d(mPt1, mPt2);
        Vector2d rhsVec = new Vector2d(mPt2, mPt3);
        float lhsSlope = lhsVec.slope();
        float rhsSlope = rhsVec.slope();
        double lhsAngle = Math.atan(lhsSlope);
        if (lhsAngle < 0)// limit angle to 0~PI
            lhsAngle += Math.PI;
        double rhsAngle = Math.atan(rhsSlope);
        if (rhsAngle < 0)// limit angle to 0~PI
            rhsAngle += Math.PI;
        double intersectAngle;
        if(lhsAngle > rhsAngle)
            intersectAngle = lhsAngle - rhsAngle;
        else if(lhsAngle == rhsAngle)
            intersectAngle = 0;
        else
            intersectAngle = rhsAngle - lhsAngle;
        Log.i("test", "intersect angle = " + (intersectAngle / Math.PI * 180f));

        if (lhsSlope == 0) {

        } else if (lhsSlope == Float.MAX_VALUE) {

        } else if (lhsSlope > 0) {

        } else {// lhsSlope < 0

        }
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
