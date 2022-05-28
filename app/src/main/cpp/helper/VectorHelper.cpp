//
// Created by liuyuzhou on 2022/5/25.
//
#include "VectorHelper.h"
#include "BaseRendererProgram.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VectorHelper"

void Point2d::set(float x, float y) {
    mX = x;
    mY = y;
}

float Vector2d::perpendicularSlope() {
    int32_t xDiff = mEnd.mX - mStart.mX;
    int32_t yDiff = mEnd.mY - mStart.mY;
    if(xDiff == 0)
        return 0.f;
    else if(yDiff == 0)
        return MAXFLOAT;
    else
        return -1.f / slope();
}

float Vector2d::slope() {
    int32_t xDiff = mEnd.mX - mStart.mX;
    int32_t yDiff = mEnd.mY - mStart.mY;
    if(xDiff == 0)
        return MAXFLOAT;
    else if(yDiff == 0)
        return 0;
    else
        return -float(yDiff) / float(xDiff);
}

VectorHelper::VectorHelper() = default;

VectorHelper::~VectorHelper() = default;

float VectorHelper::perpendicularSlop2d(const float *vector) {
    if (vector[0] == 0) {//input vector is parallel to y axis
        return 0;
    } else if (vector[1] == 0) {//input vector is parallel to y axis
        return MAXFLOAT;
    } else {
        return -1.f * vector[0] / vector[1];
    }
}

float VectorHelper::slop2d(const float *vector) {
    if (vector[0] == 0.f)//vector is parallel to y axis
        return MAXFLOAT;
    else if (vector[1] == 0.f)//vector is parallel to x axis
        return 0;
    else
        return vector[1] / vector[0];
}

void VectorHelper::segmentToPolygon(float *polygon, const float *segStartPt, const float *segEndPt,
                                    const float lineWidth) {
    float lineVector[2];
    vector2d(lineVector, segStartPt, segEndPt);
    float perpendicularSlope = perpendicularSlop2d(lineVector);
    float start[2];
    float end[2];

    if (perpendicularSlope == 0) {// normal is parallel to x axis
        if (segStartPt[1] < segEndPt[1]) {
            start[0] = segStartPt[0];
            start[1] = segStartPt[1];
            end[0] = segEndPt[0];
            end[1] = segEndPt[1];
        } else {
            start[0] = segEndPt[0];
            start[1] = segEndPt[1];
            end[0] = segStartPt[0];
            end[1] = segStartPt[1];
        }
        polygon[0] = start[0] - lineWidth / 2.f;
        polygon[1] = start[1];

        polygon[2] = start[0] + lineWidth / 2.f;
        polygon[3] = start[1];

        polygon[4] = end[0] - lineWidth / 2.f;
        polygon[5] = end[1];

        polygon[6] = end[0] + lineWidth / 2.f;
        polygon[7] = end[1];
    } else if (perpendicularSlope == MAXFLOAT) {// normal is parallel to y axis
        if (segStartPt[0] < segEndPt[0]) {
            start[0] = segStartPt[0];
            start[1] = segStartPt[1];
            end[0] = segEndPt[0];
            end[1] = segEndPt[1];
        } else {
            start[0] = segEndPt[0];
            start[1] = segEndPt[1];
            end[0] = segStartPt[0];
            end[1] = segStartPt[1];
        }
        polygon[0] = start[0];
        polygon[1] = start[1] + lineWidth / 2.f;

        polygon[2] = start[0];
        polygon[3] = start[1] - lineWidth / 2.f;

        polygon[4] = end[0];
        polygon[5] = end[1] + lineWidth / 2.f;

        polygon[6] = end[0];
        polygon[7] = end[1] - lineWidth / 2.f;
    } else if (perpendicularSlope < 0) {
        if (segStartPt[0] < segEndPt[0]) {
            start[0] = segStartPt[0];
            start[1] = segStartPt[1];
            end[0] = segEndPt[0];
            end[1] = segEndPt[1];
        } else {
            start[0] = segEndPt[0];
            start[1] = segEndPt[1];
            end[0] = segStartPt[0];
            end[1] = segStartPt[1];
        }
        float angle = std::atan(-perpendicularSlope);
        polygon[0] = start[0] - lineWidth / 2.f * std::cos(angle);
        polygon[1] = start[1] + lineWidth / 2.f * std::sin(angle);

        polygon[2] = start[0] + lineWidth / 2.f * std::cos(angle);
        polygon[3] = start[1] - lineWidth / 2.f * std::sin(angle);

        polygon[4] = end[0] - lineWidth / 2.f * std::cos(angle);
        polygon[5] = end[1] + lineWidth / 2.f * std::sin(angle);

        polygon[6] = end[0] + lineWidth / 2.f * std::cos(angle);
        polygon[7] = end[1] - lineWidth / 2.f * std::sin(angle);
    } else {
        if (segStartPt[0] < segEndPt[0]) {
            start[0] = segStartPt[0];
            start[1] = segStartPt[1];
            end[0] = segEndPt[0];
            end[1] = segEndPt[1];
        } else {
            start[0] = segEndPt[0];
            start[1] = segEndPt[1];
            end[0] = segStartPt[0];
            end[1] = segStartPt[1];
        }
        float angle = std::atan(perpendicularSlope);
        polygon[0] = start[0] + lineWidth / 2.f * std::cos(angle);
        polygon[1] = start[1] + lineWidth / 2.f * std::sin(angle);

        polygon[2] = start[0] - lineWidth / 2.f * std::cos(angle);
        polygon[3] = start[1] - lineWidth / 2.f * std::sin(angle);

        polygon[4] = end[0] + lineWidth / 2.f * std::cos(angle);
        polygon[5] = end[1] + lineWidth / 2.f * std::sin(angle);

        polygon[6] = end[0] - lineWidth / 2.f * std::cos(angle);
        polygon[7] = end[1] - lineWidth / 2.f * std::sin(angle);
    }
}

void VectorHelper::segmentToPolygonOnScreen(Polygon2d *polygon, const Point2d &startPt,
                                            const Point2d &endPt,
                                            const uint32_t lineWidth) {
    Vector2d vector(startPt, endPt);
    float slope = vector.slope();
    float pSlope = vector.perpendicularSlope();

    Point2d realStart{};
    Point2d realEnd{};

    if (slope == 0.f) {//vector is parallel to x axis
        if (startPt.mX < endPt.mX) {
            realStart.set(startPt.mX, startPt.mY);
            realEnd.set(endPt.mX, endPt.mY);
        } else {
            realStart.set(endPt.mX, endPt.mY);
            realEnd.set(startPt.mX, startPt.mY);
        }
        polygon->mLeftTop.mX = realStart.mX;
        polygon->mLeftTop.mY = realStart.mY - lineWidth / 2;

        polygon->mLeftBottom.mX = realStart.mX;
        polygon->mLeftBottom.mY = realStart.mY + lineWidth / 2;

        polygon->mRightTop.mX = realEnd.mX;
        polygon->mRightTop.mY = realEnd.mY - lineWidth / 2;

        polygon->mRightBottom.mX = realEnd.mX;
        polygon->mRightBottom.mY = realEnd.mY + lineWidth / 2;
    } else if (slope == MAXFLOAT) {//vector is parallel to y axis
        if (startPt.mY < endPt.mY) {
            realStart.set(startPt.mX, startPt.mY);
            realEnd.set(endPt.mX, endPt.mY);
        } else {
            realStart.set(endPt.mX, endPt.mY);
            realEnd.set(startPt.mX, startPt.mY);
        }
        polygon->mLeftTop.mX = realStart.mX - lineWidth / 2;
        polygon->mLeftTop.mY = realStart.mY;

        polygon->mLeftBottom.mX = realStart.mX + lineWidth / 2;
        polygon->mLeftBottom.mY = realStart.mY;

        polygon->mRightTop.mX = realEnd.mX - lineWidth / 2;
        polygon->mRightTop.mY = realEnd.mY;

        polygon->mRightBottom.mX = realEnd.mX + lineWidth / 2;
        polygon->mRightBottom.mY = realEnd.mY;
    } else {
        if (startPt.mX < endPt.mX) {
            realStart.set(startPt.mX, startPt.mY);
            realEnd.set(endPt.mX, endPt.mY);
        } else {
            realStart.set(endPt.mX, endPt.mY);
            realEnd.set(startPt.mX, startPt.mY);
        }
        if(pSlope < 0) {
            float angle = std::atan(-pSlope);
            polygon->mLeftTop.mX = realStart.mX - lineWidth / 2.f * std::cos(angle);
            polygon->mLeftTop.mY = realStart.mY - lineWidth / 2.f * std::sin(angle);

            polygon->mLeftBottom.mX = realStart.mX + lineWidth / 2.f * std::cos(angle);
            polygon->mLeftBottom.mY = realStart.mY + lineWidth / 2.f * std::sin(angle);

            polygon->mRightTop.mX = realEnd.mX - lineWidth / 2.f * std::cos(angle);
            polygon->mRightTop.mY = realEnd.mY - lineWidth / 2.f * std::sin(angle);

            polygon->mRightBottom.mX = realEnd.mX + lineWidth / 2.f * std::cos(angle);
            polygon->mRightBottom.mY = realEnd.mY + lineWidth / 2.f * std::sin(angle);
        } else {
            float angle = std::atan(pSlope);
            polygon->mLeftTop.mX = realStart.mX + lineWidth / 2.f * std::cos(angle);
            polygon->mLeftTop.mY = realStart.mY - lineWidth / 2.f * std::sin(angle);

            polygon->mLeftBottom.mX = realStart.mX - lineWidth / 2.f * std::cos(angle);
            polygon->mLeftBottom.mY = realStart.mY + lineWidth / 2.f * std::sin(angle);

            polygon->mRightTop.mX = realEnd.mX + lineWidth / 2.f * std::cos(angle);
            polygon->mRightTop.mY = realEnd.mY - lineWidth / 2.f * std::sin(angle);

            polygon->mRightBottom.mX = realEnd.mX - lineWidth / 2.f * std::cos(angle);
            polygon->mRightBottom.mY = realEnd.mY + lineWidth / 2.f * std::sin(angle);
        }
    }
}

void VectorHelper::vertex2d(float *vertex, float x, float y, const Viewport &viewport) {
    if (viewport.mWidth == 0 || viewport.mHeight == 0) {
        LogE("invalid view port(%u, %u, %u, %u)", viewport.mStartX, viewport.mStartY,
             viewport.mWidth, viewport.mHeight);
    } else {
        float halfPortWidth = ((float) viewport.mWidth) / ((float) 2);
        float halfPortHeight = ((float) viewport.mHeight) / ((float) 2);
        *vertex = (x - halfPortWidth) / halfPortWidth;
        vertex++;
        *vertex = (halfPortHeight - y) / halfPortHeight;
    }
}

void VectorHelper::vertex2d(float *vertex, const Point2d &pt, const Viewport &viewport) {
    if (viewport.mWidth == 0 || viewport.mHeight == 0) {
        LogE("invalid view port(%u, %u, %u, %u)", viewport.mStartX, viewport.mStartY,
             viewport.mWidth, viewport.mHeight);
    } else {
        float halfPortWidth = ((float) viewport.mWidth) / ((float) 2);
        float halfPortHeight = ((float) viewport.mHeight) / ((float) 2);
        *vertex = (float(pt.mX) - halfPortWidth) / halfPortWidth;
        vertex++;
        *vertex = (halfPortHeight - float(pt.mY)) / halfPortHeight;
    }
}

void VectorHelper::vector2d(/*out*/float *vector, const float *startPt, const float *endPt) {
    vector[0] = endPt[0] - startPt[0];
    vector[1] = endPt[1] - startPt[1];
}