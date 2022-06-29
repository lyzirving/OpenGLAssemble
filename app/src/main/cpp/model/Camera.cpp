//
// Created by lyzirving on 2022/6/27.
//
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float pitch, float yaw) :
        mPosition(position), mWorldUp(worldUp),
        mFront(), mRight(), mUp(),
        mPitch(pitch), mYaw(yaw),
        mZoom(ZOOM) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::updateCameraVectors() {
    // calculate the front vector by pitch and yaw
    glm::vec3 front;
    front.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
    front.y = sin(glm::radians(mPitch));
    front.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
    mFront = glm::normalize(front);
    // calculate the right and up vector
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}

