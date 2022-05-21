package com.lyzirving.opengl.assemble;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

import com.lyzirving.opengl.assemble.renderer.RendererContext;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private RendererContext mRenderer;
    private boolean mSendingMessage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mRenderer = RendererContext.createContext("test");
        findViewById(R.id.btn_start).setOnClickListener(this);
        findViewById(R.id.btn_stop).setOnClickListener(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mRenderer != null)
            RendererContext.releaseContext(mRenderer);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_start: {
                mSendingMessage = true;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        while(mSendingMessage) {
                            mRenderer.sendMessage(1);
                            int sleepMill = (int) Math.round(Math.random() * 1000);
                            try {
                                Thread.sleep(sleepMill);
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }
                }).start();
                break;
            }
            case R.id.btn_stop: {
                mSendingMessage = false;
                break;
            }
            default: {
                break;
            }
        }
    }
}
