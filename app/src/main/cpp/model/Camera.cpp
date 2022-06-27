//
// Created by lyzirving on 2022/6/27.
//
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
        mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
        mMovementSpeed(SPEED),
        mMouseSensitivity(SENSITIVITY),
        mZoom(ZOOM) {
    mPosition = position;
    mWorldUp = up;
    mYaw = yaw;
    mPitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::updateCameraVectors() {
    // calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    // re-calculate the right and up vector
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    mRight = glm::normalize(glm::cross(mFront,mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}

