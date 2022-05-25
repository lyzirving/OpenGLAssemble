//
// Created by liuyuzhou on 2022/5/25.
//
#ifndef OPENGLASSEMBLE_VECTORHELPER_H
#define OPENGLASSEMBLE_VECTORHELPER_H

class Viewport;

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
