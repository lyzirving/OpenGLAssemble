//
// Created by lyzirving on 2022/6/27.
//
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "Camera.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

Camera::Camera(glm::vec3 camPos, float pitch, float yaw) :
        mCamPosition(camPos),
        mWorldUp(glm::vec3(0.f, 1.f, 0.f)),
        mCamSight(), mCamRight(), mCamUp(),
        mCamSightPitch(pitch), mCamSightYaw(yaw),
        mViewFieldY(ZOOM),
        mViewM(1.f),
        mRightAxisSign(1.f),
        mChange(true) {}

const glm::mat4& Camera::getViewMatrix() {
    updateViewMatrix();
    return mViewM;
}

float Camera::getViewFieldY() {
    return mViewFieldY;
}

void Camera::liftUpVision(float ratio) {
    if(ratio > 1.f)
        ratio = 1.f;
    else if (ratio < -1.f)
        ratio = -1.f;

    float sign = std::abs(ratio) / ratio;
    mCamPosition.y = std::sin(M_PI * ratio) * CAMERA_LENS;
    mCamPosition.z = std::cos(M_PI * ratio) * CAMERA_LENS;
    if(std::abs(ratio) <= 0.5f)
    {
        mRightAxisSign = 1.f;
        mCamSightYaw = YAW;
        mCamSightPitch = glm::degrees(-M_PI * ratio);
    }
    else
    {
        mRightAxisSign = -1.f;
        mCamSightYaw = -YAW;
        mCamSightPitch = -sign * (float)glm::degrees(M_PI - M_PI * std::abs(ratio));
    }
    mChange.store(true);
}

void Camera::moveCameraTo(float x, float y, float z) {
    mCamPosition.x = x;
    mCamPosition.y = y;
    mCamPosition.z = z;
    mChange.store(true);
}

void Camera::updateViewMatrix() {
    if(mChange.load())
    {
        // calculate the sight vector by pitch and yaw
        // camera' vision direction is the opposite of camera front
        glm::vec3 vision;
        vision.x = cos(glm::radians(mCamSightPitch)) * cos(glm::radians(mCamSightYaw));
        vision.y = sin(glm::radians(mCamSightPitch));
        vision.z = cos(glm::radians(mCamSightPitch)) * sin(glm::radians(mCamSightYaw));
        // if we use default camera attribute(pitch = 0, yaw = -90), the camera sight vector is (0, 0, -1)
        mCamSight = glm::normalize(vision);
        // calculate the right and up vector
        mCamRight = mRightAxisSign * glm::normalize(glm::cross(mCamSight, mWorldUp));
        mCamUp = glm::normalize(glm::cross(mCamRight, mCamSight));
        // calculate view matrix
        mViewM = glm::lookAt(mCamPosition, mCamPosition + mCamSight, mCamUp);
        LogI("cam pos(%f, %f, %f)\ncam sight(%f, %f, %f)\ncam up(%f, %f, %f)",
             mCamPosition.x, mCamPosition.y, mCamPosition.z,
             mCamSight.x, mCamSight.y, mCamSight.z,
             mCamUp.x, mCamUp.y, mCamUp.z);
        mChange.store(false);
    }
}

