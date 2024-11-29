//
// Created by Samuel Stephens on 15/10/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include <../glm-0.9.7.2/glm/detail/type_mat.hpp>
#include <../glm-0.9.7.2/glm/detail/type_mat3x3.hpp>
#include <../glm-0.9.7.2/glm/detail/type_vec.hpp>
#include <../glm-0.9.7.2/glm/detail/type_vec3.hpp>


class Camera {
public:
    glm::vec3 position;
    glm::mat3 orientation;
    float focalLength;
    std::string mode;

    Camera();
    explicit Camera(glm::vec3 position, glm::mat3 orientation, float focalLength);
    void translateCamera(glm::vec3 delta);
    void panCamera(float theta);
    void tiltCamera(float theta);
    void orbit(float speed);
    void lookAt(glm::vec3 point);

    void changeMode(std::string newMode);
};



#endif //CAMERA_H
