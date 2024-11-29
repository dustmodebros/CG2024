//
// Created by Samuel Stephens on 15/10/2024.
//

#include "Camera.h"
#include <glm/gtx/string_cast.hpp>

Camera::Camera() {
    position = glm::vec3(0, 0, 4);
    orientation = glm::mat3(glm::vec3(1,0,0),glm::vec3(0,1,0),glm::vec3(0,0,1));
}

Camera::Camera(glm::vec3 position, glm::mat3 orientation, float focalLength) {
    this->position = position;
    this->orientation = orientation;
    this->focalLength = focalLength;
    this->mode = "RASTERISE";
}

void Camera::translateCamera(glm::vec3 delta) {
    position -= delta;
}

void Camera::panCamera(float theta) {
    glm::mat3 matrix = glm::mat3(glm::vec3(glm::cos(theta),0,-glm::sin(theta)),glm::vec3(0,1,0),glm::vec3(glm::sin(theta),0,glm::cos(theta)));
    orientation = orientation*matrix;
}

void Camera::tiltCamera(float theta) {
    glm::mat3 matrix = glm::mat3(glm::vec3(1,0,0),glm::vec3(0,glm::cos(theta),glm::sin(theta)),glm::vec3(0, -glm::sin(theta), glm::cos(theta)));
    orientation = orientation * matrix;
}

void Camera::orbit(float speed) {
    glm::mat3 matrix = glm::mat3(glm::vec3(glm::cos(speed),0,-glm::sin(speed)),glm::vec3(0,1,0),glm::vec3(glm::sin(speed),0,glm::cos(speed)));
    position = matrix*position;
}

void Camera::lookAt(glm::vec3 point) {
    glm::vec3 forward = normalize(position-point);
    glm::vec3 right = normalize(cross(forward, glm::vec3(0,1,0)));
    glm::vec3 up = -normalize(cross(right, forward));
    glm::mat3 boop = {glm::vec3(-1,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,1)};
    orientation = boop*glm::mat3(right, up, forward);
}