package com.lyzirving.opengl.assemble;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

import java.util.ArrayList;
import java.util.List;

public class LineTestView extends View {
    private PointF mStart;
    private PointF mEnd;
    private Paint mPaint;

    private PointF mLeftTop;
    private PointF mLeftBottom;
    private PointF mRightTop;
    private PointF mRightBottom;

    private int mLineWidth = 150;

    public LineTestView(Context context) {
        this(context, null);
    }

    public LineTestView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public LineTestView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mStart = new PointF();
        mEnd = new PointF();
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    }


    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        int width = getMeasuredWidth();
        int height = getMeasuredHeight();
        mStart.x = width / 2.f - width / 3.f;
        mStart.y = height / 4.f;
        mEnd.x = width / 2.f + width / 4.f;
        mEnd.y = height / 3.f;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        mPaint.setColor(Color.BLUE);
        mPaint.setStyle(Paint.Style.FILL);
        mPaint.setStrokeWidth(3);
        canvas.drawLine(mStart.x, mStart.y, mEnd.x, mEnd.y, mPaint);

        polygon();
        mPaint.setColor(Color.RED);
        mPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        mPaint.setColor(Color.BLACK);
        mPaint.setStrokeWidth(10);
        canvas.drawPoint(mLeftTop.x, mLeftTop.y, mPaint);
        canvas.drawPoint(mLeftBottom.x, mLeftBottom.y, mPaint);
        canvas.drawPoint(mRightTop.x, mRightTop.y, mPaint);
        canvas.drawPoint(mRightBottom.x, mRightBottom.y, mPaint);
    }

    private void polygon() {
        if (mLeftTop == null) {
            float slope = (mEnd.y - mStart.y) / (mEnd.x - mStart.x);
            float pSlope = -1.f / slope;
            PointF realSt = new PointF();
            PointF realEnd = new PointF();
            if (mStart.x < mEnd.x) {
                realSt.set(mStart.x, mStart.y);
                realEnd.set(mEnd.x, mEnd.y);
            } else {
                realSt.set(mEnd.x, mEnd.y);
                realEnd.set(mStart.x, mStart.y);
            }
            double angle = Math.atan(pSlope);
            if(pSlope < 0) {
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
