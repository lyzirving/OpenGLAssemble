package com.lyzirving.opengl.assemble.ui;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import androidx.annotation.Nullable;

/**
 * @author lyzirving
 */
public class MotionView3d extends View {
    private static final String TAG = "MotionView3d";
    private static final float INVALID_POS = Float.MAX_VALUE;

    public enum OpMode {
        /**
         * mode to control model
         */
        MODE_MODEL,
        /**
         * mode to control camera
         */
        MODEL_CAMERA
    }

    private OpMode mMode;
    private boolean mFirstMove;
    private float mDownX, mDownY;
    private float mLastX, mLastY;

    private float mModelMoveX, mModelMoveY;
    private float mCameraMoveX, mCameraMoveY;

    private MotionTrackListener mListener;

    public MotionView3d(Context context) {
        this(context, null);
    }

    public MotionView3d(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public MotionView3d(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mMode = OpMode.MODE_MODEL;
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
                processMoveNotify();
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

    public OpMode getMode() {
        return mMode;
    }

    public void setMode(OpMode mode) {
        mMode = mode;
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

        float limitX = getMeasuredWidth() * 0.5f;
        float limitY = getMeasuredHeight() * 0.5f;

        float deltaX = event.getX() - mLastX;
        float deltaY = event.getY() - mLastY;

        switch (mMode) {
            case MODE_MODEL:
            {
                mModelMoveX += deltaX;
                mModelMoveY += deltaY;

                if (mModelMoveX > limitX)
                    mModelMoveX = mModelMoveX % limitX - limitX;
                else if(mModelMoveX < -limitX)
                    mModelMoveX = limitX - Math.abs(mModelMoveX) % limitX;

                if (mModelMoveY > limitY)
                    mModelMoveY = mModelMoveY % limitY - limitY;
                else if(mModelMoveY < -limitY)
                    mModelMoveY = limitY - Math.abs(mModelMoveY) % limitY;

                break;
            }
            case MODEL_CAMERA:
            {
                mCameraMoveX += deltaX;
                mCameraMoveY += deltaY;

                if (mCameraMoveX > limitX)
                    mCameraMoveX = mCameraMoveX % limitX - limitX;
                else if(mCameraMoveX < -limitX)
                    mCameraMoveX = limitX - Math.abs(mCameraMoveX) % limitX;

                if (mCameraMoveY > limitY)
                    mCameraMoveY = mCameraMoveY % limitY - limitY;
                else if(mCameraMoveY < -limitY)
                    mCameraMoveY = limitY - Math.abs(mCameraMoveY) % limitY;

                break;
            }
            default:
            {
                break;
            }
        }

        mLastX = event.getX();
        mLastY = event.getY();
    }

    private void processMoveNotify() {
        if (mListener != null) {
            float limitX = getMeasuredWidth() * 0.5f;
            float limitY = getMeasuredHeight() * 0.5f;
            switch (mMode) {
                case MODE_MODEL:
                {
                    mListener.onModelMove(mModelMoveX, mModelMoveY,
                            mModelMoveX / limitX,
                            mModelMoveY / limitY);
                    break;
                }
                case MODEL_CAMERA:
                {
                    mListener.onCameraMove(mCameraMoveX, mCameraMoveY,
                            mCameraMoveX / limitX,
                            mCameraMoveY / limitY);
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    private void reset() {
        mFirstMove = true;
        mDownX = INVALID_POS;
        mDownY = INVALID_POS;
        mLastX = INVALID_POS;
        mLastY = INVALID_POS;
    }

    public interface MotionTrackListener {
        void onModelMove(float xDist, float yDist, float xRatio, float yRatio);
        void onCameraMove(float xDist, float yDist, float xRatio, float yRatio);
    }
}
