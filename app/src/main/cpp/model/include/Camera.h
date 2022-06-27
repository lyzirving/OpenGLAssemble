//
// Created by lyzirving on 2022/6/27.
//
#ifndef OPENGLASSEMBLE_CAMERA_H
#define OPENGLASSEMBLE_CAMERA_H

#include <glm/glm.hpp>

// Default camera values
const static float YAW         = -90.0f;
const static float PITCH       =  0.0f;
const static float SPEED       =  2.5f;
const static float SENSITIVITY =  0.1f;
const static float ZOOM        =  45.0f;

class Camera {
public:
    // camera Attributes
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;
    // euler Angles
    float mYaw;
    float mPitch;
    // camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH);

    glm::mat4 getViewMatrix();

private:
    /**
     * @brief calculates the front vector from the Camera's (updated) Euler Angles
     */
    void updateCameraVectors();

};

#endif //OPENGLASSEMBLE_CAMERA_H
