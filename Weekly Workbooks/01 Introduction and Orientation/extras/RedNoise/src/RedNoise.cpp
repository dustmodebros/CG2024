#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <ModelTriangle.h>
#include <random>
#include <TextureMap.h>
#include <valarray>
#include <vector>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/ext.hpp>

#define WIDTH 500
#define HEIGHT 400

// Global variables (yuck)
glm::vec3 cameraPos = glm::vec3(0, 0, 4);
std::vector<std::vector<float>> *depthBuffer = new std::vector<std::vector<float>>;

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

std::vector<glm::vec2> interpv2(glm::vec2 from, glm::vec2 to, int const numberOfValues) {
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

std::vector<glm::vec3> interpv3(glm::vec3 from, glm::vec3 to, int const numberOfValues) {
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
void drawLine(int x1, int y1, int x2, int y2, Colour colour, DrawingWindow &window) {
	glm::vec2 start = glm::vec2(x1, y1);
	glm::vec2 end = glm::vec2(x2, y2);
	auto points = interpv2(start, end, std::abs(start.x - end.x) + std::abs(start.y-end.y)+1);
	for (auto point: points) {
		window.setPixelColour(static_cast<int>(point.x), static_cast<int>(point.y),colour.asARGB());
	}
}

// Draws a straight interpolated line between two points with a colour to the window wrt depths
void drawOccludedLine(int x1, int y1, float z1, int x2, int y2, float z2, Colour colour, std::vector<std::vector<float>> *depthBuffer, DrawingWindow &window) {
	glm::vec3 start = glm::vec3(x1, y1, z1);
	glm::vec3 end = glm::vec3(x2, y2, z2);
	auto points = interpv3(start, end, std::abs(start.x - end.x) + std::abs(start.y-end.y)+1);
	for (auto point: points) {
		if(1/(*depthBuffer)[point.y][point.x] > 1/point.z ) {
			(*depthBuffer)[point.y][point.x] = point.z;
			window.setPixelColour(static_cast<int>(point.x), static_cast<int>(point.y),colour.asARGB());
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
void drawPartialTriangle(CanvasPoint shared, CanvasPoint p1, CanvasPoint p2, const Colour& c, DrawingWindow &window) {
	drawLine(shared.x, shared.y, p1.x, p1.y,c,window);
	drawLine(shared.x, shared.y, p2.x, p2.y,c,window);
}

// Draws a white stroked filled triangle with a flat top or bottom to the screen with fill colour c to the window
void drawFlatTriangle(CanvasTriangle triangle, const Colour &c, bool drawOutline, DrawingWindow &window) {
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
void drawTexturedLine(int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2, std::vector<std::vector<TexturePoint>> texture, DrawingWindow &window) {
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
		std::cout << "boops" << std::endl;

		auto otherLine = interp(glm::vec2(static_cast<int>(ipoint1.x), static_cast<int>(ipoint1.y)), glm::vec2(static_cast<int>(ipoint2.x), static_cast<int>(ipoint2.y)), std::max(std::abs(ipoint1.x - ipoint2.x)+1, std::abs(ipoint1.y-ipoint2.y))+1);
		auto totherLine = interp(glm::vec2(static_cast<int>(tpoint1.x), static_cast<int>(tpoint1.y)), glm::vec2(static_cast<int>(tpoint2.x), static_cast<int>(tpoint2.y)), std::max(std::abs(ipoint1.x - ipoint2.x)+1, std::abs(ipoint1.y-ipoint2.y))+1);
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
			outVector.emplace_back(vertices[std::stoi(v1)-1]*scalingParameter, vertices[std::stoi(v2)-1]*scalingParameter, vertices[std::stoi(v3)-1]*scalingParameter, currentColour);
		}
	}
	return outVector;
}

// Convert vertexPosition to CanvasPoint relative to the cameraPosition
CanvasPoint projectVertexOntoCanvasPoint(glm::vec3 cameraPosition, float focalLength, glm::vec3 vertexPosition, float scalingFactor) {
	auto tVP = vertexPosition - cameraPosition;
	float u = static_cast<int>(focalLength * -tVP.x/tVP.z * scalingFactor + WIDTH/2);
	float v = static_cast<int>(focalLength * tVP.y/tVP.z * scalingFactor + HEIGHT/2);
	return CanvasPoint(u, v, tVP.z);
}

// Render the .obj file in the window
void drawOBJ(glm::vec3 cameraPos, float focalLength, float scalingFactor, std::vector<std::vector<float>> *depthBuffer, DrawingWindow &window) {
	CanvasTriangle renderTriangle;
	const std::string filename = "/Users/samuelstephens/Downloads/cornell-box.obj";
	const std::vector<ModelTriangle> modelTriangles = parseOBJ(filename, 0.35);
	for (auto triangle: modelTriangles) {
		renderTriangle = {
			projectVertexOntoCanvasPoint(cameraPos, focalLength, triangle.vertices[0], scalingFactor),
			projectVertexOntoCanvasPoint(cameraPos, focalLength, triangle.vertices[1], scalingFactor),
			projectVertexOntoCanvasPoint(cameraPos, focalLength, triangle.vertices[2], scalingFactor)
		};
		drawOccludedFilledTriangle(renderTriangle, triangle.colour, false, depthBuffer, window);
	}
}

// Set the depth buffer to very far away everywhere
void clearDepthBuffer() {
	std::vector<float> smol;
	for (int x=0; x<WIDTH; x++) {
		smol.push_back(-100);
	}
	for (int y = 0; y < HEIGHT; ++y) {
		(*depthBuffer)[y] = smol;
	}
}

// Defines keyboard input behaviour
void handleEvent(const SDL_Event &event, DrawingWindow &window) {
	auto focalLength = 2.0;
	auto scalingFactor = 160;
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) {
			window.clearPixels();
			cameraPos = cameraPos -= glm::vec3(0.05, 0, 0);
			clearDepthBuffer();
			drawOBJ(cameraPos, focalLength, scalingFactor, depthBuffer, window);
		}
		else if (event.key.keysym.sym == SDLK_RIGHT) {
			window.clearPixels();
			cameraPos = cameraPos -= glm::vec3(-0.05, 0, 0);
			clearDepthBuffer();
			drawOBJ(cameraPos, focalLength, scalingFactor, depthBuffer, window);
		}
		else if (event.key.keysym.sym == SDLK_UP) {
			window.clearPixels();
			cameraPos = cameraPos -= glm::vec3(0, -0.05, 0);
			clearDepthBuffer();
			drawOBJ(cameraPos, focalLength, scalingFactor,depthBuffer, window);
		}
		else if (event.key.keysym.sym == SDLK_DOWN) {
			window.clearPixels();
			cameraPos = cameraPos -= glm::vec3(0, 0.05, 0);
			clearDepthBuffer();
			drawOBJ(cameraPos, focalLength, scalingFactor,depthBuffer, window);
		}
		else if (event.key.keysym.sym == SDLK_z) {
			window.clearPixels();
			cameraPos = cameraPos -= glm::vec3(0, 0, 0.05);
			clearDepthBuffer();
			drawOBJ(cameraPos, focalLength, scalingFactor,depthBuffer, window);
		}
		else if (event.key.keysym.sym == SDLK_x) {
			window.clearPixels();
			cameraPos = cameraPos -= glm::vec3(0, 0, -0.05);
			clearDepthBuffer();
			drawOBJ(cameraPos, focalLength, scalingFactor,depthBuffer, window);
		}
		else if (event.key.keysym.sym == SDLK_u) {
			CanvasTriangle triangle = randomTriangle();
			drawStrokedTriangle(triangle, Colour(rand()%255,rand()%255, rand()%255), window);
		}
		else if (event.key.keysym.sym == SDLK_y) {
			CanvasTriangle triangle = randomTriangle();
			drawOccludedFilledTriangle(triangle, Colour(rand()%255,rand()%255, rand()%255), true, depthBuffer, window);
		}
		else if (event.key.keysym.sym == SDLK_t) {
			window.clearPixels();
			clearDepthBuffer();
		}
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

void draw(DrawingWindow &window) {

}

int main(int argc, char *argv[]) {
	// const auto texture = TextureMap("/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/texture.ppm");
	const std::string filename = "/Users/samuelstephens/Downloads/cornell-box.obj";
	std::vector<float> smol;
	for (int x = 0; x < WIDTH; ++x) {
		smol.push_back(-100);
	}
	for (int y = 0; y < HEIGHT; ++y) {
		depthBuffer->emplace_back(smol);
	}
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	drawOBJ(cameraPos, 2, 160, depthBuffer, window);
	SDL_Event event;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
