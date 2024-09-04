#pragma once

#include <iostream>

struct Vector2f {
public:
	Vector2f(float p_x = 0.0f, float p_y = 0.0f) : x{ p_x }, y{ p_y } {}

	Vector2f& operator=(const Vector2f& other) {
		if (this == &other) {
			return *this;
		}
		x = other.x;
		y = other.y;
		return *this;
	}

	friend Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs) {
		return Vector2f(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	friend Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs) {
		return Vector2f(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	friend Vector2f operator*(const float& lhs, const Vector2f& rhs) {
		return Vector2f(lhs * rhs.x, lhs * rhs.y);
	}

	friend bool operator==(const Vector2f& lhs, const Vector2f& rhs) {
		return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
	}

	friend bool operator!=(const Vector2f& lhs, const Vector2f& rhs) {
		return ((lhs.x != rhs.x) || (lhs.y != rhs.y));
	}

	float dotProduct(const Vector2f& rhs) {
		return (this->x * rhs.x + this->y * rhs.y);
	}

	float magnitude() {
		return sqrt(pow(this->x, 2) + pow(this->y, 2));
	}

	void print() {
		std::cout << x << ", " << y << '\n';
	}

	float x{};
	float y{};
private:

};