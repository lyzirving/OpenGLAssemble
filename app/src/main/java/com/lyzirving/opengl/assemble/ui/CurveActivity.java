package com.lyzirving.opengl.assemble.ui;

import android.os.Bundle;
import android.os.PersistableBundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.lyzirving.opengl.assemble.R;
import com.lyzirving.opengl.assemble.renderer.CurveRendererContext;
import com.lyzirving.opengl.assemble.renderer.RendererConstant;
import com.lyzirving.opengl.assemble.renderer.RendererContext;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

/**
 * @author lyzirving
 */
public class CurveActivity extends AppCompatActivity implements SurfaceHolder.Callback {
    private CurveRendererContext mRenderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_curve);
        mRenderer = (CurveRendererContext) RendererContext.createContext(getApplicationContext(),
                CurveRendererContext.class.getName(), "Curve");

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
    public void surfaceCreated(@NonNull SurfaceHolder holder) {}

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        mRenderer.registerWindow("Curve", holder.getSurface());
        mRenderer.sendMessage(RendererConstant.MSG_DRAW, 0, 0);
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        mRenderer.removeWindow("Curve");
    }
}
