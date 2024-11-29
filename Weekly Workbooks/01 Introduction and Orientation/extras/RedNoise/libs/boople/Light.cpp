//
// Created by Samuel Stephens on 17/11/2024.
//
#include "Light.h"

Light::Light() {
    position = glm::vec3(0, 0.8, 0);
    intensity = 1;
}

Light::Light(glm::vec3 position, float intensity) {
    this->position = position;
    this->intensity = intensity;
}