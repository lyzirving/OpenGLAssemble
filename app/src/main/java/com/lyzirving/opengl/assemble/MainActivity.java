package com.lyzirving.opengl.assemble;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.lyzirving.opengl.assemble.ui.CurveActivity;
import com.lyzirving.opengl.assemble.ui.LineActivity;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.btn_line_ac).setOnClickListener(this);
        findViewById(R.id.btn_curve_ac).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_line_ac: {
                startActivity(LineActivity.class);
                break;
            }
            case R.id.btn_curve_ac: {
                startActivity(CurveActivity.class);
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
