#include <cstdio>
#include <iostream>
#include <ostream>
#include <sdw/CanvasTriangle.h>
#include <sdw/Colour.h>
#include <sdw/DrawingWindow.h>
#include <sdw/Utils.h>
#include <fstream>
#include <sdw/ModelTriangle.h>
#include <sdw/RayTriangleIntersection.h>
#include <sdw/TextureMap.h>
#include <string>
#include <utility>
#include <vector>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/ext.hpp>



#include <boople/Camera.h>

#include "SDL_keycode.h"
#include "SDL_scancode.h"
#include "boople/Light.h"
#include "glm/detail/func_geometric.hpp"
#include "glm/detail/type_mat.hpp"
#include "sdw/TexturePoint.h"

#define WIDTH 500
#define HEIGHT 400

template <typename T>
std::vector<T> interp(T from, T to, int const numberOfValues) {
	std::vector<T> outList;
	const T incr = (to-from) / static_cast<float>(numberOfValues-1);
	outList.reserve(numberOfValues);
	for (int i=0; i<numberOfValues; i++) {
		outList.push_back(from + i * incr);
	}
	return outList;
}

std::vector<glm::vec2> interpv2(const glm::vec2 from, const glm::vec2 to, int const numberOfValues) {
	std::vector<glm::vec2> outList;
	const glm::vec2 incr = (to-from) / static_cast<float>(numberOfValues-1);
	outList.reserve(numberOfValues);
	for (int i=0; i<numberOfValues; i++) {
		glm::vec2 val = from + i * incr;
		val.x = static_cast<int>(val.x);
		val.y = static_cast<int>(val.y);
		outList.push_back(from + i * incr);
	}
	return outList;
}

std::vector<glm::vec3> interpv3(const glm::vec3 from, const glm::vec3 to, int const numberOfValues) {
	std::vector<glm::vec3> outList;
	const glm::vec3 incr = (to-from) / static_cast<float>(numberOfValues-1);
	outList.reserve(numberOfValues);
	for (int i=0; i<numberOfValues; i++) {
		glm::vec3 val = from + i * incr;
		val.x = static_cast<int>(val.x);
		val.y = static_cast<int>(val.y);
		outList.push_back(from + i * incr);
	}
	return outList;
}

// Draws a straight interpolated line between two points with a colour to the window
void drawLine(const int x1, const int y1, const int x2, const int y2, Colour colour, DrawingWindow &window) {
	glm::vec2 start = glm::vec2(x1, y1);
	glm::vec2 end = glm::vec2(x2, y2);
	auto points = interpv2(start, end, std::abs(start.x - end.x) + std::abs(start.y-end.y)+1);
	for (auto point: points) {
		window.setPixelColour(static_cast<int>(point.x), static_cast<int>(point.y),colour.asARGB());
	}
}

// Draws a straight interpolated line between two points with a colour to the window wrt depths
void drawOccludedLine(const int x1, const int y1, const float z1, const int x2, const int y2, const float z2, Colour colour, std::vector<std::vector<float>> *depthBuffer, DrawingWindow &window) {
	glm::vec3 start = glm::vec3(x1, y1, z1);
	glm::vec3 end = glm::vec3(x2, y2, z2);
	auto points = interpv3(start, end, std::abs(start.x - end.x) + std::abs(start.y-end.y)+1);
	for (auto point: points) {
		if(point.x >=0 && point.y >= 0 && point.x < WIDTH && point.y < HEIGHT) {
			if((*depthBuffer)[point.y][point.x] < point.z ) {
				(*depthBuffer)[point.y][point.x] = point.z;
				window.setPixelColour(static_cast<int>(point.x), static_cast<int>(point.y),colour.asARGB());
			}
		}
	}
}

// Draws a RGB gradient to the window
void drawRainbow(DrawingWindow &window) {
	window.clearPixels();
	std::vector<glm::vec3> left = interp(glm::vec3(255,0,0), glm::vec3(255,255,0), window.height);
	std::vector<glm::vec3> right = interp(glm::vec3(0,0,255), glm::vec3(0,255,0), window.height);
	for (size_t y = 0; y < window.height; y++) {
		std::vector<uint32_t> colours;
		std::vector<glm::vec3> thingy = interp(left[y], right[y], window.width);
		for (glm::vec3 elem : thingy) {
			colours.push_back((255<<24) + (static_cast<int>(elem.x)<<16) + (static_cast<int>(elem.y)<<8) + static_cast<int>(elem.z));
		}
		for (size_t x = 0; x < window.width; x++) {
			window.setPixelColour(x,y,colours[x]);
		}
	}
}

// Returns a random CanvasTriangle
CanvasTriangle randomTriangle() {
	auto triangle = CanvasTriangle(
	CanvasPoint(rand()%WIDTH, rand()%HEIGHT, (rand()%100)-50),
	CanvasPoint(rand()%WIDTH, rand()%HEIGHT, (rand()%100)-50),
	CanvasPoint(rand()%WIDTH, rand()%HEIGHT, (rand()%100)-50)
	);
	// std::cout << triangle << std::endl;
	return triangle;
}

// Draws a stroked triangle with colour c to the window
void drawStrokedTriangle(CanvasTriangle &triangle, const Colour& c, DrawingWindow &window) {
	const auto p0 = glm::vec2(triangle.v0().x, triangle.v0().y);
	const auto p1 = glm::vec2(triangle.v1().x, triangle.v1().y);
	const auto p2 = glm::vec2(triangle.v2().x, triangle.v2().y);
	drawLine(p0.x, p0.y, p1.x, p1.y,c,window);
	drawLine(p1.x, p1.y, p2.x, p2.y,c,window);
	drawLine(p0.x, p0.y, p2.x, p2.y,c,window);
}

// Calculate barycentric coords from a vec3 and a triangle
glm::vec3 baryFromVec3(glm::vec3 point, ModelTriangle triangle) {
	glm::vec3 a = triangle.vertices[0];
	glm::vec3 b = triangle.vertices[1];
	glm::vec3 c = triangle.vertices[2];
	//barycentric co-ords!
	glm::vec3 v0 = b - a, v1 = c - a, v2 = point - a;
	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	float areav = (d11 * d20 - d01 * d21) / denom;
	float areaw = (d00 * d21 - d01 * d20) / denom;
	return {1.0f - areav - areaw, areav, areaw};
}

TexturePoint texturePointFromBary(glm::vec3 coords, const std::array<TexturePoint, 3> &triangle) {
	const TexturePoint& a = triangle[0];
	const TexturePoint& b = triangle[1];
	const TexturePoint& c = triangle[2];
	//simply weight each of a,b,c by the barycentric coords
	float mappedX = a.x * coords[0] + b.x * coords[1] + c.x * coords[2];
	float mappedY = a.y * coords[0] + b.y * coords[1] + c.y * coords[2];
	return {mappedX, mappedY};
}

// Draws a stroked triangle with colour c to the window wrt depths
void drawOccludedStrokedTriangle(CanvasTriangle &triangle, const Colour& c, std::vector<std::vector<float>> *depthBuffer, DrawingWindow &window) {
	const auto p0 = glm::vec3(triangle.v0().x, triangle.v0().y, triangle.v0().depth);
	const auto p1 = glm::vec3(triangle.v1().x, triangle.v1().y, triangle.v1().depth);
	const auto p2 = glm::vec3(triangle.v2().x, triangle.v2().y, triangle.v2().depth);
	drawOccludedLine(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, c, depthBuffer, window);
	drawOccludedLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, c, depthBuffer, window);
	drawOccludedLine(p0.x, p0.y, p0.z, p2.x, p2.y, p2.z, c, depthBuffer, window);
}

// Draws the top or bottom 2 outer stroked lines of a filled triangle half to the window
void drawPartialTriangle(const CanvasPoint &shared, const CanvasPoint &p1, const CanvasPoint &p2, const Colour& c, DrawingWindow &window) {
	drawLine(shared.x, shared.y, p1.x, p1.y,c,window);
	drawLine(shared.x, shared.y, p2.x, p2.y,c,window);
}

// Draws a white stroked filled triangle with a flat top or bottom to the screen with fill colour c to the window
void drawFlatTriangle(CanvasTriangle triangle, const Colour &c, const bool drawOutline, DrawingWindow &window) {
	glm::vec2 shared, p1, p2;
	if (triangle.v0().y == triangle.v1().y) {
		shared = glm::vec2(static_cast<int>(triangle.v2().x), static_cast<int>(triangle.v2().y));
		p1 = glm::vec2(static_cast<int>(triangle.v0().x), static_cast<int>(triangle.v0().y));
		p2 = glm::vec2(static_cast<int>(triangle.v1().x), static_cast<int>(triangle.v1().y));
	} else if (triangle.v0().y == triangle.v2().y) {
		shared = glm::vec2(static_cast<int>(triangle.v1().x), static_cast<int>(triangle.v1().y));
		p1 = glm::vec2(static_cast<int>(triangle.v0().x), static_cast<int>(triangle.v0().y));
		p2 = glm::vec2(static_cast<int>(triangle.v2().x), static_cast<int>(triangle.v2().y));
	} else {
		shared = glm::vec2(static_cast<int>(triangle.v0().x), static_cast<int>(triangle.v0().y));
		p1 = glm::vec2(static_cast<int>(triangle.v1().x), static_cast<int>(triangle.v1().y));
		p2 = glm::vec2(static_cast<int>(triangle.v2().x), static_cast<int>(triangle.v2().y));
	}
	std::vector<glm::vec2> fromPoints = interpv2(shared, p1, std::abs(shared.y-p1.y)+1);
	std::vector<glm::vec2> toPoints = interpv2(shared, p2, std::abs(shared.y-p2.y)+1);
	for (int i=0; i< static_cast<int>(fromPoints.size()); i++) {
		if (static_cast<int>(fromPoints[i].x) != static_cast<int>(toPoints[i].x) || static_cast<int>(fromPoints[i].y) != static_cast<int>(toPoints[i].y)) {
			drawLine(fromPoints[i].x,fromPoints[i].y, toPoints[i].x, toPoints[i].y ,c,window);
		}
	}
	if (drawOutline) {
		drawLine(shared.x, shared.y, p1.x, p1.y, Colour(255,255,255), window);
		drawLine(shared.x, shared.y, p2.x, p2.y, Colour(255,255,255), window);
	}
}

// Draws half of the full Filled Triangle wrt depths
void drawOccludedFlatTriangle(CanvasTriangle triangle, const Colour &c, bool drawOutline, std::vector<std::vector<float>> *depthBuffer, DrawingWindow &window) {
	glm::vec3 shared, p1, p2;
	if (triangle.v0().y == triangle.v1().y) {
		shared = glm::vec3(static_cast<int>(triangle.v2().x), static_cast<int>(triangle.v2().y), triangle.v2().depth);
		p1 = glm::vec3(static_cast<int>(triangle.v0().x), static_cast<int>(triangle.v0().y), triangle.v0().depth);
		p2 = glm::vec3(static_cast<int>(triangle.v1().x), static_cast<int>(triangle.v1().y), triangle.v1().depth);
	} else if (triangle.v0().y == triangle.v2().y) {
		shared = glm::vec3(static_cast<int>(triangle.v1().x), static_cast<int>(triangle.v1().y), triangle.v1().depth);
		p1 = glm::vec3(static_cast<int>(triangle.v0().x), static_cast<int>(triangle.v0().y), triangle.v0().depth);
		p2 = glm::vec3(static_cast<int>(triangle.v2().x), static_cast<int>(triangle.v2().y), triangle.v2().depth);
	} else {
		shared = glm::vec3(static_cast<int>(triangle.v0().x), static_cast<int>(triangle.v0().y), triangle.v0().depth);
		p1 = glm::vec3(static_cast<int>(triangle.v1().x), static_cast<int>(triangle.v1().y), triangle.v1().depth);
		p2 = glm::vec3(static_cast<int>(triangle.v2().x), static_cast<int>(triangle.v2().y), triangle.v2().depth);
	}
	std::vector<glm::vec3> fromPoints = interpv3(shared, p1, std::abs(shared.y-p1.y)+1);
	std::vector<glm::vec3> toPoints = interpv3(shared, p2, std::abs(shared.y-p2.y)+1);
	for (int i=0; i< static_cast<int>(fromPoints.size()); i++) {
		if (static_cast<int>(fromPoints[i].x) != static_cast<int>(toPoints[i].x) || static_cast<int>(fromPoints[i].y) != static_cast<int>(toPoints[i].y)) {
			drawOccludedLine(fromPoints[i].x,fromPoints[i].y, fromPoints[i].z, toPoints[i].x, toPoints[i].y, toPoints[i].z ,c, depthBuffer, window);
		}
	}
	if (drawOutline) {
		drawOccludedLine(shared.x, shared.y, shared.z, p1.x, p1.y, p1.z, Colour(255,255,255), depthBuffer, window);
		drawOccludedLine(shared.x, shared.y, shared.z, p2.x, p2.y, p2.z, Colour(255,255,255), depthBuffer, window);
	}
}

// Draws all the pixels that should be drawn according to their depths
void drawOccludedFilledTriangle(CanvasTriangle triangle, const Colour& c, bool drawOutline, std::vector<std::vector<float>> *depthBuffer, DrawingWindow &window) {
	CanvasPoint point1 = triangle.v0();
	CanvasPoint point2 = triangle.v1();
	CanvasPoint point3 = triangle.v2();
	CanvasPoint point4;
	if ((point1.y < point3.y && point1.y > point2.y) || (point1.y > point3.y && point1.y < point2.y)) {
		// point1 is the middle, so interp 2 and 3 and find x which is == y
		auto otherLine = interpv3(glm::vec3(static_cast<int>(point2.x), static_cast<int>(point2.y), point2.depth), glm::vec3(static_cast<int>(point3.x), static_cast<int>(point3.y), point3.depth), std::abs(point2.x - point3.x) + std::abs(point2.y-point3.y)+1);
		for (auto point: otherLine) {
			if (point1.y == (int) point.y) {
				point4 = CanvasPoint((int) point.x, (int) point.y, point.z);
			}
		}
		CanvasTriangle triangle1 = CanvasTriangle(point2, point1, point4);
		CanvasTriangle triangle2 = CanvasTriangle(point3, point1, point4);
		drawOccludedFlatTriangle(triangle1, c, drawOutline, depthBuffer, window);
		drawOccludedFlatTriangle(triangle2, c, drawOutline, depthBuffer, window);
	} else if ((point2.y < point1.y && point2.y > point3.y) || (point2.y > point1.y && point2.y < point3.y)) {
		//point2 is the middle point, so interp 1 and 3 and find x which is == y
		auto otherLine = interpv3(glm::vec3(static_cast<int>(point1.x), static_cast<int>(point1.y), point1.depth), glm::vec3(static_cast<int>(point3.x), static_cast<int>(point3.y), point3.depth), std::abs(point1.x - point3.x)+std::abs(point1.y-point3.y)+1);
		for (auto point: otherLine) {
			if (point2.y == (int) point.y) {
				point4 = CanvasPoint((int) point.x, (int) point.y,  point.z);
			}
		}
		CanvasTriangle triangle1 = CanvasTriangle(point1, point2, point4);
		CanvasTriangle triangle2 = CanvasTriangle(point3, point2, point4);
		drawOccludedFlatTriangle(triangle1, c, drawOutline, depthBuffer, window);
		drawOccludedFlatTriangle(triangle2, c, drawOutline, depthBuffer, window);
	} else if ((point3.y < point1.y && point3.y > point2.y) || (point3.y > point1.y && point3.y < point2.y)) {
		//point3 is the middle point, so interp 1 and 2 and find x which is == y
		auto otherLine = interpv3(glm::vec3(static_cast<int>(point1.x), static_cast<int>(point1.y), point1.depth), glm::vec3(static_cast<int>(point2.x), static_cast<int>(point2.y), point2.depth), std::abs(point1.x - point2.x)+std::abs(point1.y-point2.y)+1);
		for (auto point: otherLine) {
			if (point3.y == (int) point.y) {
				point4 = CanvasPoint((int) point.x, (int) point.y,  point.z);
			}
		}
		CanvasTriangle triangle1 = CanvasTriangle(point1, point3, point4);
		CanvasTriangle triangle2 = CanvasTriangle(point2, point3, point4);
		drawOccludedFlatTriangle(triangle1, c, drawOutline, depthBuffer, window);
		drawOccludedFlatTriangle(triangle2, c, drawOutline, depthBuffer, window);
	} else {
		//we have a flat triangle
		drawOccludedFlatTriangle(triangle, c, drawOutline, depthBuffer, window);
		if (drawOutline) {
			drawOccludedStrokedTriangle(triangle, Colour(255,255,255), depthBuffer, window);
		}
	}
}

// Draws a white stroked filled triangle to the screen with fill colour c
void drawFilledTriangle(CanvasTriangle triangle, const Colour& c, bool drawOutline, DrawingWindow &window) {
	CanvasPoint point1 = triangle.v0();
	CanvasPoint point2 = triangle.v1();
	CanvasPoint point3 = triangle.v2();
	CanvasPoint point4;
	if ((point1.y < point3.y && point1.y > point2.y) || (point1.y > point3.y && point1.y < point2.y)) {
		// point1 is the middle, so interp 2 and 3 and find x which is == y
		auto otherLine = interp(glm::vec2(static_cast<int>(point2.x), static_cast<int>(point2.y)), glm::vec2(static_cast<int>(point3.x), static_cast<int>(point3.y)), std::max(std::abs(point2.x - point3.x)+1, std::abs(point2.y-point3.y))+1);
		for (auto point: otherLine) {
			if (point1.y == (int) point.y) {
				point4 = CanvasPoint((int) point.x, (int) point.y);
			}
		}
		CanvasTriangle triangle1 = CanvasTriangle(point2, point1, point4);
		CanvasTriangle triangle2 = CanvasTriangle(point3, point1, point4);
		drawFlatTriangle(triangle1, c, drawOutline, window);
		drawFlatTriangle(triangle2, c, drawOutline, window);
		// if (drawOutline) {
		// 	drawPartialTriangle(point2, point1, point4, Colour(255,255,255), window);
		// 	drawPartialTriangle(point3, point1, point4, Colour(255,255,255), window);
		// }
	} else if ((point2.y < point1.y && point2.y > point3.y) || (point2.y > point1.y && point2.y < point3.y)) {
		//point2 is the middle point, so interp 1 and 3 and find x which is == y
		auto otherLine = interp(glm::vec2(static_cast<int>(point1.x), static_cast<int>(point1.y)), glm::vec2(static_cast<int>(point3.x), static_cast<int>(point3.y)), std::max(std::abs(point1.x - point3.x)+1, std::abs(point1.y-point3.y))+1);
		for (auto point: otherLine) {
			if (point2.y == (int) point.y) {
				point4 = CanvasPoint((int) point.x, (int) point.y);
			}
		}
		CanvasTriangle triangle1 = CanvasTriangle(point1, point2, point4);
		CanvasTriangle triangle2 = CanvasTriangle(point3, point2, point4);
		drawFlatTriangle(triangle1, c, drawOutline, window);
		drawFlatTriangle(triangle2, c, drawOutline, window);
		// if (drawOutline) {
		// 	drawPartialTriangle(point1, point2, point4, Colour(255,255,255), window);
		// 	drawPartialTriangle(point3, point2, point4, Colour(255,255,255), window);
		// }
	} else if ((point3.y < point1.y && point3.y > point2.y) || (point3.y > point1.y && point3.y < point2.y)) {
		//point3 is the middle point, so interp 1 and 2 and find x which is == y
		auto otherLine = interp(glm::vec2(static_cast<int>(point1.x), static_cast<int>(point1.y)), glm::vec2(static_cast<int>(point2.x), static_cast<int>(point2.y)), std::max(std::abs(point1.x - point2.x)+1, std::abs(point1.y-point2.y))+1);
		for (auto point: otherLine) {
			if (point3.y == (int) point.y) {
				point4 = CanvasPoint((int) point.x, (int) point.y);
			}
		}
		CanvasTriangle triangle1 = CanvasTriangle(point1, point3, point4);
		CanvasTriangle triangle2 = CanvasTriangle(point2, point3, point4);
		drawFlatTriangle(triangle1, c, drawOutline, window);
		drawFlatTriangle(triangle2, c, drawOutline, window);
		// if (drawOutline) {
		// 	drawPartialTriangle(point1, point3, point4, Colour(255,255,255), window);
		// 	drawPartialTriangle(point2, point3, point4, Colour(255,255,255), window);
		// }
	} else {
		//we have a flat triangle
		drawFlatTriangle(triangle, c, drawOutline, window);
		// if (drawOutline) {
		// 	drawStrokedTriangle(triangle, Colour(255,255,255), window);
		// }
	}
}

// Draws a line with colours according to the texture
void drawTexturedLine(const int x1, const int y1, const int x2, const int y2, const int tx1, const int ty1, const int tx2, const int ty2, const std::vector<std::vector<TexturePoint>> &texture, DrawingWindow &window) {
	glm::vec2 imageStart = glm::vec2(x1, y1);
	glm::vec2 imageEnd = glm::vec2(x2, y2);
	glm::vec2 textureStart = glm::vec2(tx1, ty1);
	glm::vec2 textureEnd = glm::vec2(tx2, ty2);
	auto texturePoints = interpv2(textureStart, textureEnd, std::abs(imageStart.x - imageEnd.x) + std::abs(imageStart.y-imageEnd.y)+1);
	auto imagePoints = interpv2(imageStart, imageEnd, std::abs(imageStart.x - imageEnd.x) + std::abs(imageStart.y-imageEnd.y)+1);
	std::vector<Colour> textureColours;
	textureColours.reserve(texturePoints.size());
	for (auto tp : texturePoints) {
		textureColours.push_back(texture[tp.y][tp.x].colour);
	}
	for (int i=0; i<static_cast<int>(imagePoints.size()); i++) {
		// std::cout << textureColours[i] << std::endl;
		window.setPixelColour(static_cast<int>(imagePoints[i].x), static_cast<int>(imagePoints[i].y),textureColours[i].asARGB());
	}
}

// Draws a Textured Triangle to the window
void drawFlatTexturedTriangle(CanvasTriangle imageTriangle, const std::vector<std::vector<TexturePoint>> &texture, CanvasTriangle textureTriangle, DrawingWindow &window) {
	glm::vec2 iShared, ip1, ip2;
	glm::vec2 tShared, tp1, tp2;
	if (imageTriangle.v0().y == imageTriangle.v1().y) {
		iShared = glm::vec2(static_cast<int>(imageTriangle.v2().x), static_cast<int>(imageTriangle.v2().y));
		tShared = glm::vec2(static_cast<int>(textureTriangle.v2().x), static_cast<int>(textureTriangle.v2().y));
		ip1 = glm::vec2(static_cast<int>(imageTriangle.v0().x), static_cast<int>(imageTriangle.v0().y));
		tp1 = glm::vec2(static_cast<int>(textureTriangle.v0().x), static_cast<int>(textureTriangle.v0().y));
		ip2 = glm::vec2(static_cast<int>(imageTriangle.v1().x), static_cast<int>(imageTriangle.v1().y));
		tp2 = glm::vec2(static_cast<int>(textureTriangle.v1().x), static_cast<int>(textureTriangle.v1().y));
	} else if (imageTriangle.v0().y == imageTriangle.v2().y) {
		iShared = glm::vec2(static_cast<int>(imageTriangle.v1().x), static_cast<int>(imageTriangle.v1().y));
		tShared = glm::vec2(static_cast<int>(textureTriangle.v1().x), static_cast<int>(textureTriangle.v1().y));
		ip1 = glm::vec2(static_cast<int>(imageTriangle.v0().x), static_cast<int>(imageTriangle.v0().y));
		tp1 = glm::vec2(static_cast<int>(textureTriangle.v0().x), static_cast<int>(textureTriangle.v0().y));
		ip2 = glm::vec2(static_cast<int>(imageTriangle.v2().x), static_cast<int>(imageTriangle.v2().y));
		tp2 = glm::vec2(static_cast<int>(textureTriangle.v2().x), static_cast<int>(textureTriangle.v2().y));
	} else {
		iShared = glm::vec2(static_cast<int>(imageTriangle.v0().x), static_cast<int>(imageTriangle.v0().y));
		tShared = glm::vec2(static_cast<int>(textureTriangle.v0().x), static_cast<int>(textureTriangle.v0().y));
		tp1 = glm::vec2(static_cast<int>(textureTriangle.v1().x), static_cast<int>(textureTriangle.v0().y));
		ip1 = glm::vec2(static_cast<int>(imageTriangle.v1().x), static_cast<int>(imageTriangle.v1().y));
		tp2 = glm::vec2(static_cast<int>(textureTriangle.v2().x), static_cast<int>(textureTriangle.v2().y));
		ip2 = glm::vec2(static_cast<int>(imageTriangle.v2().x), static_cast<int>(imageTriangle.v2().y));
	}
	std::vector<glm::vec2> imageFromPoints = interpv2(iShared, ip1, std::abs(iShared.y-ip1.y)+1);
	std::vector<glm::vec2> imageToPoints = interpv2(iShared, ip2, std::abs(iShared.y-ip2.y)+1);
	std::vector<glm::vec2> textureFromPoints = interpv2(tShared, tp1, std::abs(iShared.y-ip2.y)+1);
	std::vector<glm::vec2> textureToPoints = interpv2(tShared, tp2, std::abs(iShared.y-ip2.y)+1);

	std::vector<Colour> textureColours;
	for (int i=0; i< static_cast<int>(imageFromPoints.size()); i++) {
		if (static_cast<int>(imageFromPoints[i].x) != static_cast<int>(imageToPoints[i].x) || static_cast<int>(imageFromPoints[i].y) != static_cast<int>(imageToPoints[i].y)) {
			//If there is space to interpolate between the from and to:
			drawTexturedLine(imageFromPoints[i].x,imageFromPoints[i].y, imageToPoints[i].x, imageToPoints[i].y, textureFromPoints[i].x, textureFromPoints[i].y, textureToPoints[i].x, textureToPoints[i].y, texture, window);
		}
	}
	drawLine(iShared.x, iShared.y, ip1.x, ip1.y, Colour(255,255,255), window);
	drawLine(iShared.x, iShared.y, ip2.x, ip2.y, Colour(255,255,255), window);
}

// Converts a TextureMap to a 2D vector of points with associated colours
std::vector<std::vector<TexturePoint>> loadTexture(const TextureMap &texture) {
	std::cout << "Loading image: " << texture.width << " by " << texture.height << std::endl;
	std::vector<std::vector<TexturePoint>> outVector;
	std::vector<TexturePoint> smallOutVector;

	for (int y=0; y<static_cast<int>(texture.height); y++) {
		for (int x=0; x<static_cast<int>(texture.width); x++) {
			if (y==0) {
				smallOutVector.push_back(TexturePoint(x, y, Colour((texture.pixels[y*texture.width+x]&0xFF0000)>>16, (texture.pixels[y*texture.width+x]&0xFF00)>>8, texture.pixels[y*texture.width+x]&0xFF)));
			} else {
				smallOutVector[x] = (TexturePoint(x, y, Colour(((texture.pixels[y*texture.width+x]&0xFF0000)>>16), ((texture.pixels[y*texture.width+x]&0xFF00)>>8), texture.pixels[y*texture.width+x]&0xFF)));
			}
		}
		outVector.push_back(smallOutVector);
	}
	return outVector;
}

// Draws the texture to the window
void drawTexture(const TextureMap &texture, DrawingWindow &window) {
	auto texturePoints = loadTexture(texture);
	for (int y=0; y<texture.height; y++){
		for (int x=0; x<texture.width; x++) {
			window.setPixelColour(x, y, texturePoints[y][x].colour.asARGB());
		}
	}
}

//TODO: fix this
void drawTexturedTriangle(CanvasTriangle imageTriangle, const std::vector<std::vector<TexturePoint>>& texture, CanvasTriangle textureTriangle, DrawingWindow &window) {
	drawStrokedTriangle(imageTriangle, Colour(255,255,255), window);
	CanvasPoint ipoint1 = imageTriangle.v0();
	CanvasPoint tpoint1 = textureTriangle.v0();
	CanvasPoint ipoint2 = imageTriangle.v1();
	CanvasPoint tpoint2 = textureTriangle.v1();
	CanvasPoint ipoint3 = imageTriangle.v2();
	CanvasPoint tpoint3 = textureTriangle.v2();
	CanvasPoint ipoint4;
	CanvasPoint tpoint4;
	Colour c = {255,255,255};
	if ((ipoint1.y < ipoint3.y && ipoint1.y > ipoint2.y) || (ipoint1.y > ipoint3.y && ipoint1.y < ipoint2.y)) {
		// point1 is the middle, so interp 2 and 3 and find x which is == y
		auto iotherLine = interp(glm::vec2(static_cast<int>(ipoint2.x), static_cast<int>(ipoint2.y)), glm::vec2(static_cast<int>(ipoint3.x), static_cast<int>(ipoint3.y)), std::max(std::abs(ipoint2.x - ipoint3.x)+1, std::abs(ipoint2.y-ipoint3.y))+1);
		auto totherLine = interp(glm::vec2(static_cast<int>(tpoint2.x), static_cast<int>(tpoint2.y)), glm::vec2(static_cast<int>(tpoint3.x), static_cast<int>(tpoint3.y)), std::max(std::abs(ipoint2.x - ipoint3.x)+1, std::abs(ipoint2.y-ipoint3.y))+1);
		for (auto ipoint: iotherLine) {
			if (ipoint1.y == (int) ipoint.y) {
				ipoint4 = CanvasPoint((int) ipoint.x, (int) ipoint.y);
			}
		}
		// find proportion of way down the triangle
		// top-middle/top-bottom is the proportion
		// so the point4 will be proportion along totherLine
		float proportion = std::abs(ipoint2.y-ipoint4.y)/std::abs(ipoint2.y-ipoint3.y);
		auto tIndex = proportion * (std::max(std::abs(ipoint2.x - ipoint3.x)+1, std::abs(ipoint2.y-ipoint3.y))+1);
		tpoint4 = CanvasPoint(totherLine[static_cast<int>(tIndex)].x, totherLine[static_cast<int>(tIndex)].y);
		//make an interp list of all the points along the otherline, then multiply the numberOfValues by (top.y-middle.y)/(top.y-bottom.y) to get the position in outlist that you want
		CanvasTriangle itriangle1 = CanvasTriangle(ipoint2, ipoint1, ipoint4);
		CanvasTriangle itriangle2 = CanvasTriangle(ipoint3, ipoint1, ipoint4);
		CanvasTriangle ttriangle1 = CanvasTriangle(tpoint2, tpoint1, tpoint4);
		CanvasTriangle ttriangle2 = CanvasTriangle(tpoint3, tpoint1, tpoint4);
		drawFlatTexturedTriangle(itriangle1, texture, ttriangle1, window);
		drawFlatTexturedTriangle(itriangle2, texture, ttriangle2, window);
		drawPartialTriangle(ipoint2, ipoint1, ipoint4, Colour(255,255,255), window);
		drawPartialTriangle(ipoint3, ipoint1, ipoint4, Colour(255,255,255), window);
	} else if ((ipoint2.y < ipoint1.y && ipoint2.y > ipoint3.y) || (ipoint2.y > ipoint1.y && ipoint2.y < ipoint3.y)) {
		//point2 is the middle point, so interp 1 and 3 and find x which is == y

		auto otherLine = interp(glm::vec2(static_cast<int>(ipoint1.x), static_cast<int>(ipoint1.y)), glm::vec2(static_cast<int>(ipoint3.x), static_cast<int>(ipoint3.y)), std::max(std::abs(ipoint1.x - ipoint3.x)+1, std::abs(ipoint1.y-ipoint3.y))+1);
		auto totherLine = interp(glm::vec2(static_cast<int>(tpoint1.x), static_cast<int>(tpoint1.y)), glm::vec2(static_cast<int>(tpoint3.x), static_cast<int>(tpoint3.y)), std::max(std::abs(ipoint1.x - ipoint3.x)+1, std::abs(ipoint1.y-ipoint3.y))+1);
		for (auto point: otherLine) {
			if (ipoint2.y == (int) point.y) {
				ipoint4 = CanvasPoint((int) point.x, (int) point.y);
			}
		}
		float proportion = std::abs(ipoint1.y-ipoint4.y)/std::abs(ipoint1.y-ipoint3.y);
		auto tIndex = proportion * (std::max(std::abs(ipoint1.x - ipoint3.x)+1, std::abs(ipoint1.y-ipoint3.y))+1);
		tpoint4 = CanvasPoint(totherLine[static_cast<int>(tIndex)].x, totherLine[static_cast<int>(tIndex)].y);
		CanvasTriangle itriangle1 = CanvasTriangle(ipoint1, ipoint2, ipoint4);
		CanvasTriangle itriangle2 = CanvasTriangle(ipoint3, ipoint2, ipoint4);
		CanvasTriangle ttriangle1 = CanvasTriangle(tpoint1, tpoint2, tpoint4);
		CanvasTriangle ttriangle2 = CanvasTriangle(tpoint3, tpoint2, tpoint4);
		drawFlatTexturedTriangle(itriangle1, texture, ttriangle1, window);
		drawFlatTexturedTriangle(itriangle2, texture, ttriangle2, window);
		drawPartialTriangle(ipoint1, ipoint2, ipoint4, Colour(255,255,255), window);
		drawPartialTriangle(ipoint3, ipoint2, ipoint4, Colour(255,255,255), window);
	} else if ((ipoint3.y < ipoint1.y && ipoint3.y > ipoint2.y) || (ipoint3.y > ipoint1.y && ipoint3.y < ipoint2.y)) {
		//point3 is the middle point, so interp 1 and 2 and find x which is == y

		auto otherLine = interpv2(glm::vec2(static_cast<int>(ipoint1.x), static_cast<int>(ipoint1.y)), glm::vec2(static_cast<int>(ipoint2.x), static_cast<int>(ipoint2.y)), std::abs(ipoint1.x - ipoint2.x)+std::abs(ipoint1.y-ipoint2.y)+1);
		auto totherLine = interpv2(glm::vec2(static_cast<int>(tpoint1.x), static_cast<int>(tpoint1.y)), glm::vec2(static_cast<int>(tpoint2.x), static_cast<int>(tpoint2.y)), std::abs(ipoint1.x - ipoint2.x)+std::abs(ipoint1.y-ipoint2.y)+1);
		for (auto point: otherLine) {
			if (ipoint3.y == (int) point.y) {
				ipoint4 = CanvasPoint((int) point.x, (int) point.y);
			}
		}
		float proportion = std::abs(ipoint1.y-ipoint4.y)/std::abs(ipoint1.y-ipoint2.y);
		auto tIndex = proportion * (std::max(std::abs(ipoint2.x - ipoint3.x)+1, std::abs(ipoint2.y-ipoint3.y))+1);

		tpoint4 = CanvasPoint(totherLine[static_cast<int>(tIndex)].x, totherLine[static_cast<int>(tIndex)].y);
		CanvasTriangle itriangle1 = CanvasTriangle(ipoint1, ipoint3, ipoint4);
		CanvasTriangle itriangle2 = CanvasTriangle(ipoint2, ipoint3, ipoint4);
		CanvasTriangle ttriangle1 = CanvasTriangle(tpoint1, tpoint3, tpoint4);
		CanvasTriangle ttriangle2 = CanvasTriangle(tpoint2, tpoint3, tpoint4);
		drawFlatTexturedTriangle(itriangle1, texture, ttriangle1, window);
		drawFlatTexturedTriangle(itriangle2, texture, ttriangle2, window);
		drawPartialTriangle(ipoint1, ipoint3, ipoint4, Colour(255,255,255), window);
		drawPartialTriangle(ipoint2, ipoint3, ipoint4, Colour(255,255,255), window);
	} else {
		//we have a flat triangle
		drawFlatTexturedTriangle(imageTriangle, texture, textureTriangle, window);
		drawStrokedTriangle(imageTriangle, Colour(255,255,255), window);
	}
}

// Returns a vector of ModelTriangles that represent the triangles in the OBJ file
std::vector<ModelTriangle> parseOBJ(const std::string& filename, const float scalingParameter) {
	std::ifstream theOBJ(filename);
	std::string line;
	std::string vectorDelimiter = " ";
	std::string faceDelimiter = "/";
	std::vector<ModelTriangle> outVector;
	std::vector<glm::vec3> vertices;
	std::vector<glm::ivec3> faces;
	Colour currentColour;
	while(theOBJ.eof() == false) {
		getline(theOBJ, line);
		if(line[0] == 'u') {
			line = line.substr(line.find(" ")+1,line.length());
			if (line == "White") {
				currentColour = Colour(255,255,255);
			} else if (line == "Grey") {
				currentColour = Colour(125,125,125);
			} else if (line == "Cyan") {
				currentColour = Colour(0,255,255);
			} else if (line == "Green") {
				currentColour = Colour(0,255,0);
			} else if (line == "Magenta") {
				currentColour = Colour(255,0,255);
			} else if (line == "Yellow") {
				currentColour = Colour(255,255,0);
			} else if (line == "Red") {
				currentColour = Colour(255,0,0);
			} else if (line == "Blue") {
				currentColour = Colour(0,0,255);
			} else {
				currentColour = Colour(0,0,0);
			}
		} else if (line[0] == 'v') {
			line = line.substr(2, line.size());
			// std::cout << line << std::endl;
			auto v1 = line.substr(0, line.find(vectorDelimiter));
			line = line.substr(line.find(vectorDelimiter) + 1, line.size());
			auto v2 = line.substr(0, line.find(vectorDelimiter));
			line = line.substr(line.find(vectorDelimiter) + 1, line.size());
			auto v3 = line.substr(0, line.find(vectorDelimiter));
			// std::cout << v1 << " one  " << v2 << " two  " << v3 << " three" <<std::endl;
			vertices.push_back(glm::vec3(std::stof(v1), std::stof(v2), std::stof(v3)));
		} else if (line[0] == 'f') {
			line = line.substr(2, line.size());
			// std::cout << line << std::endl;
			auto v1 = line.substr(0, line.find(faceDelimiter));
			line = line.substr(line.find(faceDelimiter) + 2, line.size());
			auto v2 = line.substr(0, line.find(faceDelimiter));
			line = line.substr(line.find(faceDelimiter) + 2, line.size());
			auto v3 = line.substr(0, line.find(faceDelimiter));
			// std::cout << v1 << " one  "<< v2 << " two  "  << v3 << " three" << std::endl;
			ModelTriangle shwoop = {vertices[std::stoi(v1)-1]*scalingParameter, vertices[std::stoi(v2)-1]*scalingParameter, vertices[std::stoi(v3)-1]*scalingParameter, currentColour};
			outVector.emplace_back(shwoop);
		}
	}
	return outVector;
}

glm::vec3 calculateVertexNormal(const glm::vec3 vertex, const std::vector<ModelTriangle>& triangles) {
	std::vector<glm::vec3> normals;
	for (auto triangle: triangles) {
		if (vertex == triangle.vertices[0] || vertex == triangle.vertices[1] || vertex == triangle.vertices[2]) {
			normals.push_back(triangle.normal);
		}
	}
	glm::vec3 sum = {0,0,0};
	for (auto item : normals) {
		sum += item;
	}
	return normalize(sum);
}

std::vector<ModelTriangle> debugParseOBJ(const std::string& filename, const Light &light, const std::vector<std::vector<TexturePoint>> &texture, const float scalingParameter) {
	std::ifstream theOBJ(filename);
	std::string line;
	std::string vectorDelimiter = " ";
	std::string faceDelimiter = "/";
	std::vector<ModelTriangle> outVector;
	std::vector<glm::vec3> vertices;
	std::vector<glm::ivec3> faces;
	int numTriangles = 0;

	Colour currentColour;
	while(theOBJ.eof() == false) {
		getline(theOBJ, line);
		if(line[0] == 'u') {
			line = line.substr(line.find(" ")+1,line.length());
			if (line == "White") {
				currentColour = Colour(255,255,255);
			} else if (line == "Grey") {
				currentColour = Colour(125,125,125);
			} else if (line == "Cyan") {
				currentColour = Colour(0,255,255);
			} else if (line == "Green") {
				currentColour = Colour(0,255,0);
			} else if (line == "Magenta") {
				currentColour = Colour(255,0,255);
			} else if (line == "Yellow") {
				currentColour = Colour(255,255,0);
			} else if (line == "Red") {
				currentColour = Colour(255,0,0);
			} else if (line == "Blue") {
				currentColour = Colour(0,0,255);
			} else {
				currentColour = Colour(0,0,0);
			}
		} else if (line[0] == 'v') {
			line = line.substr(2, line.size());
			auto v1 = line.substr(0, line.find(vectorDelimiter));
			line = line.substr(line.find(vectorDelimiter) + 1, line.size());
			auto v2 = line.substr(0, line.find(vectorDelimiter));
			line = line.substr(line.find(vectorDelimiter) + 1, line.size());
			auto v3 = line.substr(0, line.find(vectorDelimiter));
			vertices.push_back(glm::vec3(std::stof(v1), std::stof(v2), std::stof(v3)));
		} else if (line[0] == 'f') {
			line = line.substr(2, line.size());
			auto v1 = line.substr(0, line.find(faceDelimiter));
			line = line.substr(line.find(faceDelimiter) + 2, line.size());
			auto v2 = line.substr(0, line.find(faceDelimiter));
			line = line.substr(line.find(faceDelimiter) + 2, line.size());
			auto v3 = line.substr(0, line.find(faceDelimiter));
			ModelTriangle shwoop = {vertices[std::stoi(v1)-1]*scalingParameter, vertices[std::stoi(v2)-1]*scalingParameter, vertices[std::stoi(v3)-1]*scalingParameter, currentColour};
			numTriangles++;
			// glm::vec3 average = (shwoop.vertices[0] + shwoop.vertices[1] + shwoop.vertices[2])/3;
			// ModelTriangle normViewer = ModelTriangle(average, average+shwoop.normal*0.1, average+shwoop.normal*0.05, Colour(255,127,127));
			// outVector.emplace_back(normViewer);

			outVector.emplace_back(shwoop);
		}
	}
	int texturedThings = 0;
	auto width = texture[0].size();
	auto height = texture.size();
	std::array<TexturePoint, 3> texturePoints1 = {TexturePoint(0, static_cast<float>(height)), TexturePoint(static_cast<float>(width),0), TexturePoint(0,0)};
	std::array<TexturePoint, 3> texturePoints2 = {TexturePoint(0, static_cast<float>(height)), TexturePoint(width, static_cast<float>(height)), TexturePoint(static_cast<float>(width),0)};
	// std::array<TexturePoint, 3> texturePoints2 = {TexturePoint(0,0), TexturePoint(0,0), TexturePoint(0,0)};
	std::array<std::array<TexturePoint, 3>, 2> shoople = {texturePoints1, texturePoints2};
	int j=0;

	for (int i=0; i<numTriangles; i++) {
		if (outVector[i].colour == Colour(0,255,0)) {
			std::cout << "finds green triangle" << std::endl;
			outVector[i].texturePoints = shoople[j];
			outVector[i].textured = true;
			j++;
		}
	}
	// std::vector<ModelTriangle> lights;
	// std::vector<ModelTriangle> vertnorms;
	// for (auto triangle: outVector) {
	// 	for (auto vertex : triangle.vertices) {
	// 		auto norm = calculateVertexNormal(vertex, outVector);
	// 		ModelTriangle vertNorm = ModelTriangle(vertex, vertex+norm*0.1 , vertex+norm*0.05, Colour(255,255,255));
	// 		ModelTriangle toLight = ModelTriangle(vertex, vertex + 0.1 * normalize(light.position - vertex),vertex + 0.05 * normalize(light.position - vertex), Colour(255,255,127));
	// 		lights.emplace_back(toLight);
	// 		vertnorms.emplace_back(vertNorm);
	// 	}
	// }
	// for (auto vertnorm: vertnorms) {
	// 	outVector.emplace_back(vertnorm);
	// }
	// for (auto lightvec: lights) {
	// 	outVector.emplace_back(lightvec);
	// }
	return outVector;
}

// Convert vertexPosition to CanvasPoint relative to the cameraPosition
CanvasPoint projectVertexOntoCanvasPoint(Camera *camera, const glm::vec3 vertexPosition, const float scalingFactor) {
	auto tVP = vertexPosition - camera->position;
	tVP = camera->orientation*tVP;
	float u = static_cast<int>(camera->focalLength * -tVP.x/tVP.z * scalingFactor + WIDTH/2);
	float v = static_cast<int>(camera->focalLength * tVP.y/tVP.z * scalingFactor + HEIGHT/2);
	return {u, v, -1/tVP.z};
}

// Return a pointer to a new depthBuffer object
std::vector<std::vector<float>> *newDepthBuffer() {
	std::vector<std::vector<float>> *depthBuffer = new std::vector<std::vector<float>>;
	std::vector<float> smol;
	for (int x = 0; x < WIDTH; ++x) {
		smol.push_back(-100);
	}
	for (int y = 0; y < HEIGHT; ++y) {
		depthBuffer->emplace_back(smol);
	}
	return depthBuffer;
}

// Render the .obj file in the window
void drawOBJ(Camera *camera, const float scalingFactor, std::vector<std::vector<float>> *depthBuffer, const std::vector<ModelTriangle>& triangles, DrawingWindow &window) {
	CanvasTriangle renderTriangle;
// #pragma omp parallel for
	for (auto triangle: triangles) {
		renderTriangle = {
			projectVertexOntoCanvasPoint(camera, triangle.vertices[0], scalingFactor),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[1], scalingFactor),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[2], scalingFactor)
		};
		drawOccludedFilledTriangle(renderTriangle, triangle.colour, false, depthBuffer, window);
	}
}

// Returns the closest triangle to the camera wrt a ray from the camera.
std::pair<ModelTriangle, glm::vec3> getClosestIntersection(glm::vec3 fromPoint, glm::vec3 direction, const std::vector<ModelTriangle>& sceneTriangles) {
	glm::vec3 closestSoFar = {MAXFLOAT, MAXFLOAT, MAXFLOAT};
	direction = direction * glm::mat3(glm::vec3(-1,0,0), glm::vec3(0,-1,0), glm::vec3(0,0,1));
	ModelTriangle outTriangle;
	for (ModelTriangle triangle: sceneTriangles) {
		glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
		glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
		glm::vec3 SPVector = fromPoint - triangle.vertices[0];
		glm::mat3 DEMatrix = {-direction, e0, e1};
		glm::vec3 possibleSolution = inverse(DEMatrix) * SPVector;
		glm::vec3 point = triangle.vertices[0] + possibleSolution.y*e0 + possibleSolution.z*e1;
		if (possibleSolution.y >= 0.0 && possibleSolution.y <= 1.0 && possibleSolution.z >= 0.0 && possibleSolution.z <= 1.0 && possibleSolution.y + possibleSolution.z <= 1.0 && possibleSolution.x <= 0){
			if (length(point - fromPoint) < length(closestSoFar - fromPoint )) {
				closestSoFar = point;
				outTriangle = triangle;
			}
		}
	}
	return std::pair<ModelTriangle, glm::vec3>(outTriangle, closestSoFar);
}

float calculateProximityLighting(std::pair<ModelTriangle, glm::vec3> res, glm::vec3 point, const Light light, const std::vector<ModelTriangle> &triangles) {
	float out = length(point - light.position) * length(point - light.position);
	if (length(point - res.second) > 0.0000001) {
		if (length(point - res.second) < length(point - light.position)) {
			out = (out * 4);
		}
	}
	out = 1/out;
	float result = std::min(1.0f, out);
	result = std::max(0.0f, result);
	return result;
}

float calculateDiffuseLighting(std::pair<ModelTriangle, glm::vec3> res, Camera *camera, glm::vec3 point, Light light, const std::vector<ModelTriangle> &triangles) {
	auto normal = res.first.normal;
	if (dot(camera->position - point, normal) < 0){
		return 0;
	}
	float out = dot(normalize(light.position - res.second), normal);
	if (length(point - res.second) > 0.01) {
		if (length(point - res.second) < length(point - light.position)) {
			out = 0.2;
		}
	}
	float result = std::min(1.0f, out);
	result = std::max(0.0f, result);
	return result;
}

float calculateDiffuseReflectionLighting(std::pair<ModelTriangle, glm::vec3> res, glm::vec3 from, glm::vec3 point, Light light, const std::vector<ModelTriangle> &triangles) {
	auto normal = res.first.normal;
	if (dot(from - point, normal) < 0){
		return 0;
	}
	float out = dot(normalize(light.position - res.second), normal);
	if (length(point - res.second) > 0.01) {
		if (length(point - res.second) < length(point - light.position)) {
			out = 0.2;
		}
	}
	float result = std::min(1.0f, out);
	result = std::max(0.0f, result);
	return result;
}

float calculateNormalDiffuseLighting(std::pair<ModelTriangle, glm::vec3> res, glm::vec3 point, glm::vec3 normal, Light light, const std::vector<ModelTriangle> &triangles) {
	float out = dot(normalize(light.position - res.second), normal);
	if (length(point - res.second) > 0.01) {
		if (length(point - res.second) < length(point - light.position)) {
			out = 0.2;
		}
	}
	float result = std::min(1.0f, out);
	result = std::max(0.0f, result);
	return result;
}


float calculateSpecularLighting(std::pair<ModelTriangle, glm::vec3> res, Camera *camera, glm::vec3 point, Light light, int power, const std::vector<ModelTriangle> &triangles) {
	auto normal = res.first.normal;
	if (dot(camera->position - point, normal) < 0){
		return 0;
	}
	glm::vec3 Ri = normalize(point - light.position);
	glm::vec3 Rr = Ri - 2*res.first.normal*dot(Ri, res.first.normal);
	float out = std::pow(dot(normalize(camera->position - point), Rr), std::pow(2, power));
	if (length(point - res.second) > 0.01) {
		if (length(point - res.second) < length(point - light.position)) {
			out = 0;
		}
	}
	float result = std::min(1.0f, out);
	result = std::max(0.0f, result);
	return result;
}

float calculateSpecularReflectionLighting(std::pair<ModelTriangle, glm::vec3> res, glm::vec3 from, glm::vec3 point, Light light, int power, const std::vector<ModelTriangle> &triangles) {
	auto normal = res.first.normal;
	if (dot(from - point, normal) < 0){
		return 0;
	}
	glm::vec3 Ri = normalize(point - light.position);
	glm::vec3 Rr = Ri - 2*res.first.normal*dot(Ri, res.first.normal);
	float out = std::pow(dot(normalize(from - point), Rr), std::pow(2, power));
	if (length(point - res.second) > 0.01) {
		if (length(point - res.second) < length(point - light.position)) {
			out = 0;
		}
	}
	float result = std::min(1.0f, out);
	result = std::max(0.0f, result);
	return result;
}

float calculateNormalSpecularLighting(std::pair<ModelTriangle, glm::vec3> res, Camera *camera, glm::vec3 point, glm::vec3 normal, Light light, int power, const std::vector<ModelTriangle> &triangles) {
	glm::vec3 Ri = normalize(point - light.position);
	if (dot(camera->position - point, normal) < 0) {
		return 0;
	}
	glm::vec3 Rr = Ri - 2*normal*dot(Ri, normal);
	float out = std::pow(dot(normalize(camera->position - point), Rr), std::pow(2, power));
	if (length(point - res.second) > 0.01) {
		if (length(point - res.second) < length(point - light.position)) {
			out = 0;
		}
	}
	float result = std::min(1.0f, out);
	result = std::max(0.0f, result);
	return result;
}

float calculateRaytracedLighting(Camera *camera, glm::vec3 point, const Light light, const std::vector<ModelTriangle> &triangles) {
	point = point + 0.001 * normalize(light.position - point)* glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,-1));
	std::pair<ModelTriangle, glm::vec3> res = getClosestIntersection(point, normalize(point - light.position), triangles);
	float ambientWeight = 0.2;
	float prox = calculateProximityLighting(res, point, light, triangles);
	float diff = sqrt(calculateDiffuseLighting(res, camera, point, light, triangles));
	float comb = (1-ambientWeight)*(-(prox*diff)*(prox*diff) + 2 * prox*diff) + ambientWeight;
	float final = 0.8 * comb + 0.2 * calculateSpecularLighting(res, camera, point, light, 4, triangles);
	return final;
}

float calculateReflectionLighting(glm::vec3 from, glm::vec3 point, const Light light, const std::vector<ModelTriangle> &triangles) {
	point = point + 0.001 * normalize(light.position - point)* glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,-1));
	std::pair<ModelTriangle, glm::vec3> res = getClosestIntersection(point, normalize(point - light.position), triangles);
	float ambientWeight = 0.2;
	float prox = calculateProximityLighting(res, point, light, triangles);
	float diff = sqrt(calculateDiffuseReflectionLighting(res, from, point, light, triangles));
	float comb = (1-ambientWeight)*(-(prox*diff)*(prox*diff) + 2 * prox*diff) + ambientWeight;
	float final = 0.8 * comb + 0.2 * calculateSpecularReflectionLighting(res, from, point, light, 4, triangles);
	return final;
}

float calculateNormalRaytracedLighting(Camera *camera, glm::vec3 point, const glm::vec3 normal, const Light light, const std::vector<ModelTriangle> &triangles) {
	point = point + 0.001 * normalize(light.position - point)* glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,-1));
	std::pair<ModelTriangle, glm::vec3> res = getClosestIntersection(point, normalize(point - light.position), triangles);
	float ambientWeight = 0.3;
	float prox = calculateProximityLighting(res, point, light, triangles);
	float diff = sqrt(calculateNormalDiffuseLighting(res, point, normal, light, triangles));
	float comb = (1-ambientWeight)*(-(prox*diff)*(prox*diff) + 2 * prox*diff) + ambientWeight;
	float final = 0.8 * comb + 0.2 * calculateNormalSpecularLighting(res, camera, point, normal, light, 4, triangles);
	// float comb = calculateNormalSpecularLighting(camera, point,normal, light, 4, triangles);
	return final;
}

float calculateGouraudLighting(Camera *camera, glm::vec3 point, Light light, const std::vector<ModelTriangle> &triangles) {
	auto cipoint = point + 0.001 * normalize(light.position - point)* glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,-1));
	std::pair<ModelTriangle, glm::vec3> res = getClosestIntersection(cipoint, normalize(cipoint - light.position), triangles);
	auto triangle = res.first;
	auto normal1 = calculateVertexNormal(triangle.vertices[0], triangles);
	auto normal2 = calculateVertexNormal(triangle.vertices[1], triangles);
	auto normal3 = calculateVertexNormal(triangle.vertices[2], triangles);
	auto normal = glm::normalize(normal1 + normal2 + normal3);
	if (dot(camera->position - point, normal) < 0) {
		return 0;
	}
	float vb0 = calculateNormalRaytracedLighting(camera, triangle.vertices[0], normal1, light, triangles);
	float vb1 = calculateNormalRaytracedLighting(camera, triangle.vertices[1], normal2, light, triangles);
	float vb2 = calculateNormalRaytracedLighting(camera, triangle.vertices[2], normal3, light, triangles);
	auto weights = baryFromVec3(point, triangle);
	return vb0 * weights[0] + vb1 * weights[1] + vb2 * weights[2];
}

float calculatePhongLighting(Camera *camera, glm::vec3 point, Light light, const std::vector<ModelTriangle> &triangles) {
	auto cipoint = point + 0.001 * normalize(light.position - point)* glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,-1));
	std::pair<ModelTriangle, glm::vec3> res = getClosestIntersection(cipoint, normalize(cipoint - light.position), triangles);
	auto triangle = res.first;
	auto vn0 = calculateVertexNormal(triangle.vertices[0], triangles);
	auto vn1 = calculateVertexNormal(triangle.vertices[1], triangles);
	auto vn2 = calculateVertexNormal(triangle.vertices[2], triangles);
	auto weights = baryFromVec3(point, triangle);
	glm::vec3 normal = weights[0] * vn0 + weights[1] * vn1 + weights[2] * vn2;
	return calculateNormalRaytracedLighting(camera, point, normal, light, triangles);
}

Colour getTextureMappedColour(const std::vector<std::vector<TexturePoint>> &texture, const glm::vec3 triangleCoords, const ModelTriangle &triangle, const std::array<TexturePoint, 3> &textureCoords) {
	auto weights = baryFromVec3(triangleCoords, triangle);
	TexturePoint onTexture = texturePointFromBary(weights, textureCoords);
	auto toPaint = texture[onTexture.y][onTexture.x];
	return toPaint.colour;
}

Colour getReflectionColour(Camera *camera, std::vector<std::vector<TexturePoint>> texture, glm::vec3 point, Light *light, ModelTriangle triangle, std::vector<ModelTriangle> triangles){
	glm::vec3 Ri = normalize(point - camera->position);
	glm::vec3 Rr = normalize(Ri - 2*triangle.normal*dot(Ri, triangle.normal)) * glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,-1));
	point = point + 0.001 * normalize(light->position - point);
	std::pair<ModelTriangle, glm::vec3> res = getClosestIntersection(point, Rr, triangles);
	auto weighting = calculateReflectionLighting(point, res.second, *light, triangles);
	return res.first.colour * weighting;
}

void drawRaytraceOBJ(Camera *camera, float scalingFactor, const std::vector<std::vector<TexturePoint>>& texture, const std::vector<ModelTriangle>& triangles, Light *light, DrawingWindow &window) {
	float step = 0.00622;
#pragma omp parallel for
	for (int i=-WIDTH/2; i<WIDTH/2; i++) {
		for (int j=-HEIGHT/2; j<HEIGHT/2; j++) {//right, up, forward
			glm::vec3 pixel = camera->position + camera->orientation[0] * step * i - camera->orientation[1] * step * j + camera->focalLength * - camera->orientation[2];
			std::pair<ModelTriangle, glm::vec3> toPaint = getClosestIntersection(camera->position, normalize(camera->position - pixel), triangles);
			if (camera->mode == "RAYTRACE_P") { // this is inefficient
				glm::vec3 point = toPaint.second;
				point = point + 0.001 * normalize(light->position - point)* glm::mat3(glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,-1));
				std::pair<ModelTriangle, glm::vec3> res = getClosestIntersection(point, normalize(point - light->position), triangles);
				window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (toPaint.first.colour * calculateProximityLighting(res, point, *light, triangles)).asARGB());
			} else if (camera->mode == "RAYTRACE_D") {
				auto lighting = calculateRaytracedLighting(camera, toPaint.second, *light, triangles);
				window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (toPaint.first.colour * lighting).asARGB());
			} else if (camera->mode == "SPHERE_G") {
				auto lighting = calculateGouraudLighting(camera, toPaint.second, *light, triangles);
				window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (toPaint.first.colour * lighting).asARGB());
			} else if (camera->mode == "SPHERE_P") {
				auto lighting = calculatePhongLighting(camera, toPaint.second, *light, triangles);
				window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (toPaint.first.colour * lighting).asARGB());
			} else if (camera->mode == "RAYTRACE_TM") {
				auto lighting = calculateRaytracedLighting(camera, toPaint.second, *light, triangles);
				if (toPaint.first.colour == Colour(0,255,0)) {
					window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (getTextureMappedColour(texture, toPaint.second, toPaint.first,toPaint.first.texturePoints) *lighting).asARGB());
				} else {
					window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (toPaint.first.colour * lighting).asARGB());
				}
			} else if (camera->mode == "RAYTRACE_R") {
				auto lighting = calculateRaytracedLighting(camera, toPaint.second, *light, triangles);
				if (toPaint.first.colour == Colour(255, 0,255)) {
					window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (getReflectionColour(camera, texture, toPaint.second, light, toPaint.first, triangles) *lighting).asARGB());
				} else {
					window.setPixelColour(WIDTH/2-i, HEIGHT/2-j, (toPaint.first.colour * lighting).asARGB());
				}
			}
		}
	}
}

// Set the depth buffer to very far away everywhere
void clearDepthBuffer(std::vector<std::vector<float>> *depthBuffer) {
	std::vector<float> smol;
	for (int x=0; x<WIDTH; x++) {
		smol.push_back(-100);
	}
	for (int y = 0; y < HEIGHT; ++y) {
		(*depthBuffer)[y] = smol;
	}
}

// Defines keyboard input behaviour
void handleEvent(const SDL_Event &event, std::vector<std::vector<float>> *depthBuffer, Camera *camera, std::string filename, Light *light, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_u) {
			CanvasTriangle triangle = randomTriangle();
			drawStrokedTriangle(triangle, Colour(rand()%255,rand()%255, rand()%255), window);
		}
		else if (event.key.keysym.sym == SDLK_y) {
			CanvasTriangle triangle = randomTriangle();
			drawOccludedFilledTriangle(triangle, Colour(rand()%255,rand()%255, rand()%255), true, depthBuffer, window);
		}
		// else if (event.key.keysym.sym == SDLK_t) {
		// 	isThisWorking(camera, *light, parseOBJ(filename, 0.35));
		// 	std::cout << "boop" << std::endl;
		// }
		else if (event.key.keysym.sym == SDLK_e) {
			camera->lookAt(glm::vec3(0,0,0));
			window.clearPixels();
			clearDepthBuffer(depthBuffer);
		}
		else if (event.key.keysym.sym == SDLK_o) {
			// Print current orientation matrix
			std::cout << camera->orientation[0][0] << ", " << camera->orientation[1][0] << ", " << camera->orientation[2][0] << std::endl;
			std::cout << camera->orientation[0][1] << ", " << camera->orientation[1][1] << ", " << camera->orientation[2][1] << std::endl;
			std::cout << camera->orientation[0][2] << ", " << camera->orientation[1][2] << ", " << camera->orientation[2][2] << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_1) {
			camera->mode = "WIREFRAME";
		}
		else if (event.key.keysym.sym == SDLK_2) {
			camera->mode = "RASTERISE";
		}
		else if (event.key.keysym.sym == SDLK_3) {
			//proximity lighting
			camera->mode = "RAYTRACE_P";
		}
		else if (event.key.keysym.sym == SDLK_4) {
			//prox, diff and specular
			camera->mode = "RAYTRACE_D";
		}
		else if (event.key.keysym.sym == SDLK_5) {
			//sphere wireframe
			camera->mode = "SPHERE_W";
		}
		else if (event.key.keysym.sym == SDLK_6) {
			//enable Gouraud shading
			camera->mode = "SPHERE_G";
		}
		else if (event.key.keysym.sym == SDLK_7) {
			//enable Phong shading
			camera->mode = "SPHERE_P";
		}
		else if (event.key.keysym.sym == SDLK_8) {
			//texture map
			std::cout << "boople" << std::endl;
			camera->mode = "RAYTRACE_TM";
		}
		else if (event.key.keysym.sym == SDLK_9) {
			//texture map
			std::cout << "boople" << std::endl;
			camera->mode = "RAYTRACE_R";
		}
		else if (event.key.keysym.sym == SDLK_r) {
			//texture map
			std::cout << "boople" << std::endl;
			camera->mode = "RECORD";
		}
		else if (event.key.keysym.sym == SDLK_p) {
			auto print = camera->position;
			std::cout << print.x << ", " << print.y << ", " << print.z << std::endl;
		}
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

void draw(std::vector<std::vector<float>> *depthBuffer, Camera *camera, std::vector<ModelTriangle> trianglesB, std::vector<ModelTriangle> trianglesS, const std::vector<std::vector<TexturePoint>>& texture, Light *light, DrawingWindow &window) {
	window.clearPixels();
	if (camera->mode == "WIREFRAME") {
		for (int i=0; i<trianglesB.size(); i++) {
			auto triangle = trianglesB[i];
			clearDepthBuffer(depthBuffer);
			CanvasTriangle zoop = CanvasTriangle(
			projectVertexOntoCanvasPoint(camera, triangle.vertices[0], 160),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[1], 160),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[2], 160));
			drawStrokedTriangle(zoop, triangle.colour, window);
		}
	} else if (camera->mode == "RASTERISE") {
		clearDepthBuffer(depthBuffer);
		drawOBJ(camera, 160, depthBuffer, trianglesB, window);
	} else if (camera->mode == "RAYTRACE_TM") {
		drawRaytraceOBJ(camera, 0.35, texture, trianglesB, light, window);
	} else if (camera->mode == "RAYTRACE_R") {
		drawRaytraceOBJ(camera, 0.35, texture, trianglesB, light, window);
	} else if (camera->mode == "SPHERE_G" || camera->mode == "SPHERE_P") {
		drawRaytraceOBJ(camera, 0.35, texture, trianglesS, light, window);
	} else if (camera->mode == "SPHERE_W") {
		for (int i=0; i<trianglesS.size(); i++) {
			auto triangle = trianglesS[i];
			clearDepthBuffer(depthBuffer);
			CanvasTriangle zoop = CanvasTriangle(
			projectVertexOntoCanvasPoint(camera, triangle.vertices[0], 160),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[1], 160),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[2], 160));
			drawStrokedTriangle(zoop, triangle.colour, window);
		}
	} else if (camera->mode == "RECORD") {
		for (int i=0; i<trianglesB.size(); i++) {
			auto triangle = trianglesB[i];
			clearDepthBuffer(depthBuffer);
			CanvasTriangle zoop = CanvasTriangle(
			projectVertexOntoCanvasPoint(camera, triangle.vertices[0], 160),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[1], 160),
			projectVertexOntoCanvasPoint(camera, triangle.vertices[2], 160));
			drawStrokedTriangle(zoop, triangle.colour, window);
		}
	} else {
		drawRaytraceOBJ(camera, 0.35, texture, trianglesB, light, window);
	}
}

void movement(std::vector<std::vector<float>> *depthBuffer, Camera *camera, DrawingWindow &window, Light *light, const float deltaTime) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	float speed = 0.5f;
	if (state[SDL_SCANCODE_LEFT]) {
		camera->translateCamera(glm::vec3(1, 0, 0) * speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		camera->translateCamera(glm::vec3(-1, 0, 0) * speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_UP]) {
		camera->translateCamera(glm::vec3(0, -1, 0) * speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_DOWN]) {
		camera->translateCamera(glm::vec3(0, 1, 0) * speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_W]) {
		camera->tiltCamera(speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_S]) {
		camera->tiltCamera(-speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_A]) {
		camera->panCamera(-speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_D]) {
		camera->panCamera(speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_Z]) {
		camera->translateCamera(glm::vec3(0, 0, 1) * speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_X]) {
		camera->translateCamera(glm::vec3(0, 0, -1) * speed * deltaTime);
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_Q]) {
		camera->orbit(0.3f * deltaTime);
		camera->lookAt(glm::vec3(0,0,0));
		window.clearPixels();
		clearDepthBuffer(depthBuffer);
	}
	if (state[SDL_SCANCODE_MINUS]) {
		light->position -= (0.1f * deltaTime * glm::vec3{0,1,-0.1});
	}
	if (state[SDL_SCANCODE_EQUALS]) {
		light->position += (0.1f * deltaTime * glm::vec3{0,1,-0.1});
	}
}

void doPlayback(Camera *camera, std::vector<std::pair<glm::vec3, glm::mat3>> poss, std::vector<std::vector<float>> *depthBuffer,std::vector<ModelTriangle> trianglesB,std::vector<ModelTriangle> trianglesS, const std::vector<std::vector<TexturePoint>>& texture, Light *light, DrawingWindow window){
	int id=0;
	camera->mode = "SPHERE_P";
	std::string filename;
	for (auto elem : poss) {
		camera->position = elem.first;
		camera->orientation = elem.second;
		draw(depthBuffer, camera, trianglesB, trianglesS, texture, light, window);
		if (id < 10){
			filename = "assets/bmps/b000" + std::to_string(id) + ".bmp";
		}else if (id < 100){
			filename = "assets/bmps/b00" + std::to_string(id) + ".bmp";
		}else if (id < 1000){
			filename = "assets/bmps/b0" + std::to_string(id) + ".bmp";
		}else{
			filename = "assets/bmps/b" + std::to_string(id) + ".bmp";
		}
		window.saveBMP(filename);
		std::cout << "frame " << filename << std::endl;
		id++;
	}
}

std::vector<std::pair<glm::vec3, glm::mat3>> parseRecording(std::string filename){
	std::ifstream myfile;
	myfile.open("/home/dustmodebros/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/assets/recording.txt",std::ios_base::in);
	std::string line;
	std::vector<std::pair<glm::vec3, glm::mat3>> out;
	while(std::getline(myfile, line)){
		std::string posLine;
		std::string orLine1;
		std::string orLine2;
		std::string orLine3;
		getline(myfile, posLine);
		getline(myfile, orLine1);
		getline(myfile, orLine2);
		getline(myfile, orLine3);
		posLine = posLine.substr(posLine.find(":")+2);
		float posLine1 = std::stof(posLine.substr(0,posLine.find(",")));
		posLine = posLine.substr(posLine.find(",")+2);
		float posLine2 = std::stof(posLine.substr(0,posLine.find(",")));
		posLine = posLine.substr(posLine.find(",")+2);
		float posLine3 = std::stof(posLine);
		glm::vec3 pos = {posLine1, posLine2, posLine3};

		orLine1 = orLine1.substr(orLine1.find(":")+1);
		float orLine11 = std::stof(orLine1.substr(0,orLine1.find(",")));
		orLine1 = orLine1.substr(orLine1.find(",")+1);
		float orLine12 = std::stof(orLine1.substr(0,orLine1.find(",")));
		orLine1 = orLine1.substr(orLine1.find(",")+1);
		float orLine13 = std::stof(orLine1);

		orLine2 = orLine2.substr(orLine2.find(":")+1);
		float orLine21 = std::stof(orLine2.substr(0,orLine2.find(",")));
		orLine2 = orLine2.substr(orLine2.find(",")+1);
		float orLine22 = std::stof(orLine2.substr(0,orLine2.find(",")));
		orLine2 = orLine2.substr(orLine2.find(",")+1);
		float orLine23 = std::stof(orLine2);

		orLine3 = orLine3.substr(orLine3.find(":")+1);
		float orLine31 = std::stof(orLine3.substr(0,orLine3.find(",")));
		orLine3 = orLine3.substr(orLine3.find(",")+1);
		float orLine32 = std::stof(orLine3.substr(0,orLine3.find(",")));
		orLine3 = orLine3.substr(orLine3.find(",")+1);
		float orLine33 = std::stof(orLine3);

		glm::vec3 or1 = {orLine11, orLine21, orLine31};
		glm::vec3 or2 = {orLine12, orLine22, orLine32};
		glm::vec3 or3 = {orLine13, orLine23, orLine33};
		auto orMat = glm::mat3(or1, or2, or3);
		out.emplace_back(std::pair<glm::vec3, glm::mat3>(pos, orMat));
	}
	return out;
}

int main(int argc, char *argv[]) {
	const auto texture_map = TextureMap("assets/texture.ppm");
	const std::string filename = "assets/cornell-box.obj";
	const std::string filename2 = "assets/sphere.obj";
	auto texture = loadTexture(texture_map);
	// const std::string filename = "assets/cornell-box copy.obj";
	auto depthBuffer = newDepthBuffer();
	Camera c = Camera(glm::vec3(0,0,4), glm::mat3(glm::vec3(1,0,0),glm::vec3(0,1,0),glm::vec3(0,0,1)), 2);
	Camera *camera = &c;
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	Light light =  Light();
	SDL_Event event;
	bool playback = false;
	// drawTexture(texture, window);
	auto trianglesB = debugParseOBJ(filename, light, texture, 0.35);
	auto trianglesS = debugParseOBJ(filename2, light, texture, 0.35);
	float deltaTime = 0.0f;
	std::vector<std::pair<glm::vec3, glm::mat3>> movements;
	if (playback){
		movements = parseRecording("assets/recording.txt");
		std::cout << movements.size() << std::endl;
	}
	
	Uint32 lastFrameTime = SDL_GetTicks();
	while (true) {
		Uint32 thisFrameTime = SDL_GetTicks();
		if (camera->mode != "RECORD"){
			deltaTime = (static_cast<float>(thisFrameTime - lastFrameTime))/1000;
		} else {
			deltaTime = 1.0/300;
		}
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, depthBuffer, camera, filename, &light, window);
		movement(depthBuffer, camera, window, &light, deltaTime);
		if (!playback){
			draw(depthBuffer, camera, trianglesB, trianglesS, texture, &light, window);
		} else {
			std::cout << "starting render" << std::endl;
			doPlayback(camera, movements, depthBuffer, trianglesB, trianglesS, texture, &light, window);
			std::cout << "done render" << std::endl;
			exit(0);
		}
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		if (camera->mode == "RECORD"){
			std::ofstream myfile;
			myfile.open("/home/dustmodebros/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/assets/recording.txt",std::ios_base::app);
			myfile << "pos: " << camera->position.x << ", " << camera->position.y << ", " << camera->position.z << std::endl;
			myfile << "orient1: " << camera->orientation[0].x << ", " << camera->orientation[1].x << ", " << camera->orientation[2].x << std::endl;
			myfile << "orient2: " << camera->orientation[0].y << ", " << camera->orientation[1].y << ", " << camera->orientation[2].y << std::endl;
			myfile << "orient3: " << camera->orientation[0].z << ", " << camera->orientation[1].z << ", " << camera->orientation[2].z << std::endl;
			myfile << std::endl;
			myfile.close();
		}
		window.renderFrame();
		lastFrameTime = thisFrameTime;
	}
}
