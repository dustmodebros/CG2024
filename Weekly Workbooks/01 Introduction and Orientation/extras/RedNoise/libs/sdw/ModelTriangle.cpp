#include "ModelTriangle.h"
#include <utility>
#include <glm/gtx/dual_quaternion.hpp>

ModelTriangle::ModelTriangle() = default;

ModelTriangle::ModelTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, Colour trigColour) :
		vertices({{v0, v1, v2}}), texturePoints(), colour(std::move(trigColour)), normal(normalize(cross(v0-v1, v0-v2))), textured(false) {}

ModelTriangle::ModelTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, Colour trigColour, const bool textured) :
		vertices({{v0, v1, v2}}), texturePoints(), colour(std::move(trigColour)), normal(normalize(cross(v0-v1, v0-v2))), textured(textured) {}

std::ostream &operator<<(std::ostream &os, const ModelTriangle &triangle) {
	os << "(" << triangle.vertices[0].x << ", " << triangle.vertices[0].y << ", " << triangle.vertices[0].z << ")\n";
	os << "(" << triangle.vertices[1].x << ", " << triangle.vertices[1].y << ", " << triangle.vertices[1].z << ")\n";
	os << "(" << triangle.vertices[2].x << ", " << triangle.vertices[2].y << ", " << triangle.vertices[2].z << ")\n";
	os << "colour: (" << triangle.colour.red << ", " << triangle.colour.green << ", " << triangle.colour.blue << ")\n";
	return os;
}