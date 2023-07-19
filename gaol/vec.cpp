#include <cstdint>
#include <string>
#include <vector>

#include "gaol.hpp"

#include <cmath>

void GAOL::vec::set(double _x, double _y) {
	x = _x;
	y = _y;
}

void GAOL::vec::set_ang(double ang, double mag) {
	x = mag * cos(ang);
	y = mag * sin(ang);
}

double GAOL::vec::mag() const {
	return sqrt(pow(x,2) + pow(y,2));
}

double GAOL::vec::ang() const {
	return atan2(y, x);
}

GAOL::vec GAOL::vec::operator=(vec other) {
	x = other.x;
	y = other.y;
	return *this;
}

GAOL::vec GAOL::vec::operator+(vec other) const {
	return vec(x + other.x, y + other.y);
}

GAOL::vec GAOL::vec::operator+=(vec other) {
	operator=(operator+(other));
	return *this;
}

GAOL::vec GAOL::vec::operator-(vec other) const {
	return vec(x - other.x, y - other.y);
}

GAOL::vec GAOL::vec::operator-=(vec other) {
	operator=(operator-(other));
	return *this;
}

GAOL::vec GAOL::vec::operator*(vec other) const {
	return vec(x * other.x, y * other.y);
}

GAOL::vec GAOL::vec::operator*=(vec other) {
	operator=(operator*(other));
	return *this;
}

GAOL::vec GAOL::vec::operator*(double scalar) const {
	return vec(x * scalar, y * scalar);
}

GAOL::vec GAOL::vec::operator*=(double scalar) {
	operator=(operator*(scalar));
	return *this;
}

GAOL::vec GAOL::vec::operator/(vec other) const {
	return vec(x / other.x, y / other.y);
}

GAOL::vec GAOL::vec::operator/=(vec other) {
	operator=(operator/(other));
	return *this;
}

GAOL::vec GAOL::vec::operator/(double scalar) const {
	return vec(x / scalar, y / scalar);
}

GAOL::vec GAOL::vec::operator/=(double scalar) {
	operator=(operator/(scalar));
	return *this;
}
