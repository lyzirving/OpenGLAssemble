package com.lyzirving.opengl.assemble;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
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
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_line_ac: {
                Intent i = new Intent(getApplicationContext(), LineActivity.class);
                startActivity(i);
                break;
            }
            default: {
                break;
            }
        }
    }
}
