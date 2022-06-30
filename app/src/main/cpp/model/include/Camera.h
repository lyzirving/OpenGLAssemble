//
// Created by lyzirving on 2022/6/27.
//
#ifndef OPENGLASSEMBLE_CAMERA_H
#define OPENGLASSEMBLE_CAMERA_H

#include <glm/glm.hpp>

// default camera values
const static float PITCH       =  0.0f;
const static float YAW         = -90.0f;
const static float ZOOM        =  45.0f;

class Camera {
public:
    // camera Attributes
    glm::vec3 mCamPosition;
    glm::vec3 mWorldUp;
    /**
     * @brief camera's view direction
     */
    glm::vec3 mCamSight;
    /**
     * @brief camera's positive x-axis
     */
    glm::vec3 mCamRight;
    /**
     * @brief camera's positive y-axis
     */
    glm::vec3 mCamUp;
    /**
     * @brief pitch and yaw are euler angles that are used to decide the camera-sight vector.
     *        note that the x, y and z axises mentioned below are OpenGL's standard coordinate axises.
     *        pitch is rotated around x-axis, and it is an intersection angle between camera-sight vector and x-z plane.
     *        pitch is positive when it rotates from x-z plane to positive y-axis.
     *        pitch will decide how high the camera is from the x-z plane.
     */
    float mCamSightPitch;

    /**
     * @brief pitch and yaw are euler angles that are used to decide the camera-sight vector.
     *        note that the x, y and z axises mentioned below are OpenGL's standard coordinate axises.
     *        yaw is rotated around y-axis, and it should be only expressed in x-z plane.
     *        yaw is positive when it rotates from positive x-axis to positive z-axis.
     *        yaw will decide the rotation between camera and positive z-axis.
     */
    float mCamSightYaw;

    // camera options
    float mZoom;

    Camera(glm::vec3 camPos,
           glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
           float pitch = PITCH,
           float yaw = YAW);

    glm::mat4 getViewMatrix();

private:
    /**
     * @brief calculates camera's x-axis and y-axis by camera-sight's pitch and yaw
     */
    void updateCameraVectors();

};

#endif //OPENGLASSEMBLE_CAMERA_H
