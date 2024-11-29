#include "Colour.h"
#include <utility>

Colour::Colour() = default;
Colour::Colour(int r, int g, int b) : red(r), green(g), blue(b) {}
Colour::Colour(std::string n, int r, int g, int b) :
		name(std::move(n)),
		red(r), green(g), blue(b) {}

int Colour::asARGB() {
	return (255<<24) + (this->red<<16) + (this->green<<8) + (this->blue);
}

Colour Colour::operator*(float x) const {
	return {static_cast<int>(x * red), static_cast<int>(x * green), static_cast<int>(x * blue)};
}


std::ostream &operator<<(std::ostream &os, const Colour &colour) {
	os << colour.name << " ["
	   << colour.red << ", "
	   << colour.green << ", "
	   << colour.blue << "]";
	return os;
}

bool Colour::operator==(const Colour &colour) const {
	return (red == colour.red && green == colour.green && blue == colour.blue);
}
