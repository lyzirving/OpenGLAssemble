//
// Created by lyzirving on 2022/6/27.
//
#ifndef OPENGLASSEMBLE_CAMERA_H
#define OPENGLASSEMBLE_CAMERA_H

#include <glm/glm.hpp>
#include <atomic>

// default camera values
const static float PITCH         =  0.0f;
const static float YAW           = -90.0f;
const static float DEFAULT_ZOOM  =  45.0f;
const static float ZOOM_SPAN     =  35.0f;
const static float CAMERA_LENS   =  3.f;

class Camera {
public:
    Camera(glm::vec3 camPos = glm::vec3(0.f, 0.f, CAMERA_LENS),
           float pitch = PITCH,
           float yaw = YAW);

    void adjustFov(float ratio);
    const glm::vec3& getCameraPosition();
    const glm::mat4& getViewMatrix();
    float getFov();
    void liftUpVision(float ratio);
    void moveCameraTo(float x, float y, float z);

private:
    /**
     * @brief calculates camera's x-axis and y-axis by camera-sight's pitch and yaw
     */
    void updateViewMatrix();

    // camera attributes
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
     *        pitch is positive when it rotates from x-z plane to +y-axis.
     *        pitch will decide how high the camera is from the x-z plane.
     */
    float mCamSightPitch;

    /**
     * @brief pitch and yaw are euler angles that are used to decide the camera-sight vector.
     *        note that the x, y and z axises mentioned below are OpenGL's standard coordinate axises.
     *        yaw is rotated around y-axis, and it should be only expressed in x-z plane.
     *        yaw is positive when it rotates from  +x-axis to +z-axis.
     *        yaw will decide the rotation between camera and +z-axis.
     */
    float mCamSightYaw;

    // camera options
    /**
     * @brief field of view in y direction, and it's valued in degree
     */
    float mFov;

    glm::mat4 mViewM;
    float mRightAxisSign;
    std::atomic<bool> mChange;
};

#endif //OPENGLASSEMBLE_CAMERA_H
