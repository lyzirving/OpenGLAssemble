//
// Created by liuyuzhou on 2022/5/23.
//
#ifndef OPENGLASSEMBLE_MATRIXUTIL_H
#define OPENGLASSEMBLE_MATRIXUTIL_H

#include <cstdint>

class MatrixUtil {
public:
    static void flip(float *src, bool horizontal, bool vertical);
    static void identity(float *src);
    static float length(float x, float y, float z);
    static void rotate(float *src, float angle, float x, float y, float z);
    static void scale(float *src, float x, float y, float z);

private:
    static void arrayCopy(float *src, uint32_t srcOffset, float *dst, uint32_t dstOffset, uint32_t length);
    static void buildRotateM(float *src, float angle, float x, float y, float z);
    static void multiplyMM(float * result, float *lhs, float *rhs);
};

#endif //OPENGLASSEMBLE_MATRIXUTIL_H
