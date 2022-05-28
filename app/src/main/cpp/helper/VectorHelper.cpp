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

float Polygon2d::width() {
    return (float) std::sqrt(std::pow(mLeftTop.mX - mRightTop.mX, 2) +
                             std::pow(mLeftTop.mY - mRightTop.mY, 2));
}

float Polygon2d::height() {
    return (float) std::sqrt(std::pow(mLeftTop.mX - mLeftBottom.mX, 2) +
                             std::pow(mLeftTop.mY - mLeftBottom.mY, 2));
}

VectorHelper::VectorHelper() = default;

VectorHelper::~VectorHelper() = default;

void VectorHelper::segmentToPolygon(Polygon2d *polygon, const Point2d &startPt,
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