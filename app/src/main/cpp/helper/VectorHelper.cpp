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

double Vector2d::azimuth() {
    float val = slope();
    double angle;
    if(val == MAXFLOAT) {
        angle = M_PI_2;
    } else {
        angle = std::atan(val);
        // limit angle to 0~PI
        if (angle < 0)
            angle += M_PI;
    }
    return angle;
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

void VectorHelper::fillTurningPtPolygon_v1(Polygon2d *result, bool setForLeft,
                                           const Point2d &centerLineStart, const Point2d &centerLineEnd,
                                           const Polygon2d &polygon,
                                           const Point2d &innerPt, const Point2d &outerPt) {
    Vector2d mainVec(centerLineStart, centerLineEnd);
    float slope = mainVec.slope();
    if(setForLeft) {
        if(slope == 0) {
            if(mainVec.mDx < 0) {
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftBottom.mX, polygon.mLeftBottom.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                }
            }
        }
        else if(slope == MAXFLOAT) {
            if(mainVec.mDy < 0) {
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftBottom.mX, polygon.mLeftBottom.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                }
            }
        }
        else if(slope > 0) {
            if(mainVec.mDy < 0) {
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftBottom.mX, polygon.mLeftBottom.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {// mDy > 0
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftBottom.set(innerPt.mX, innerPt.mY);
                }
            }
        }
        else {//slope < 0
            if(mainVec.mDy < 0) {
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                }
                else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {// mDy > 0
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftBottom.mX, polygon.mLeftBottom.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                }
                else {
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                }
            }
        }
    }
    else {// set for right handed side vector
        if(slope == 0) {
            if(mainVec.mDx < 0) {
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftBottom.mX, polygon.mLeftBottom.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftBottom.set(innerPt.mX, innerPt.mY);
                }
            }
        }
        else if(slope == MAXFLOAT) {
            if(mainVec.mDy < 0) {
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                }
            }
        }
        else if(slope > 0) {
            if(mainVec.mDy < 0) {
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftBottom.mX, polygon.mLeftBottom.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                }
            }
        }
        else {// slope < 0
            if (mainVec.mDy < 0) {
                result->mLeftTop.set(polygon.mLeftTop.mX, polygon.mLeftTop.mY);
                result->mLeftBottom.set(polygon.mLeftBottom.mX, polygon.mLeftBottom.mY);

                bool sameSide = ptsSameSide(polygon.mLeftTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mRightTop.set(innerPt.mX, innerPt.mY);
                    result->mRightBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mRightTop.set(outerPt.mX, outerPt.mY);
                    result->mRightBottom.set(innerPt.mX, innerPt.mY);
                }
            }
            else {
                result->mRightTop.set(polygon.mRightTop.mX, polygon.mRightTop.mY);
                result->mRightBottom.set(polygon.mRightBottom.mX, polygon.mRightBottom.mY);

                bool sameSide = ptsSameSide(polygon.mRightTop, innerPt, centerLineStart, centerLineEnd);
                if(sameSide) {
                    result->mLeftTop.set(innerPt.mX, innerPt.mY);
                    result->mLeftBottom.set(outerPt.mX, outerPt.mY);
                } else {
                    result->mLeftTop.set(outerPt.mX, outerPt.mY);
                    result->mLeftBottom.set(innerPt.mX, innerPt.mY);
                }
            }
        }
    }
}

void VectorHelper::fillTurningPtPolygon_v2(Polygon2d *result, const Point2d &preStart,
                                           const Point2d &start, const Point2d &end,
                                           const Point2d &startInnerPt, const Point2d &startOuterPt,
                                           const Point2d &endInnerPt, const Point2d &endOuterPt) {
    Vector2d mainVec(start, end);
    float slope = mainVec.slope();
    double azimuth = mainVec.azimuth();

    Vector2d preVec(preStart, start);
    double preVecAzi = mainVec.azimuth();

    bool sameSide = ptsSameSide(startInnerPt, endInnerPt, start, end);

    if(slope == 0) {
        if(mainVec.mDx < 0) {
            if(preVec.mDy >= 0) {
                result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                if(sameSide) {
                    result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                } else {
                    result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                }
            }
            else {
                result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                if(sameSide) {
                    result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                } else {
                    result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                }
            }
        }
        else {
            if(preVec.mDy >= 0) {
                result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                if(sameSide) {
                    result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                } else {
                    result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                }
            }
            else {
                result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                if(sameSide) {
                    result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                } else {
                    result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                }
            }
        }
    }
    else if(slope == MAXFLOAT) {
        if(mainVec.mDy < 0) {
            if((preVec.mDy > 0 && preVecAzi < M_PI_2) || (preVec.mDy < 0 && preVecAzi > M_PI_2)) {
                result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                if(sameSide) {
                    result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                } else {
                    result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                }
            }
            else if((preVec.mDy > 0 && preVecAzi > M_PI_2) || (preVec.mDy < 0 && preVecAzi < M_PI_2)) {
                result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                if(sameSide) {
                    result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                } else {
                    result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                }
            }
        }
        else {
            if((preVec.mDy > 0 && preVecAzi < M_PI_2) || (preVec.mDy < 0 && preVecAzi > M_PI_2)) {
                result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                if(sameSide) {
                    result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                } else {
                    result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                }
            }
            else if((preVec.mDy > 0 && preVecAzi > M_PI_2) || (preVec.mDy < 0 && preVecAzi < M_PI_2)) {
                result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                if(sameSide) {
                    result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                    result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                } else {
                    result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                    result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                }
            }
        }
    }
    else if(slope > 0) {
        if(mainVec.mDy < 0) {
            if(preVec.mDy > 0) {
                if(preVecAzi > azimuth) {
                    result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
                else {
                    result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
            }
            else if(preVec.mDy < 0) {
                if(preVecAzi > azimuth) {
                    result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
                else {
                    result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
            }
            else {
                if (preStart.mX < start.mX) {
                    result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
                else {
                    result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
            }
        }
        else {//mDy > 0
            if(preVec.mDy > 0) {
                if(preVecAzi > azimuth) {
                    result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
                else {
                    result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
            }
            else if(preVec.mDy < 0){//preVec.mDy < 0
                if(preVecAzi > azimuth) {
                    result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
                else {
                    result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
            }
            else { //preVec.mDy == 0
                if(preStart.mX > start.mX) {
                    result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
                else {
                    result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
            }
        }
    }
    else {//slope < 0
        if(mainVec.mDy < 0) {
            if(preVec.mDy > 0) {
                if(preVecAzi > azimuth) {
                    result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
                else {
                    result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
            }
            else if(preVec.mDy < 0) {
                if(preVecAzi > azimuth) {
                    result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
                else {
                    result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
            }
            else {
                if(preStart.mX < start.mX) {
                    result->mRightTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mRightBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
                else {
                    result->mRightTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mRightBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mLeftTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mLeftBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mLeftTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mLeftBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
            }
        }
        else {
            if(preVec.mDy > 0) {
                if(preVecAzi > azimuth) {
                    result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
                else {
                    result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
            }
            else if(preVec.mDy < 0) {
                if(preVecAzi > azimuth) {
                    result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
                else {
                    result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
            }
            else {
                if(preStart.mX < start.mX) {
                    result->mLeftTop.set(startOuterPt.mX, startOuterPt.mY);
                    result->mLeftBottom.set(startInnerPt.mX, startInnerPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    } else {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    }
                }
                else {
                    result->mLeftTop.set(startInnerPt.mX, startInnerPt.mY);
                    result->mLeftBottom.set(startOuterPt.mX, startOuterPt.mY);
                    if(sameSide) {
                        result->mRightTop.set(endInnerPt.mX, endInnerPt.mY);
                        result->mRightBottom.set(endOuterPt.mX, endOuterPt.mY);
                    } else {
                        result->mRightTop.set(endOuterPt.mX, endOuterPt.mY);
                        result->mRightBottom.set(endInnerPt.mX, endInnerPt.mY);
                    }
                }
            }
        }
    }
}

void VectorHelper::lineTurningPt(Point2d *pt1, Point2d *pt2, const Point2d &startPt,
                                           const Point2d &midPt, const Point2d &endPt,
                                           const uint32_t lineWidth) {
    Vector2d lhsVec(startPt, midPt);
    float lhsSlope = lhsVec.slope();
    double lhsAngle = lhsVec.azimuth();

    Vector2d rhsVec(midPt, endPt);
    double rhsAngle = rhsVec.azimuth();

    double intersectAngle, dstAngle;
    double len;

    if (lhsSlope == 0) {
        if(lhsVec.mDx > 0) {
            if(rhsVec.mDy < 0) {
                intersectAngle = M_PI - rhsAngle;
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = intersectAngle * 0.5f;
                pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
            }
            else {
                intersectAngle = rhsAngle;
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = intersectAngle * 0.5f;
                pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
            }
        }
        else {
            if(rhsVec.mDy < 0) {
                intersectAngle = rhsAngle;
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = intersectAngle * 0.5f;
                pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
            }
            else {
                intersectAngle = M_PI - rhsAngle;
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = intersectAngle * 0.5f;
                pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
            }
        }
    }
    else if (lhsSlope == MAXFLOAT) {
        if(lhsVec.mDy < 0) {
            if(rhsVec.mDy < 0) {
                intersectAngle = rhsAngle > M_PI * 0.5f ?
                                 (rhsAngle - M_PI * 0.5f) : (M_PI * 0.5f - rhsAngle);
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = rhsAngle > M_PI * 0.5f ?
                           (intersectAngle * 0.5f) : (intersectAngle * 0.5f + rhsAngle);
                if(rhsAngle > M_PI * 0.5f) {
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
            }
            else {
                intersectAngle = rhsAngle > M_PI * 0.5f ?
                                 (M_PI * 1.5f - rhsAngle) : (M_PI * 0.5f + rhsAngle);
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = M_PI * 0.5f - intersectAngle * 0.5f;
                if(rhsAngle > M_PI * 0.5f) {
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
            }
        }
        else {
            if(rhsVec.mDy < 0) {
                intersectAngle = rhsAngle > M_PI * 0.5f ?
                                 (rhsAngle - M_PI * 0.5f) : (M_PI * 0.5f - rhsAngle);
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = rhsAngle > M_PI * 0.5f ?
                           (M_PI - rhsAngle + intersectAngle * 0.5f) : (intersectAngle * 0.5f + rhsAngle);
                if(rhsAngle > M_PI * 0.5f) {
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
            }
            else {
                intersectAngle = rhsAngle > M_PI * 0.5f ?
                                 (M_PI * 1.5f - rhsAngle) : (rhsAngle + M_PI * 0.5f);
                len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                dstAngle = M_PI * 0.5f - intersectAngle * 0.5f;
                if(rhsAngle > M_PI * 0.5f) {
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
            }
        }
    }
    else if (lhsSlope > 0) {
        //in this condition, lhs angle ranges from 0 to Pi / 2
        if (lhsVec.mDy < 0) {
            if (lhsAngle > rhsAngle) {
                if (rhsVec.mDy < 0) {
                    intersectAngle = M_PI - (lhsAngle - rhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = M_PI - lhsAngle - intersectAngle * 0.5f;
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = lhsAngle - rhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = rhsAngle + intersectAngle * 0.5f;
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
            }
            else {
                if (rhsVec.mDy < 0) {
                    intersectAngle = M_PI - (rhsAngle - lhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = M_PI - rhsAngle - intersectAngle * 0.5f;
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = rhsAngle - lhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = lhsAngle + intersectAngle * 0.5f;
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
            }
        }
        else {
            if (lhsAngle > rhsAngle) {
                if (rhsVec.mDy < 0) {
                    intersectAngle = lhsAngle - rhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = rhsAngle + intersectAngle * 0.5f;
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = M_PI - (lhsAngle - rhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = M_PI - lhsAngle - intersectAngle * 0.5f;
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
            }
            else {
                if (rhsVec.mDy < 0) {
                    intersectAngle = rhsAngle - lhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = rhsAngle - intersectAngle * 0.5f;
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = M_PI - (rhsAngle - lhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = intersectAngle * 0.5f - lhsAngle;
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
            }
        }
    }
    else {// lhsSlope < 0
        if(lhsVec.mDy < 0) {
            if (lhsAngle > rhsAngle) {
                if(rhsVec.mDy < 0) {
                    intersectAngle = M_PI - (lhsAngle - rhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = intersectAngle * 0.5f - (M_PI - lhsAngle);
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = lhsAngle - rhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = intersectAngle * 0.5f + (M_PI - lhsAngle);
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
            }
            else {
                if(rhsVec.mDy < 0) {
                    intersectAngle = M_PI - (rhsAngle - lhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = lhsAngle - intersectAngle * 0.5f;
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = rhsAngle - lhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = M_PI - rhsAngle + intersectAngle * 0.5f;
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
            }
        }
        else {
            if (lhsAngle > rhsAngle) {
                if(rhsVec.mDy < 0) {
                    intersectAngle = lhsAngle - rhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = rhsAngle + intersectAngle * 0.5f;
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = M_PI - (lhsAngle - rhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle =  intersectAngle * 0.5f - (M_PI - lhsAngle);
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY + (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY - (float) (len * std::sin(dstAngle));
                }
            }
            else {
                if(rhsVec.mDy < 0) {
                    intersectAngle = rhsAngle - lhsAngle;
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = M_PI - rhsAngle + intersectAngle * 0.5f;
                    pt1->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
                else {
                    intersectAngle = M_PI - (rhsAngle - lhsAngle);
                    len = lineWidth * 0.5f / std::sin(intersectAngle * 0.5f);
                    dstAngle = intersectAngle * 0.5f - (M_PI - rhsAngle);
                    pt1->mX = midPt.mX + (float) (len * std::cos(dstAngle));
                    pt1->mY = midPt.mY - (float) (len * std::sin(dstAngle));

                    pt2->mX = midPt.mX - (float) (len * std::cos(dstAngle));
                    pt2->mY = midPt.mY + (float) (len * std::sin(dstAngle));
                }
            }
        }
    }
}

bool VectorHelper::ptsSameSide(const Point2d &pt1, const Point2d &pt2, const Point2d &lineStart,
                               const Point2d &lineEnd) {
    Vector2d pt1lhs(lineStart, pt1);
    Vector2d pt1rhs(lineEnd, pt1);
    float crossVal1 = pt1lhs.mDx * pt1rhs.mDy - pt1rhs.mDx * pt1lhs.mDy;

    Vector2d pt2lhs(lineStart, pt2);
    Vector2d pt2rhs(lineEnd, pt2);
    float crossVal2 = pt2lhs.mDx * pt2rhs.mDy - pt2rhs.mDx * pt2lhs.mDy;

    return crossVal1 * crossVal2 >= 0;
}

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

void VectorHelper::segToContinuousPolygon(Polygon2d *polygon, const Point2d *ptOnScreen,
                                          uint32_t start, uint32_t count, const uint32_t lineWidth) {
    uint32_t end =  start + 1;
    if(end > count - 1) {
        LogE("invalid input, start(%u), count(%u)", start, count);
        return;
    }
    if (count == 2) {
        segmentToPolygon(polygon, ptOnScreen[0], ptOnScreen[1], lineWidth);
    } else if (start == 0) {
        Polygon2d tmp;
        segmentToPolygon(&tmp, ptOnScreen[start], ptOnScreen[end], lineWidth);

        Point2d innerPt, outerPt;
        lineTurningPt(&innerPt, &outerPt, ptOnScreen[start], ptOnScreen[end], ptOnScreen[end + 1], lineWidth);

        fillTurningPtPolygon_v1(polygon, true, ptOnScreen[start], ptOnScreen[end], tmp, innerPt, outerPt);
    } else if (start == count - 2) {
        Polygon2d tmp;
        segmentToPolygon(&tmp, ptOnScreen[start], ptOnScreen[end], lineWidth);

        Point2d innerPt, outerPt;
        lineTurningPt(&innerPt, &outerPt, ptOnScreen[start - 1], ptOnScreen[start], ptOnScreen[end], lineWidth);

        fillTurningPtPolygon_v1(polygon, false, ptOnScreen[start], ptOnScreen[end], tmp, innerPt, outerPt);
    } else {
        Point2d innerPt1, outerPt1, innerPt2, outerPt2;
        lineTurningPt(&innerPt1, &outerPt1, ptOnScreen[start - 1], ptOnScreen[start], ptOnScreen[end], lineWidth);
        lineTurningPt(&innerPt2, &outerPt2, ptOnScreen[start], ptOnScreen[end], ptOnScreen[end + 1], lineWidth);

        fillTurningPtPolygon_v2(polygon, ptOnScreen[start - 1], ptOnScreen[start], ptOnScreen[end], innerPt1, outerPt1, innerPt2, outerPt2);
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