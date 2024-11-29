#pragma once

#include <iostream>

struct Colour {
	std::string name;
	int red{};
	int green{};
	int blue{};
	Colour();
	Colour(int r, int g, int b);
	Colour(std::string n, int r, int g, int b);
	int asARGB();
	Colour operator*(float x) const;

	bool operator==(const Colour &colour) const;

};

std::ostream &operator<<(std::ostream &os, const Colour &colour);
