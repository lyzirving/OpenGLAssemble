package com.lyzirving.opengl.assemble.ui;

import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.ImageView;

import com.lyzirving.opengl.assemble.R;
import com.lyzirving.opengl.assemble.renderer.RendererConstant;
import com.lyzirving.opengl.assemble.renderer.RendererContext;
import com.lyzirving.opengl.assemble.renderer.Scene3d;
import com.lyzirving.opengl.assemble.utils.LogUtil;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

/**
 * @author lyzirving
 */
public class SceneActivity extends AppCompatActivity implements SurfaceHolder.Callback,
                                                                MotionView3d.MotionTrackListener,
                                                                View.OnClickListener {
    private static final String TAG = "SceneActivity";

    private Scene3d mScene;
    private MotionView3d mModelView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scene);
        mScene = (Scene3d) RendererContext.createContext(getApplicationContext(),
                Scene3d.class.getName(), "Scene3d");

        SurfaceView surfaceView = findViewById(R.id.view_surface);
        mModelView = findViewById(R.id.view_3d);
        ImageView modelIcon = findViewById(R.id.iv_model_icon);
        ImageView cameraIcon = findViewById(R.id.iv_camera_icon);

        mModelView.setMotionListener(this);
        surfaceView.getHolder().addCallback(this);
        modelIcon.setOnClickListener(this);
        cameraIcon.setOnClickListener(this);

        modelIcon.setImageResource(R.drawable.model_selected);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.iv_model_icon:
            {
                if (mModelView.getMode() != MotionView3d.OpMode.MODE_MODEL) {
                    mModelView.setMode(MotionView3d.OpMode.MODE_MODEL);
                    cancelAllSelect();
                    ImageView view = (ImageView)(findViewById(R.id.iv_model_icon));
                    view.setImageResource(R.drawable.model_selected);
                }
                break;
            }
            case R.id.iv_camera_icon:
            {
                if (mModelView.getMode() != MotionView3d.OpMode.MODEL_CAMERA) {
                    mModelView.setMode(MotionView3d.OpMode.MODEL_CAMERA);
                    cancelAllSelect();
                    ImageView view = (ImageView)(findViewById(R.id.iv_camera_icon));
                    view.setImageResource(R.drawable.camera_selected);
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mScene != null)
            RendererContext.releaseContext(mScene);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {}

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        Surface surface = holder.getSurface();
        mScene.registerWindow("Scene3d", surface);
        mScene.sendMessage(RendererConstant.MSG_DRAW, 0, 0);
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        mScene.removeWindow("Scene3d");
    }

    @Override
    public void onModelMove(float xDist, float yDist, float xRatio, float yRatio) {
        int angle = (int) (xRatio * 180);
        if (mScene != null)
            mScene.rotateModel(angle);
    }

    @Override
    public void onCameraMove(float xDist, float yDist, float xRatio, float yRatio) {
        float zDist = -yRatio;
        int angle = (int) (zDist / Math.abs(zDist)) * (int) (Math.toDegrees(Math.atan(Math.abs(zDist))));
        LogUtil.logI(TAG, "camera: ratio = " + yRatio + ", dist = " + zDist + ", angle = " + angle);
        if (mScene != null)
            mScene.liftUpVision(zDist, angle);
    }

    private void cancelAllSelect() {
        ImageView view = (ImageView)(findViewById(R.id.iv_camera_icon));
        view.setImageResource(R.drawable.camera_normal);

        view = (ImageView)(findViewById(R.id.iv_model_icon));
        view.setImageResource(R.drawable.model_normal);
    }
}
