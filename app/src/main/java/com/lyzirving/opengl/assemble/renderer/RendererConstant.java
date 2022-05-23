package com.lyzirving.opengl.assemble.renderer;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

import androidx.annotation.IntDef;

/**
 * @author lyzirving
 */
public class RendererConstant {
    /**
     * notice message id should be the same with the native message
     */
    public static final int MSG_DRAW = 0x03;

    @IntDef({MSG_DRAW})
    @Retention(RetentionPolicy.SOURCE)
    public @interface Message {}
}
