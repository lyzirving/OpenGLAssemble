//
// Created by liuyuzhou on 2022/5/23.
//
#ifndef OPENGLASSEMBLE_RENDERERMETADATA_H
#define OPENGLASSEMBLE_RENDERERMETADATA_H

namespace shader {
    static const char TWO_DIMEN_VERTEX_SHADER[] =
            "attribute vec2 aVertexCoords;\n"
            "attribute vec2 aTexCoords;\n"
            "uniform mat4 uMatrix;\n"
            "varying vec2 vTexCoords;\n"
            "void main() {\n"
            "  vTexCoords = vec2(aTexCoords.x, aTexCoords.y);\n"
            "  gl_Position = uMatrix * vec4(aVertexCoords, 0.0, 1.0);\n"
            "}\n";

    static const char TWO_DIMEN_FRAGMENT_SHADER[] =
            "precision mediump float;\n"
            "uniform sampler2D uTwoDimensTextureSampler;\n"
            "varying vec2 vTexCoords;\n"
            "void main() {\n"
            "    vec4 color = texture2D(uTwoDimensTextureSampler, vTexCoords);\n"
            "    gl_FragColor = color;\n"
            "}\n";

    static const char GRAPHIC_VERTEX_SHADER[] =
            "#version 320 es\n"
            "precision mediump float;\n"
            "in vec2 aVertexCoords;\n"
            "uniform int uVertexCount;\n"
            "uniform mat4 uMatrix;\n"
            "uniform vec4 uGradient[2];\n"
            "out vec4 vColor;\n"
            "void main() {\n"
            "  float rRange = uGradient[1].r - uGradient[0].r;"
            "  float gRange = uGradient[1].g - uGradient[0].g;"
            "  float bRange = uGradient[1].b - uGradient[0].b;"
            "  float aRange = uGradient[1].a - uGradient[0].a;"
            "  float ratio = float(gl_VertexID) / float(uVertexCount - 1);"
            "  vColor = vec4(uGradient[0].r + ratio * rRange, uGradient[0].g + ratio * gRange, uGradient[0].b + ratio * bRange, uGradient[0].a + ratio * aRange);"
            "  gl_Position = uMatrix * vec4(aVertexCoords, 0.0, 1.0);\n"
            "}\n";

    static const char GRAPHIC_FRAGMENT_SHADER[] =
            "#version 320 es\n"
            "precision mediump float;\n"
            "in vec4 vColor;\n"
            "out vec4 mFragColor;\n"
            "void main() {\n"
            "    mFragColor = vColor;\n"
            "}\n";

    static const char ANTI_ALIS_LINE_VERTEX_SHADER[] =
            "precision mediump float;\n"
            "attribute vec2 aVertexCoords;\n"
            "uniform float uThreshold;\n"
            "uniform vec2 uSegment[2];\n"
            "uniform mat4 uMatrix;\n"
            "void main() {\n"
            "  gl_Position = uMatrix * vec4(aVertexCoords, 0.0, 1.0);\n"
            "}\n";

    static const char ANTI_ALIS_LINE_FRAGMENT_SHADER[] =
            "precision mediump float;\n"
            "uniform vec4 uColor;\n"
            "void main() {\n"
            "    gl_FragColor = uColor;\n"
            "}\n";

    static const char POINT_VERTEX_SHADER[] =
            "precision mediump float;\n"
            "attribute vec2 aVertexCoords;\n"
            "void main() {\n"
            "  gl_Position = vec4(aVertexCoords, 0.0, 1.0);\n"
            "  gl_PointSize = 10.0;\n"
            "}\n";

    static const char POINT_FRAGMENT_SHADER[] =
            "precision mediump float;\n"
            "void main() {\n"
            "    gl_FragColor = vec4(0.7, 0.0, 0.3, 1.0);\n"
            "}\n";

}

namespace vertex {
    /**
     *  @brief the vertex is organized in a rule as bellow
     *         (-1,1)--------(1,1)
     *            |          / |
     *            |         /  |
     *            |        /   |
     *            |    0,0)    |
     *            |     /      |
     *            |    /       |
     *            |   /        |
     *         (-1,-1)-------(1,-1)
     *  @author LiuYuzhou
     *  @date 2022-02-28
     */
    static const int TWO_DIMENS_VERTEX_COUNT = 4;
    static const int TWO_DIMENS_VERTEX_COMPONENT = 2;

    static const float TWO_DIMENS_VERTEX_ARRAY[] = {
            // left - top
            -1, 1,
            // left - bottom
            -1, -1,
            // right - top
            1, 1,
            // right - bottom
            1, -1,
    };
}

namespace texturecoord {
    /**
     *  @brief the texture coordinate is organized in a rule as bellow
     *          (0,0)--------(1,0)
     *            |          / |
     *            |         /  |
     *            |        /   |
     *            |  (0.5,0.5) |
     *            |     /      |
     *            |    /       |
     *            |   /        |
     *          (0,1)--------(1,1)
     *  @author LiuYuzhou
     *  @date 2022-02-28
     */
    static const int TWO_DIMENS_TEXTURE_COORD_COUNT = 4;
    static const int TWO_DIMENS_TEXTURE_COORD_COMPONENT = 2;

    static const float TWO_DIMEN_TEXTURE_COORD[] = {
            0, 0,
            0, 1,
            1, 0,
            1, 1,
    };
}

namespace renderer
{
    static const char TWO_DIMEN_RENDERER[] = "2d renderer";
    static const char GRAPHIC_RENDERER[] = "graphic renderer";
    static const char ANTI_ALIAS_RENDERER[] = "antialias renderer";
};

#endif //OPENGLASSEMBLE_RENDERERMETADATA_H
