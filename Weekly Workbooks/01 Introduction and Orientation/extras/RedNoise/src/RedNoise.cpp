#include <CanvasTriangle.h>
#include <Colour.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <random>
#include <TextureMap.h>
#include <valarray>
#include <vector>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/ext.hpp>

#define WIDTH 500
#define HEIGHT 400
// std::vector<float> interpolateSingleFloats(const float from, const float to, int const numberOfValues) {
// 	auto const outList = new(std::vector<float>[numberOfValues]);
// 	const float incr = (to-from) / (static_cast<float>(numberOfValues-1));
// 	for (int i=0; i<numberOfValues; i++) {
// 		outList->push_back(from + i * incr);
// 	}
// 	return *outList;
// }

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

// std::vector<int> interpv2(int from, int to, int const numberOfValues) {
// 	std::vector<int> outList;
// 	const int incr = (to-from) / numberOfValues-1;
// 	outList.reserve(numberOfValues);
// 	for (int i=0; i<numberOfValues; i++) {
// 		outList.push_back(from + i * incr);
// 	}
// 	return outList;
// }

void drawLine(int x1, int y1, int x2, int y2, Colour colour, DrawingWindow &window) {
	glm::vec2 start = glm::vec2(x1, y1);
	glm::vec2 end = glm::vec2(x2, y2);
	auto points = interpv2(start, end, std::abs(start.x - end.x) + std::abs(start.y-end.y)+1);
	for (auto point: points) {
		window.setPixelColour(static_cast<int>(point.x), static_cast<int>(point.y),colour.asARGB());
	}
}

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

CanvasTriangle randomTriangle() {
	return CanvasTriangle(
	CanvasPoint(rand()%WIDTH, rand()%HEIGHT),
	CanvasPoint(rand()%WIDTH, rand()%HEIGHT),
	CanvasPoint(rand()%WIDTH, rand()%HEIGHT)
	);
}

void drawStrokedTriangle(CanvasTriangle &triangle, const Colour& c, DrawingWindow &window) {
	auto p0 = glm::vec2(triangle.v0().x, triangle.v0().y);
	auto p1 = glm::vec2(triangle.v1().x, triangle.v1().y);
	auto p2 = glm::vec2(triangle.v2().x, triangle.v2().y);
	drawLine(p0.x, p0.y, p1.x, p1.y,c,window);
	drawLine(p1.x, p1.y, p2.x, p2.y,c,window);
	drawLine(p0.x, p0.y, p2.x, p2.y,c,window);
}

void drawPartialTriangle(CanvasPoint shared, CanvasPoint p1, CanvasPoint p2, const Colour& c, DrawingWindow &window) {
	drawLine(shared.x, shared.y, p1.x, p1.y,c,window);
	drawLine(shared.x, shared.y, p2.x, p2.y,c,window);
}

std::vector<glm::vec2> drawFlatTriangle(CanvasTriangle triangle, const Colour &c, DrawingWindow &window) {
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
	for (int i=0; i< fromPoints.size(); i++) {
		if (static_cast<int>(fromPoints[i].x) != static_cast<int>(toPoints[i].x) || static_cast<int>(fromPoints[i].y) != static_cast<int>(toPoints[i].y)) {
			drawLine(fromPoints[i].x,fromPoints[i].y, toPoints[i].x, toPoints[i].y ,c,window);
		}
	}
	drawLine(shared.x, shared.y, p1.x, p1.y, Colour(255,255,255), window);
	drawLine(shared.x, shared.y, p2.x, p2.y, Colour(255,255,255), window);
	return fromPoints;
}

void drawFilledTriangle(CanvasTriangle triangle, Colour c, DrawingWindow &window) {
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
		drawFlatTriangle(triangle1, c, window);
		drawFlatTriangle(triangle2, c, window);
		drawPartialTriangle(point2, point1, point4, Colour(255,255,255), window);
		drawPartialTriangle(point3, point1, point4, Colour(255,255,255), window);
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
		drawFlatTriangle(triangle1, c, window);
		drawFlatTriangle(triangle2, c, window);
		drawPartialTriangle(point1, point2, point4, Colour(255,255,255), window);
		drawPartialTriangle(point3, point2, point4, Colour(255,255,255), window);
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
		drawFlatTriangle(triangle1, c, window);
		drawFlatTriangle(triangle2, c, window);
		drawPartialTriangle(point1, point3, point4, Colour(255,255,255), window);
		drawPartialTriangle(point2, point3, point4, Colour(255,255,255), window);
	} else {
		//we have a flat triangle
		drawFlatTriangle(triangle, c, window);
		drawStrokedTriangle(triangle, Colour(255,255,255), window);
	}
}

std::vector<glm::vec2> drawTexturedLine(int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2, std::vector<std::vector<TexturePoint>> texture, DrawingWindow &window) {
	//return vector of interpolated pixel values between two points on the texture, with the numberOfValues being the same as the numberOfValues in the image
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
	for (int i=0; i<imagePoints.size(); i++) {
		std::cout << textureColours[i] << std::endl;
		window.setPixelColour(static_cast<int>(imagePoints[i].x), static_cast<int>(imagePoints[i].y),textureColours[i].asARGB());
	}
	return texturePoints;
}


void drawFlatTexturedTriangle(CanvasTriangle imageTriangle, std::vector<std::vector<TexturePoint>> texture, CanvasTriangle textureTriangle, const Colour &c, DrawingWindow &window) {
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
		tp1 = glm::vec2(static_cast<int>(textureTriangle.v0().x), static_cast<int>(textureTriangle.v0().y));
		ip1 = glm::vec2(static_cast<int>(imageTriangle.v1().x), static_cast<int>(imageTriangle.v1().y));
		tp2 = glm::vec2(static_cast<int>(textureTriangle.v2().x), static_cast<int>(textureTriangle.v2().y));
		ip2 = glm::vec2(static_cast<int>(imageTriangle.v2().x), static_cast<int>(imageTriangle.v2().y));
	}
	//I GOT UP TO HERE
	std::vector<glm::vec2> fromPoints = interpv2(iShared, ip1, std::abs(iShared.y-ip1.y)+1);
	std::vector<glm::vec2> toPoints = interpv2(iShared, ip2, std::abs(iShared.y-ip2.y)+1);
	for (int i=0; i< fromPoints.size(); i++) {
		if (static_cast<int>(fromPoints[i].x) != static_cast<int>(toPoints[i].x) || static_cast<int>(fromPoints[i].y) != static_cast<int>(toPoints[i].y)) {
			drawLine(fromPoints[i].x,fromPoints[i].y, toPoints[i].x, toPoints[i].y ,c,window);
		}
	}
	drawTexturedLine(iShared.x, iShared.y, ip1.x, ip1.y, Colour(255,255,255), window);
	drawTexturedLine(iShared.x, iShared.y, ip2.x, ip2.y, Colour(255,255,255), window);
}

std::vector<std::vector<TexturePoint>> loadTexture(TextureMap texture) {
	std::cout << "Loading image: " << texture.width << " by " << texture.height << std::endl;
	std::vector<std::vector<TexturePoint>> outVector;
	std::vector<TexturePoint> smallOutVector;

	for (int y=0; y<texture.height; y++) {
		for (int x=0; x<texture.width; x++) {
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

void drawTexturedTriangle(CanvasTriangle imageTriangle, TextureMap texture, CanvasPoint tp1, CanvasPoint tp2, CanvasPoint tp3, DrawingWindow &window) {
	drawStrokedTriangle(imageTriangle, Colour(255,255,255), window);
	CanvasPoint point1 = imageTriangle.v0();
	CanvasPoint point2 = imageTriangle.v1();
	CanvasPoint point3 = imageTriangle.v2();
	CanvasPoint point4;
	Colour c = {255,255,255};
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
		drawFlatTriangle(triangle1, c, window);
		drawFlatTriangle(triangle2, c, window);
		drawPartialTriangle(point2, point1, point4, Colour(255,255,255), window);
		drawPartialTriangle(point3, point1, point4, Colour(255,255,255), window);
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
		drawFlatTriangle(triangle1, c, window);
		drawFlatTriangle(triangle2, c, window);
		drawPartialTriangle(point1, point2, point4, Colour(255,255,255), window);
		drawPartialTriangle(point3, point2, point4, Colour(255,255,255), window);
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
		drawFlatTriangle(triangle1, c, window);
		drawFlatTriangle(triangle2, c, window);
		drawPartialTriangle(point1, point3, point4, Colour(255,255,255), window);
		drawPartialTriangle(point2, point3, point4, Colour(255,255,255), window);
	} else {
		//we have a flat triangle
		drawFlatTriangle(imageTriangle, c, window);
		drawStrokedTriangle(imageTriangle, Colour(255,255,255), window);
	}
}


void draw(DrawingWindow &window) {
	// TextureMap texture = TextureMap("/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/texture.ppm");
	// CanvasTriangle triangle =
	// 	{CanvasPoint(160,10),
	// 	 CanvasPoint(300,230),
	// 	 CanvasPoint(10,150)
	// };
	// TexturePoint tp1 = {195,5};
	// TexturePoint tp2 = {395,380};
	// TexturePoint tp3 = {65,330};
	// drawTexturedTriangle(triangle, texture, tp1, tp2, tp3, window);
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_u) {
			CanvasTriangle triangle = randomTriangle();
			drawStrokedTriangle(triangle, Colour(rand()%255,rand()%255, rand()%255), window);
		}
		else if (event.key.keysym.sym == SDLK_y) {
			CanvasTriangle triangle = randomTriangle();
			drawFilledTriangle(triangle, Colour(rand()%255,rand()%255, rand()%255), window);
		}
		else if (event.key.keysym.sym == SDLK_t) {
			window.clearPixels();
		}
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	std::vector<float> result;
	for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
	auto v1 = glm::vec2(27, 30);
	auto v2 = glm::vec2(13, 30);
	auto v3 = glm::vec2(15, 50);
	auto res1 = interpv2(v1, v2, std::abs(v1.y-v2.y)+1);
	auto res2 = interpv2(v2, v3, std::abs(v1.y-v2.y)+1);
	auto res3 = interpv2(v1, v2, std::abs(v1.y-v2.y)+1);
	for (glm::vec2 elem : res1) {
		std::cout << glm::to_string(elem) << std::endl;
	}
	TextureMap texture = TextureMap("/Users/samuelstephens/Year3/CG2024/Weekly Workbooks/01 Introduction and Orientation/extras/RedNoise/texture.ppm");
	auto texturePoints = loadTexture(texture);
	// for (int y=0; y<texture.height; y++){
	// 	for (int x=0; x<texture.width; x++) {
	// 		window.setPixelColour(x, y, texturePoints[y][x].colour.asARGB());
	// 	}
	// }
	auto texturePointsOut = drawTextureLine(0,200,200,200, 112,40,113,47,texturePoints, window);
	for (auto each : texturePointsOut) {
		std::cout << each.x << ", " << each.y << std::endl;
	}
	std::cout << std::endl;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
