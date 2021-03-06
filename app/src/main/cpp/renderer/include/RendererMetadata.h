//
// Created by liuyuzhou on 2022/5/23.
//
#ifndef OPENGLASSEMBLE_RENDERERMETADATA_H
#define OPENGLASSEMBLE_RENDERERMETADATA_H
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
    static const float TWO_DIMEN_TEXTURE_COORD_STRIP[] = {
            0, 0,
            0, 1,
            1, 0,
            1, 1,
    };
    static const float TWO_DIMEN_TEXTURE_COORD_NORMAL[] = {
            0, 0,
            0, 1,
            1, 0,
            1, 0,
            0, 1,
            1, 1,
    };
}

namespace renderer
{
    static const char TWO_DIMEN_RENDERER[] = "2d renderer";
    static const char GRAPHIC_RENDERER[] = "graphic renderer";
    static const char ANTI_ALIAS_RENDERER[] = "antialias renderer";
    static const char CONTINUOUS_LINE_RENDERER[] = "continuous line renderer";
    static const char CURVE_RENDERER[] = "curve renderer";
    static const char DOT_RENDERER[] = "dot renderer";
};

#endif //OPENGLASSEMBLE_RENDERERMETADATA_H
