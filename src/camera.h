#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
	glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movementSpeed = 0.03f;
    float mouseSensitivity = 0.5f;
    float zoom;

    Camera()
    {

    }

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    {
        this->position = position;
        this->worldUp = up;
        this->up = up;
        this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
        this->yaw = yaw;
        this->pitch = pitch;
        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->position, this->position + this->forward, this->up);
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->mouseSensitivity;
        yoffset *= this->mouseSensitivity;

        this->yaw += xoffset;
        this->pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->pitch > 89.0f)
                this->pitch = 89.0f;
            if (this->pitch < -89.0f)
                this->pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }


    void UpdateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        float pitch_cos = std::cos(glm::radians(this->pitch));
        front.x = std::cos(glm::radians(this->yaw)) * pitch_cos;
        front.y = std::sin(glm::radians(this->pitch));
        front.z = std::sin(glm::radians(this->yaw)) * pitch_cos;
        this->forward = glm::normalize(front);
        // also re-calculate the Right and Up vector
        this->right = glm::normalize(glm::cross(this->forward, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->forward));
    }
};

#endif //CAMERA_H