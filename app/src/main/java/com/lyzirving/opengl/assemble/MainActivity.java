package com.lyzirving.opengl.assemble;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

/**
 * @author lyzirving
 */
public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("lib-assembler");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
}
