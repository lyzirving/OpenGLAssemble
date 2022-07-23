package com.lyzirving.opengl.assemble.ui;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.lyzirving.opengl.assemble.utils.LogUtil;

import androidx.annotation.Nullable;

/**
 * @author lyzirving
 */
public class MotionView3d extends View {
    private static final String TAG = "MotionView3d";
    private static final float INVALID_POS = Float.MAX_VALUE;

    private boolean mFirstMove;
    private float mDownX, mDownY;
    private float mLastX, mLastY;
    private float mMoveX, mMoveY;

    private MotionTrackListener mListener;

    public MotionView3d(Context context) {
        this(context, null);
    }

    public MotionView3d(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public MotionView3d(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        reset();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getAction();
        switch (action) {
            case MotionEvent.ACTION_DOWN: {
                mDownX = event.getX();
                mDownY = event.getY();
                break;
            }
            case MotionEvent.ACTION_MOVE: {
                processMove(event);
                if (mListener != null)
                    mListener.onMove(mMoveX, mMoveY,
                                    (mMoveX / (getMeasuredWidth() * 0.5f)),
                                    (mMoveY / (getMeasuredHeight() * 0.5f)));
                break;
            }
            case MotionEvent.ACTION_CANCEL:
            case MotionEvent.ACTION_UP: {
                reset();
                break;
            }
            default: {
                break;
            }
        }
        return true;
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mListener = null;
    }

    public void setMotionListener(MotionTrackListener l) {
        mListener = l;
    }

    private void processMove(MotionEvent event) {
        if (mFirstMove) {
            mLastX = mDownX;
            mLastY = mDownY;
            mFirstMove = false;
        }
        mMoveX += event.getX() - mLastX;
        mMoveY += event.getY() - mLastY;

        float limitX = getMeasuredWidth() * 0.5f;
        float limitY = getMeasuredHeight() * 0.5f;

        if (mMoveX > limitX)
            mMoveX = mMoveX % limitX - limitX;
        else if(mMoveX < -limitX)
            mMoveX = limitX - Math.abs(mMoveX) % limitX;

        if (mMoveY > limitY)
            mMoveY = mMoveY % limitY - limitY;
        else if(mMoveY < -limitY)
            mMoveY = limitY - Math.abs(mMoveY) % limitY;

        mLastX = event.getX();
        mLastY = event.getY();
    }

    private void reset() {
        mFirstMove = true;
        mDownX = INVALID_POS;
        mDownY = INVALID_POS;
        mLastX = INVALID_POS;
        mLastY = INVALID_POS;
    }

    public interface MotionTrackListener {
        void onMove(float xDist, float yDist, float xRatio, float yRatio);
    }
}
