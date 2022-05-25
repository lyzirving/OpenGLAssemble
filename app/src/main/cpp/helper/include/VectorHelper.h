//
// Created by liuyuzhou on 2022/5/25.
//
#ifndef OPENGLASSEMBLE_VECTORHELPER_H
#define OPENGLASSEMBLE_VECTORHELPER_H

class Viewport;

#define CHANNEL_R(_COLOR) (((float)(((_COLOR) & 0xFF000000) >> 24) / (float)255))
#define CHANNEL_G(_COLOR) (((float)(((_COLOR) & 0x00FF0000) >> 16) / (float)255))
#define CHANNEL_B(_COLOR) (((float)(((_COLOR) & 0x0000FF00) >> 8) / (float)255))
#define CHANNEL_A(_COLOR) (((float)(((_COLOR) & 0x000000FF)) / (float)255))

class VectorHelper {
public:

    static float perpendicularSlop2d(const float *vector);
    static void segmentToPolygon(float *polygon, const float *segStartPt, const float *segEndPt, const float lineWidth);
    static float slop2d(const float *vector);
    static void vertex2d(float *vertex, float x, float y, const Viewport &viewport);
    static void vector2d(/*out*/float *vector, const float *startPt, const float *endPt);
private:
    VectorHelper();
    ~VectorHelper();
};

#endif //OPENGLASSEMBLE_VECTORHELPER_H
