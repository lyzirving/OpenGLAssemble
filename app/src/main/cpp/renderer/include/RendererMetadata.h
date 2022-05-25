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
            "precision highp float;\n"
            "attribute vec2 aVertexCoords;\n"
            "uniform mat4 uMatrix;\n"
            "varying vec2 vVertex;\n"
            "void main() {\n"
            "   vec4 vertex = uMatrix * vec4(aVertexCoords, 0.0, 1.0);\n"
            "   vVertex = vec2(vertex.x, vertex.y);\n"
            "   gl_Position = vertex;\n"
            "}\n";

    static const char ANTI_ALIS_LINE_FRAGMENT_SHADER[] =
            "precision highp float;\n"
            "uniform vec4 uColor;\n"
            "uniform vec2 uLines[2];\n"
            "uniform vec2 uPortSize;\n"
            "uniform float uThreshold;\n"
            "uniform float uHalfLineWidth;\n"
            "varying vec2 vVertex;\n"
            "\n"
            "float getDist(vec2 pt, vec2 start, vec2 end) {\n"
            "   vec2 vector1 = vec2(pt.x - start.x, pt.y - start.y);\n"
            "   vec2 vector2 = vec2(end.x - start.x, end.y - start.y);\n"
            "   vec2 nVec1 = normalize(vector1);\n"
            "   vec2 nVec2 = normalize(vector2);\n"
            "   float valCos = dot(nVec1, nVec2);\n"
            "   float angle = acos(valCos);\n"
            "   float result = sin(angle) * length(vector1);\n"
            "   return result;\n"
            "}\n"
            "\n"
            "void main() {\n"
            "   float ratio = uPortSize.y / uPortSize.x;\n"
            "   vec2 pos = vec2((vVertex.x + 1.0) / 2.0, (vVertex.y + 1.0) / 2.0);\n"
            "   pos.y = pos.y * ratio;\n"
            "\n"
            "   vec2 start = vec2((uLines[0].x + 1.0) / 2.0, (uLines[0].y + 1.0) / 2.0);\n"
            "   start.y = start.y * ratio;\n"
            "   vec2 end = vec2((uLines[1].x + 1.0) / 2.0, (uLines[1].y + 1.0) / 2.0);\n"
            "   end.y = end.y * ratio;\n"
            "\n"
            "   float dist = getDist(pos, start, end);\n"
            "   if(dist <= uThreshold) {\n"
            "       gl_FragColor = uColor;\n"
            "   } else {\n"
            "       float alpha = 1.0 - (dist - uThreshold) / (uHalfLineWidth - uThreshold);\n"
            "       gl_FragColor = vec4(uColor.r, uColor.g, uColor.b, alpha);\n"
            "   }\n"
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
