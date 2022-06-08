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

/**
 * Vector2d represents vector on screen coordinate whose origin is (0, 0),
 * x axis is from left to right, y axis is from up to down
 */
struct Vector2d {
public:
    Point2d mStart, mEnd;
    float mDx, mDy;

    Vector2d() : mStart(), mEnd(), mDx(0.f), mDy(0.f) {}
    Vector2d(int32_t startX, int32_t startY, int32_t endX, int32_t endY) : mStart(startX, startY),
                                                                           mEnd(endX, endY),
                                                                           mDx(endX - startX),
                                                                           mDy(endY - startY) {}
    Vector2d(const Point2d &startPt, const Point2d &endPt) : mStart(startPt.mX, startPt.mY),
                                                             mEnd(endPt.mX, endPt.mY),
                                                             mDx(endPt.mX - startPt.mX),
                                                             mDy(endPt.mY - startPt.mY) {}
    /**
     * calculate the azimuth of this vector.
     * @return azimuth of vector, the result is limited from 0 to Pi.
     */
    double azimuth();
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
    float width();
    float height();
};

class VectorHelper {
public:

    /**
     * create four points to transfer a two-point line to a rectangle which is formed by two triangle
     * notice these points are all measured in screen coordinate system whose origin is the left-top point
     *
     * @param polygon[out] pointer for the result rectangle
     * @param startPt[in]  start point for the line
     * @param endPt[in]    end point for the line
     * @param lineWidth    line width
     */
    static void segmentToPolygon(Polygon2d *polygon, const Point2d &startPt, const Point2d &endPt,
                                 const uint32_t lineWidth);
    static void segToContinuousPolygon(Polygon2d *polygon, const Point2d *ptOnScreen, uint32_t start,
            uint32_t count, const uint32_t lineWidth);
    static void vertex2d(float *vertex, float x, float y, const Viewport &viewport);
    static void vertex2d(float *vertex, const Point2d &pt, const Viewport &viewport);
private:
    VectorHelper();
    ~VectorHelper();

    static void fillTurningPtPolygon_v1(Polygon2d *result, bool setForLeft,
                                        const Point2d &centerLineStart, const Point2d &centerLineEnd,
                                        const Polygon2d &polygon,
                                        const Point2d &innerPt, const Point2d &outerPt);
    static void fillTurningPtPolygon_v2(Polygon2d *result, const Point2d &preStart,
                                        const Point2d &start, const Point2d &end,
                                        const Point2d &startInnerPt, const Point2d &startOuterPt,
                                        const Point2d &endInnerPt, const Point2d &endOuterPt);
    static void lineTurningPt(Point2d *pt1, Point2d *pt2,
                              const Point2d &startPt, const Point2d &midPt,
                              const Point2d &endPt,
                              const uint32_t lineWidth);

    /**
     * judge whether pt1 and pt2 are on the same side of the line formed by lineStart and lineEnd.
     * the main algorithm is the result of vector's cross value.
     * @param pt1
     * @param pt2
     * @param lineStart
     * @param lineEnd
     * @return true if the two pts are on the same side; false otherwise.
     */
    static bool ptsSameSide(const Point2d &pt1, const Point2d &pt2, const Point2d &lineStart, const Point2d &lineEnd);
};

#endif //OPENGLASSEMBLE_VECTORHELPER_H
