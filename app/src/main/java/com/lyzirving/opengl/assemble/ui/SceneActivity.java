package com.lyzirving.opengl.assemble.ui;

import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.SeekBar;

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
                                                                SeekBar.OnSeekBarChangeListener {
    private static final String TAG = "SceneActivity";
    private Scene3d mScene;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scene);
        mScene = (Scene3d) RendererContext.createContext(getApplicationContext(),
                Scene3d.class.getName(), "Scene3d");

        SurfaceView surfaceView = findViewById(R.id.view_surface);
        SeekBar seekBarRotate = findViewById(R.id.seek_rotate_model);

        surfaceView.getHolder().addCallback(this);
        seekBarRotate.setOnSeekBarChangeListener(this);
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
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        switch (seekBar.getId())
        {
            case R.id.seek_rotate_model:
            {
                int angle = (int) (((progress - 50) * 1f / 50) * 180);
                if (mScene != null)
                    mScene.rotateModel(angle);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {}

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {}
}
