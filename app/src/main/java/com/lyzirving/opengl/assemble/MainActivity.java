package com.lyzirving.opengl.assemble;

import androidx.annotation.IdRes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;

import com.lyzirving.opengl.assemble.ui.LineActivity;
import com.lyzirving.opengl.assemble.utils.LogUtil;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private static final String TAG = "MainActivity";
    private static final int CODE_REQUEST_READ_WRITE_PERMISSION = 0x01;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.btn_line_ac).setOnClickListener(this);
    }

    @Override
    protected void onPostCreate(@Nullable Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        requestUserPermission(CODE_REQUEST_READ_WRITE_PERMISSION);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case CODE_REQUEST_READ_WRITE_PERMISSION: {
                LogUtil.logD(TAG, "onRequestPermissionsResult: read and write");
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED
                        && grantResults[1] == PackageManager.PERMISSION_GRANTED) {
                    LogUtil.logI(TAG, "onRequestPermissionsResult: read write permission granted");
                    enableButtons(R.id.btn_line_ac,true);
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_line_ac: {
                startActivity(LineActivity.class);
                break;
            }
            default: {
                break;
            }
        }
    }

    private void enableButtons(@IdRes int id, boolean enable) {
        findViewById(id).setEnabled(enable);
    }

    private void requestUserPermission(int type) {
        switch (type) {
            case CODE_REQUEST_READ_WRITE_PERMISSION: {
                int permissionState = ActivityCompat.checkSelfPermission(getApplicationContext(),
                        Manifest.permission.WRITE_EXTERNAL_STORAGE);
                if (permissionState != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(this,
                            new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,
                                    Manifest.permission.READ_EXTERNAL_STORAGE},
                            CODE_REQUEST_READ_WRITE_PERMISSION);
                } else {
                    LogUtil.logI(TAG, "requestUserPermission: permission granted");
                    enableButtons(R.id.btn_line_ac,true);
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    private void startActivity(Class<?> activityClass) {
        Intent intent = new Intent();
        intent.setClass(getApplicationContext(), activityClass);
        startActivity(intent);
    }
}
