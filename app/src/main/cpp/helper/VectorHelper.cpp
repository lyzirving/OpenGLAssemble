//
// Created by liuyuzhou on 2022/5/25.
//
#include <cmath>

#include "VectorHelper.h"
#include "BaseRendererProgram.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VectorHelper"

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
        polygon[0] = start[0] - lineWidth;
        polygon[1] = start[1];

        polygon[2] = start[0] + lineWidth;
        polygon[3] = start[1];

        polygon[4] = end[0] - lineWidth;
        polygon[5] = end[1];

        polygon[6] = end[0] + lineWidth;
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
        polygon[1] = start[1] + lineWidth;

        polygon[2] = start[0];
        polygon[3] = start[1] - lineWidth;

        polygon[4] = end[0];
        polygon[5] = end[1] + lineWidth;

        polygon[6] = end[0];
        polygon[7] = end[1] - lineWidth;
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
        polygon[0] = start[0] - lineWidth * std::cos(angle);
        polygon[1] = start[1] + lineWidth * std::sin(angle);

        polygon[2] = start[0] + lineWidth * std::cos(angle);
        polygon[3] = start[1] - lineWidth * std::sin(angle);

        polygon[4] = end[0] - lineWidth * std::cos(angle);
        polygon[5] = end[1] + lineWidth * std::sin(angle);

        polygon[6] = end[0] + lineWidth * std::cos(angle);
        polygon[7] = end[1] - lineWidth * std::sin(angle);
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
        polygon[0] = start[0] + lineWidth * std::cos(angle);
        polygon[1] = start[1] + lineWidth * std::sin(angle);

        polygon[2] = start[0] - lineWidth * std::cos(angle);
        polygon[3] = start[1] - lineWidth * std::sin(angle);

        polygon[4] = end[0] + lineWidth * std::cos(angle);
        polygon[5] = end[1] + lineWidth * std::sin(angle);

        polygon[6] = end[0] - lineWidth * std::cos(angle);
        polygon[7] = end[1] - lineWidth * std::sin(angle);
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

void VectorHelper::vector2d(/*out*/float *vector, const float *startPt, const float *endPt) {
    vector[0] = endPt[0] - startPt[0];
    vector[1] = endPt[1] - startPt[1];
}