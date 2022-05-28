package com.lyzirving.opengl.assemble;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.lyzirving.opengl.assemble.renderer.RendererConstant;
import com.lyzirving.opengl.assemble.renderer.RendererContext;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener, SurfaceHolder.Callback {

    private RendererContext mRenderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mRenderer = RendererContext.createContext(getApplicationContext(), "test");

        SurfaceView surfaceView = findViewById(R.id.view_surface);
        surfaceView.getHolder().addCallback(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mRenderer != null)
            RendererContext.releaseContext(mRenderer);
    }

    @Override
    public void onClick(View v) { }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {}

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        Surface surface = holder.getSurface();
        mRenderer.registerWindow("test", surface);
        mRenderer.sendMessage(RendererConstant.MSG_DRAW, 0, 0);
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        mRenderer.removeWindow("test");
    }
}
