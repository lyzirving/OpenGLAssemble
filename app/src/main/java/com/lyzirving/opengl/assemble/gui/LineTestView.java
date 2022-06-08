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
    private PointF mPtHalf1;
    private PointF mPtHalf2;
    private Polygon2d mPolygon1;
    private Polygon2d mPolygon2;
    private Paint mPaint;

    private PointF mLeftTop;
    private PointF mLeftBottom;
    private PointF mRightTop;
    private PointF mRightBottom;

    private int mLineWidth = 50;

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
        mPtHalf1 = new PointF();
        mPtHalf2 = new PointF();
        mPolygon1 = new Polygon2d();
        mPolygon2 = new Polygon2d();
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        int width = getMeasuredWidth();
        int height = getMeasuredHeight();
        mPt1.x = (width) / 2.f;
        mPt1.y = (height) / 2.f - (height) / 9.f;
        mPt2.x = (width) / 2.f - (width) / 5.f;
        mPt2.y = (height) / 2.f - (height) / 6.f;
        mPt3.x = (width) / 2.f + (width) / 3.f;
        mPt3.y = (height) / 2.f + (height) / 4.f;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        polygon2();
        polygon3(mPolygon1, mPt1, mPt2);
        polygon3(mPolygon2, mPt2, mPt3);

        mPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        mPaint.setColor(Color.BLACK);
        mPaint.setStrokeWidth(20);
        canvas.drawPoint(mPt1.x, mPt1.y, mPaint);
        canvas.drawPoint(mPt2.x, mPt2.y, mPaint);
        canvas.drawPoint(mPt3.x, mPt3.y, mPaint);
        canvas.drawPoint(mPtHalf1.x, mPtHalf1.y, mPaint);
        canvas.drawPoint(mPtHalf2.x, mPtHalf2.y, mPaint);

        mPaint.setColor(Color.BLUE);
        mPaint.setStyle(Paint.Style.FILL);
        mPaint.setStrokeWidth(3);
        canvas.drawLine(mPt1.x, mPt1.y, mPt2.x, mPt2.y, mPaint);
        canvas.drawLine(mPt2.x, mPt2.y, mPt3.x, mPt3.y, mPaint);

        mPaint.setColor(Color.RED);
        canvas.drawLine(mPtHalf1.x, mPtHalf1.y, mPtHalf2.x, mPtHalf2.y, mPaint);

        Vector2d vector1 = new Vector2d(mPt1, mPt2);
        float vec1Slope = vector1.slope();
        if(vec1Slope > 0) {
            if(vector1.mDy < 0) {
                canvas.drawLine(mPolygon1.mLeftTop.x, mPolygon1.mLeftTop.y,
                        mPolygon1.mLeftBottom.x, mPolygon1.mLeftBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPolygon1.mLeftTop, mPtHalf1);
                if (Math.abs(vec1Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPolygon1.mLeftTop.x, mPolygon1.mLeftTop.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                    canvas.drawLine(mPolygon1.mLeftBottom.x, mPolygon1.mLeftBottom.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                }
                else {
                    canvas.drawLine(mPolygon1.mLeftTop.x, mPolygon1.mLeftTop.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                    canvas.drawLine(mPolygon1.mLeftBottom.x, mPolygon1.mLeftBottom.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                }
            }
            else {
                canvas.drawLine(mPolygon1.mRightTop.x, mPolygon1.mRightTop.y,
                        mPolygon1.mRightBottom.x, mPolygon1.mRightBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPolygon1.mRightTop, mPtHalf1);
                if(Math.abs(vec1Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPolygon1.mRightTop.x, mPolygon1.mRightTop.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                    canvas.drawLine(mPolygon1.mRightBottom.x, mPolygon1.mRightBottom.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                } else {
                    canvas.drawLine(mPolygon1.mRightTop.x, mPolygon1.mRightTop.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                    canvas.drawLine(mPolygon1.mRightBottom.x, mPolygon1.mRightBottom.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                }
            }
        }
        else {
            if(vector1.mDy < 0) {
                canvas.drawLine(mPolygon1.mRightTop.x, mPolygon1.mRightTop.y,
                        mPolygon1.mRightBottom.x, mPolygon1.mRightBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPolygon1.mRightTop, mPtHalf1);
                if(Math.abs(vec1Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPolygon1.mRightTop.x, mPolygon1.mRightTop.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                    canvas.drawLine(mPolygon1.mRightBottom.x, mPolygon1.mRightBottom.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                } else {
                    canvas.drawLine(mPolygon1.mRightTop.x, mPolygon1.mRightTop.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                    canvas.drawLine(mPolygon1.mRightBottom.x, mPolygon1.mRightBottom.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                }
            }
            else {
                canvas.drawLine(mPolygon1.mLeftTop.x, mPolygon1.mLeftTop.y,
                        mPolygon1.mLeftBottom.x, mPolygon1.mLeftBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPolygon1.mLeftTop, mPtHalf1);
                if (Math.abs(vec1Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPolygon1.mLeftTop.x, mPolygon1.mLeftTop.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                    canvas.drawLine(mPolygon1.mLeftBottom.x, mPolygon1.mLeftBottom.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                } else {
                    canvas.drawLine(mPolygon1.mLeftTop.x, mPolygon1.mLeftTop.y,
                            mPtHalf2.x, mPtHalf2.y, mPaint);
                    canvas.drawLine(mPolygon1.mLeftBottom.x, mPolygon1.mLeftBottom.y,
                            mPtHalf1.x, mPtHalf1.y, mPaint);
                }
            }
        }

        Vector2d vector2 = new Vector2d(mPt2, mPt3);
        float vec2Slope = vector2.slope();
        if(vec2Slope > 0) {
            if(vector2.mDy < 0) {
                canvas.drawLine(mPolygon2.mRightTop.x, mPolygon2.mRightTop.y,
                        mPolygon2.mRightBottom.x, mPolygon2.mRightBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPtHalf1, mPolygon2.mRightTop);
                if(Math.abs(vec2Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mRightTop.x, mPolygon2.mRightTop.y,mPaint);
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mRightBottom.x, mPolygon2.mRightBottom.y,mPaint);
                } else {
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mRightTop.x, mPolygon2.mRightTop.y,mPaint);
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mRightBottom.x, mPolygon2.mRightBottom.y,mPaint);
                }
            }
            else {
                canvas.drawLine(mPolygon2.mLeftTop.x, mPolygon2.mLeftTop.y,
                        mPolygon2.mLeftBottom.x, mPolygon2.mLeftBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPtHalf1, mPolygon2.mLeftTop);
                if(Math.abs(vec2Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mLeftTop.x, mPolygon2.mLeftTop.y,mPaint);
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mLeftBottom.x, mPolygon2.mLeftBottom.y,mPaint);
                } else {
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mLeftTop.x, mPolygon2.mLeftTop.y,mPaint);
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mLeftBottom.x, mPolygon2.mLeftBottom.y,mPaint);
                }
            }
        }
        else {
            if(vector2.mDy < 0) {
                canvas.drawLine(mPolygon2.mLeftTop.x, mPolygon2.mLeftTop.y,
                        mPolygon2.mLeftBottom.x, mPolygon2.mLeftBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPtHalf1, mPolygon2.mLeftTop);
                if(Math.abs(vec2Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mLeftTop.x, mPolygon2.mLeftTop.y,mPaint);
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mLeftBottom.x, mPolygon2.mLeftBottom.y,mPaint);
                } else {
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mLeftTop.x, mPolygon2.mLeftTop.y,mPaint);
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mLeftBottom.x, mPolygon2.mLeftBottom.y,mPaint);
                }
            }
            else {
                canvas.drawLine(mPolygon2.mRightTop.x, mPolygon2.mRightTop.y,
                        mPolygon2.mRightBottom.x, mPolygon2.mRightBottom.y, mPaint);

                Vector2d compare = new Vector2d(mPtHalf1, mPolygon2.mRightTop);
                if(Math.abs(vec2Slope - compare.slope()) < 0.05) {
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mRightTop.x, mPolygon2.mRightTop.y,mPaint);
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mRightBottom.x, mPolygon2.mRightBottom.y,mPaint);
                } else {
                    canvas.drawLine(mPtHalf2.x, mPtHalf2.y,
                            mPolygon2.mRightTop.x, mPolygon2.mRightTop.y,mPaint);
                    canvas.drawLine(mPtHalf1.x, mPtHalf1.y,
                            mPolygon2.mRightBottom.x, mPolygon2.mRightBottom.y,mPaint);
                }
            }
        }
    }

    private void polygon2() {
        Vector2d lhsVec = new Vector2d(mPt1, mPt2);
        Vector2d rhsVec = new Vector2d(mPt2, mPt3);
        float lhsSlope = lhsVec.slope();
        float rhsSlope = rhsVec.slope();
        double lhsAngle, rhsAngle;
        if(lhsSlope == Float.MAX_VALUE) {
            lhsAngle = (float)(Math.PI * 0.5f);
        } else {
            lhsAngle = Math.atan(lhsSlope);
            // limit angle to 0~PI
            if (lhsAngle < 0)
                lhsAngle += Math.PI;
        }
        if(rhsSlope == Float.MAX_VALUE) {
            rhsAngle = (float)(Math.PI * 0.5f);
        } else {
            rhsAngle = Math.atan(rhsSlope);
            // limit angle to 0~PI
            if (rhsAngle < 0)
                rhsAngle += Math.PI;
        }
        double intersectAngle, dstAngle;
        double len;

        if (lhsSlope == 0) {
            if(lhsVec.mDx > 0) {
                if(rhsVec.mDy < 0) {
                    intersectAngle = Math.PI - rhsAngle;
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = intersectAngle * 0.5f;
                    mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                    mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                    mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                    mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                } else {
                    intersectAngle = rhsAngle;
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = intersectAngle * 0.5f;
                    mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                    mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                    mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                    mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                }
            }
            else {
                if(rhsVec.mDy < 0) {
                    intersectAngle = rhsAngle;
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = intersectAngle * 0.5f;
                    mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                    mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                    mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                    mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                } else {
                    intersectAngle = Math.PI - rhsAngle;
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = intersectAngle * 0.5f;
                    mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                    mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                    mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                    mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                }
            }
        }
        else if (lhsSlope == Float.MAX_VALUE) {
            if(lhsVec.mDy < 0) {
                if(rhsVec.mDy < 0) {
                    intersectAngle = rhsAngle > Math.PI * 0.5f ?
                            (rhsAngle - Math.PI * 0.5f) : (Math.PI * 0.5f - rhsAngle);
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = rhsAngle > Math.PI * 0.5f ?
                            (intersectAngle * 0.5f) : (intersectAngle * 0.5f + rhsAngle);
                    if(rhsAngle > Math.PI * 0.5f) {
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    }
                }
                else {
                    intersectAngle = rhsAngle > Math.PI * 0.5f ?
                            (Math.PI * 1.5f - rhsAngle) : (Math.PI * 0.5f + rhsAngle);
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = Math.PI * 0.5f - intersectAngle * 0.5f;
                    if(rhsAngle > Math.PI * 0.5f) {
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    }
                }
            }
            else {
                if(rhsVec.mDy < 0) {
                    intersectAngle = rhsAngle > Math.PI * 0.5f ?
                            (rhsAngle - Math.PI * 0.5f) : (Math.PI * 0.5f - rhsAngle);
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = rhsAngle > Math.PI * 0.5f ?
                            (Math.PI - rhsAngle + intersectAngle * 0.5f) : (intersectAngle * 0.5f + rhsAngle);
                    if(rhsAngle > Math.PI * 0.5f) {
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    }
                }
                else {
                    intersectAngle = rhsAngle > Math.PI * 0.5f ?
                            (Math.PI * 1.5f - rhsAngle) : (rhsAngle + Math.PI * 0.5f);
                    len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                    dstAngle = Math.PI * 0.5f - intersectAngle * 0.5f;
                    if(rhsAngle > Math.PI * 0.5f) {
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    }
                }
            }
        }
        else if (lhsSlope > 0) {
            //in this condition, lhs angle ranges from 0 to Pi / 2
            if (lhsVec.mDy < 0) {
                if (lhsAngle > rhsAngle) {
                    if (rhsVec.mDy < 0) {
                        intersectAngle = Math.PI - (lhsAngle - rhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = Math.PI - lhsAngle - intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = lhsAngle - rhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = rhsAngle + intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    }
                }
                else {
                    if (rhsVec.mDy < 0) {
                        intersectAngle = Math.PI - (rhsAngle - lhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = Math.PI - rhsAngle - intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = rhsAngle - lhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = lhsAngle + intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    }
                }
            }
            else {
                if (lhsAngle > rhsAngle) {
                    if (rhsVec.mDy < 0) {
                        intersectAngle = lhsAngle - rhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = rhsAngle + intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = Math.PI - (lhsAngle - rhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = Math.PI - lhsAngle - intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    }
                }
                else {
                    if (rhsVec.mDy < 0) {
                        intersectAngle = rhsAngle - lhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = rhsAngle - intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = Math.PI - (rhsAngle - lhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = intersectAngle * 0.5f - lhsAngle;
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    }
                }
            }
        }
        else {// lhsSlope < 0
            if(lhsVec.mDy < 0) {
                if (lhsAngle > rhsAngle) {
                    if(rhsVec.mDy < 0) {
                        intersectAngle = Math.PI - (lhsAngle - rhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = intersectAngle * 0.5f - (Math.PI - lhsAngle);
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = lhsAngle - rhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = intersectAngle * 0.5f + (Math.PI - lhsAngle);
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    }
                }
                else {
                    if(rhsVec.mDy < 0) {
                        intersectAngle = Math.PI - (rhsAngle - lhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = lhsAngle - intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = rhsAngle - lhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = Math.PI - rhsAngle + intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    }
                }
            }
            else {
                if (lhsAngle > rhsAngle) {
                    if(rhsVec.mDy < 0) {
                        intersectAngle = lhsAngle - rhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = rhsAngle + intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = Math.PI - (lhsAngle - rhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle =  intersectAngle * 0.5f - (Math.PI - lhsAngle);
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y + (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y - (float) (len * Math.sin(dstAngle));
                    }
                }
                else {
                    if(rhsVec.mDy < 0) {
                        intersectAngle = rhsAngle - lhsAngle;
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = Math.PI - rhsAngle + intersectAngle * 0.5f;
                        mPtHalf1.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    } else {
                        intersectAngle = Math.PI - (rhsAngle - lhsAngle);
                        len = mLineWidth * 0.5f / Math.sin(intersectAngle / 2);
                        dstAngle = intersectAngle * 0.5f - (Math.PI - rhsAngle);
                        mPtHalf1.x = mPt2.x + (float) (len * Math.cos(dstAngle));
                        mPtHalf1.y = mPt2.y - (float) (len * Math.sin(dstAngle));

                        mPtHalf2.x = mPt2.x - (float) (len * Math.cos(dstAngle));
                        mPtHalf2.y = mPt2.y + (float) (len * Math.sin(dstAngle));
                    }
                }
            }
        }
    }

    private void polygon3(Polygon2d polygon, PointF start, PointF end) {
        float slope = -(end.y - start.y) / (end.x - start.x);
        float pSlope = -1.f / slope;
        PointF realSt = new PointF();
        PointF realEnd = new PointF();
        if (start.x < end.x) {
            realSt.set(start.x, start.y);
            realEnd.set(end.x, end.y);
        } else {
            realSt.set(end.x, end.y);
            realEnd.set(start.x, start.y);
        }
        if (pSlope < 0) {
            double angle = Math.atan(-pSlope);
            polygon.mLeftTop.x = (float) (realSt.x - mLineWidth / 2.f * Math.cos(angle));
            polygon.mLeftTop.y = (float) (realSt.y - mLineWidth / 2.f * Math.sin(angle));

            polygon.mLeftBottom.x = (float) (realSt.x + mLineWidth / 2.f * Math.cos(angle));
            polygon.mLeftBottom.y = (float) (realSt.y + mLineWidth / 2.f * Math.sin(angle));

            polygon.mRightTop.x = (float) (realEnd.x - mLineWidth / 2.f * Math.cos(angle));
            polygon.mRightTop.y = (float) (realEnd.y - mLineWidth / 2.f * Math.sin(angle));

            polygon.mRightBottom.x = (float) (realEnd.x + mLineWidth / 2.f * Math.cos(angle));
            polygon.mRightBottom.y = (float) (realEnd.y + mLineWidth / 2.f * Math.sin(angle));
        } else {
            double angle = Math.atan(pSlope);

            polygon.mLeftTop.x = (float) (realSt.x + mLineWidth / 2.f * Math.cos(angle));
            polygon.mLeftTop.y = (float) (realSt.y - mLineWidth / 2.f * Math.sin(angle));

            polygon.mLeftBottom.x = (float) (realSt.x - mLineWidth / 2.f * Math.cos(angle));
            polygon.mLeftBottom.y = (float) (realSt.y + mLineWidth / 2.f * Math.sin(angle));

            polygon.mRightTop.x = (float) (realEnd.x + mLineWidth / 2.f * Math.cos(angle));
            polygon.mRightTop.y = (float) (realEnd.y - mLineWidth / 2.f * Math.sin(angle));

            polygon.mRightBottom.x = (float) (realEnd.x - mLineWidth / 2.f * Math.cos(angle));
            polygon.mRightBottom.y = (float) (realEnd.y + mLineWidth / 2.f * Math.sin(angle));
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
            if (pSlope < 0) {
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
