//
// Created by lyzirving on 2022/6/27.
//
#ifndef OPENGLASSEMBLE_CAMERA_H
#define OPENGLASSEMBLE_CAMERA_H

#include <glm/glm.hpp>

// Default camera values
const static float PITCH       =  0.0f;
const static float YAW         = -90.0f;
const static float ZOOM        =  45.0f;

class Camera {
public:
    // camera Attributes
    glm::vec3 mPosition;
    glm::vec3 mWorldUp;
    glm::vec3 mFront; // -(dst - position)
    glm::vec3 mRight; // camera's x
    glm::vec3 mUp;    // camera's y
    // euler Angles
    float mPitch;// rotate around x
    float mYaw;  // rotate around y
    // camera options
    float mZoom;

    Camera(glm::vec3 position,
           glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
           float pitch = PITCH,
           float yaw = YAW);

    glm::mat4 getViewMatrix();

private:
    /**
     * @brief calculates the front vector from the Camera's (updated) Euler Angles
     */
    void updateCameraVectors();

};

#endif //OPENGLASSEMBLE_CAMERA_H
