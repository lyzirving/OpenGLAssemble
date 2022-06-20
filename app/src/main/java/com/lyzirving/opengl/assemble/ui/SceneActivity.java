package com.lyzirving.opengl.assemble.ui;

import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.lyzirving.opengl.assemble.R;
import com.lyzirving.opengl.assemble.renderer.RendererConstant;
import com.lyzirving.opengl.assemble.renderer.RendererContext;
import com.lyzirving.opengl.assemble.renderer.Scene3d;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

/**
 * @author lyzirving
 */
public class SceneActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private Scene3d mScene;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scene);
        mScene = (Scene3d) RendererContext.createContext(getApplicationContext(),
                Scene3d.class.getName(), "Scene3d");

        SurfaceView surfaceView = findViewById(R.id.view_surface);
        surfaceView.getHolder().addCallback(this);
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
}
