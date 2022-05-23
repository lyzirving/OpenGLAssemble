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
};

#endif //OPENGLASSEMBLE_RENDERERMETADATA_H
