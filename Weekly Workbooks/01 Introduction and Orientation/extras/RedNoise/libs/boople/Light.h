//
// Created by Samuel Stephens on 17/11/2024.
//

#ifndef LIGHT_H
#define LIGHT_H
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>

class Light {
    public:
    glm::vec3 position;
    float intensity;

    Light();
    explicit Light(glm::vec3 position, float intensity);
};



#endif //LIGHT_H
