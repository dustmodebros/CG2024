#include "TexturePoint.h"

#include "Colour.h"

TexturePoint::TexturePoint() = default;
TexturePoint::TexturePoint(float xPos, float yPos) : x(xPos), y(yPos){}
TexturePoint::TexturePoint(float xPos, float yPos, Colour colour) : x(xPos), y(yPos), colour(colour){}

std::ostream &operator<<(std::ostream &os, const TexturePoint &point) {
	os << "x: " << point.x << " y: " << point.y;
	return os;
}
