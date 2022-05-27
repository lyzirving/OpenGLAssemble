//
// Created by liuyuzhou on 2022/5/25.
//
#ifndef OPENGLASSEMBLE_VECTORHELPER_H
#define OPENGLASSEMBLE_VECTORHELPER_H

#include <cmath>

class Viewport;

#define CHANNEL_R(_COLOR) (((float)(((_COLOR) & 0xFF000000) >> 24) / (float)255))
#define CHANNEL_G(_COLOR) (((float)(((_COLOR) & 0x00FF0000) >> 16) / (float)255))
#define CHANNEL_B(_COLOR) (((float)(((_COLOR) & 0x0000FF00) >> 8) / (float)255))
#define CHANNEL_A(_COLOR) (((float)(((_COLOR) & 0x000000FF)) / (float)255))

struct Point2d {
public:
    int32_t mX;
    int32_t mY;

    Point2d() : mX(0), mY(0) {}
    Point2d(int32_t x, int32_t y) : mX(x), mY(y) {}

    void set(float x, float y);
};

struct Vector2d {
public:
    Point2d mStart;
    Point2d mEnd;

    Vector2d() : mStart(), mEnd() {}
    Vector2d(int32_t startX, int32_t startY, int32_t endX, int32_t endY) : mStart(startX, startY),
                                                                           mEnd(endX, endY) {}
    Vector2d(const Point2d &startPt, const Point2d &endPt) : mStart(startPt.mX, startPt.mY),
                                                             mEnd(endPt.mX, endPt.mY) {}
    float perpendicularSlope();
    float slope();
};

struct Polygon2d {
public:
    Point2d mLeftTop;
    Point2d mLeftBottom;
    Point2d mRightTop;
    Point2d mRightBottom;

    Polygon2d() : mLeftTop(), mLeftBottom(), mRightTop(), mRightBottom() {}
};

class VectorHelper {
public:

    static float perpendicularSlop2d(const float *vector);
    static void segmentToPolygon(float *polygon, const float *segStartPt, const float *segEndPt, const float lineWidth);
    static void segmentToPolygonOnScreen(Polygon2d *polygon, const Point2d &startPt, const Point2d &endPt,
                                         const uint32_t lineWidth);
    static float slop2d(const float *vector);
    static void vertex2d(float *vertex, float x, float y, const Viewport &viewport);
    static void vertex2d(float *vertex, const Point2d &pt, const Viewport &viewport);
    static void vector2d(/*out*/float *vector, const float *startPt, const float *endPt);
private:
    VectorHelper();
    ~VectorHelper();
};

#endif //OPENGLASSEMBLE_VECTORHELPER_H
