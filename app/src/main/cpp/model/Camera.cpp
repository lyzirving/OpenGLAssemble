//
// Created by lyzirving on 2022/6/27.
//
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(glm::vec3 camPos, glm::vec3 worldUp, float pitch, float yaw) :
        mCamPosition(camPos), mWorldUp(worldUp),
        mCamSight(), mCamRight(), mCamUp(),
        mCamSightPitch(pitch), mCamSightYaw(yaw),
        mZoom(ZOOM) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(mCamPosition, mCamPosition + mCamSight, mCamUp);
}

void Camera::updateCameraVectors() {
    // calculate the front vector by pitch and yaw
    glm::vec3 front;
    front.x = cos(glm::radians(mCamSightPitch)) * cos(glm::radians(mCamSightYaw));
    front.y = sin(glm::radians(mCamSightPitch));
    front.z = cos(glm::radians(mCamSightPitch)) * sin(glm::radians(mCamSightYaw));
    // if we use default camera attribute(pitch = 0, yaw = -90), the camera sight vector is (0, 0, -1)
    mCamSight = glm::normalize(front);
    // calculate the right and up vector
    mCamRight = glm::normalize(glm::cross(mCamSight, mWorldUp));
    mCamUp = glm::normalize(glm::cross(mCamRight, mCamSight));
}

