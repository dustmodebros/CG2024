#pragma once

#include "Colour.h"
#include <iostream>

struct TexturePoint {
	float x{};
	float y{};
	Colour colour;

	TexturePoint();
	TexturePoint(float xPos, float yPos);
	friend std::ostream &operator<<(std::ostream &os, const TexturePoint &point);
	TexturePoint(float xPos, float yPos, Colour colour);

};

