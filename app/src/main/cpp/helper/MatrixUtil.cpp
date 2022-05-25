//
// Created by liuyuzhou on 2022/5/23.
//
#include <cmath>
#include <mutex>

#include "MatrixUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "TwoDimensRenderer"

#define MATRIX_LEN 16
#define I(_i, _j) ((_j) + 4 * (_i))

static float gTmpMatrix[32];
static std::mutex mMutex{};

void MatrixUtil::arrayCopy(float *src, uint32_t srcOffset, float *dst, uint32_t dstOffset,
                           uint32_t length) {
    for (int32_t i = 0; i < length; i++) {
        dst[i + dstOffset] = src[i + srcOffset];
    }
}

void MatrixUtil::buildRotateM(float *src, float angle, float x, float y, float z) {
    src[3] = 0;
    src[7] = 0;
    src[11] = 0;
    src[12] = 0;
    src[13] = 0;
    src[14] = 0;
    src[15] = 1;

    float pi = 4.0f * atan(1.0f);
    float radian = angle * pi / 180.0f;
    float sinVal = sin(radian);
    float cosVal = cos(radian);

    if (1 == x && 0 == y && 0 == z) {
        src[5] = cosVal;
        src[10] = cosVal;
        src[6] = sinVal;
        src[9] = -sinVal;
        src[1] = 0;
        src[2] = 0;
        src[4] = 0;
        src[8] = 0;
        src[0] = 1;
    } else if (0 == x && 1 == y && 0 == z) {
        src[0] = cosVal;
        src[10] = cosVal;
        src[8] = sinVal;
        src[2] = -sinVal;
        src[1] = 0;
        src[4] = 0;
        src[6] = 0;
        src[9] = 0;
        src[5] = 1;
    } else if (0 == x && 0 == y && 1 == z) {
        src[0] = cosVal;
        src[5] = cosVal;
        src[1] = sinVal;
        src[4] = -sinVal;
        src[2] = 0;
        src[6] = 0;
        src[8] = 0;
        src[9] = 0;
        src[10] = 1;
    } else {
        float len = length(x, y, z);
        if (1 != len) {
            float recipLen = 1.0f / len;
            x *= recipLen;
            y *= recipLen;
            z *= recipLen;
        }
        float nc = 1 - cosVal;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * sinVal;
        float ys = y * sinVal;
        float zs = z * sinVal;
        src[0] = x * x * nc + cosVal;
        src[4] = xy * nc - zs;
        src[8] = zx * nc + ys;
        src[1] = xy * nc + zs;
        src[5] = y * y * nc + cosVal;
        src[9] = yz * nc - xs;
        src[2] = zx * nc - ys;
        src[6] = yz * nc + xs;
        src[10] = z * z * nc + cosVal;
    }
}

void MatrixUtil::flip(float *src, bool horizontal, bool vertical) {
    if (horizontal || vertical) {
        scale(src, horizontal ? -1 : 1, vertical ? -1 : 1, 1);
    }
}

void MatrixUtil::identity(float *src) {
    for (int i = 0; i < MATRIX_LEN; i++) {
        src[i] = 0;
    }
    for (int i = 0; i < MATRIX_LEN; i += 5) {
        src[i] = 1.0f;
    }
}

float MatrixUtil::length(float x, float y, float z) {
    return sqrt(x * x + y * y + z * z);
}

void MatrixUtil::multiplyMM(float *result, float *lhs, float *rhs) {
    for (int i = 0; i < 4; i++) {
        const float rhs_i0 = rhs[I(i, 0)];
        float ri0 = lhs[I(0, 0)] * rhs_i0;
        float ri1 = lhs[I(0, 1)] * rhs_i0;
        float ri2 = lhs[I(0, 2)] * rhs_i0;
        float ri3 = lhs[I(0, 3)] * rhs_i0;
        for (int j = 1; j < 4; j++) {
            const float rhs_ij = rhs[I(i, j)];
            ri0 += lhs[I(j, 0)] * rhs_ij;
            ri1 += lhs[I(j, 1)] * rhs_ij;
            ri2 += lhs[I(j, 2)] * rhs_ij;
            ri3 += lhs[I(j, 3)] * rhs_ij;
        }
        result[I(i, 0)] = ri0;
        result[I(i, 1)] = ri1;
        result[I(i, 2)] = ri2;
        result[I(i, 3)] = ri3;
    }
}

void MatrixUtil::rotate(float *src, float angle, float x, float y, float z) {
    std::lock_guard<std::mutex> lockGuard(mMutex);
    buildRotateM(gTmpMatrix, angle, x, y, z);
    multiplyMM(gTmpMatrix + MATRIX_LEN, src, gTmpMatrix);
    arrayCopy(gTmpMatrix, MATRIX_LEN, src, 0, MATRIX_LEN);
}

void MatrixUtil::scale(float *src, float x, float y, float z) {
    for (int i = 0; i < 4; i++) {
        src[i] *= x;
        src[4 + i] *= y;
        src[8 + i] *= z;
    }
}


