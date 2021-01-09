//
// Created by viniciusrsouza on 20/05/2020.
//
#include "pch.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include "Vector2D.h"

using namespace MathLib;

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(float nX, float nY) : x(nX), y(nY) {}

Vector2D& Vector2D::operator=(const Vector2D& u) {
    x = u.x;
    y = u.y;
    return *this;
}

bool Vector2D::operator==(const Vector2D& u) const {
    return x == u.x && y == u.y;
}

Vector2D Vector2D::operator-() const {
    return Vector2D(-x, -y);
}

void Vector2D::zero() {
    x = y = 0.0f;
}

Vector2D Vector2D::operator-(const Vector2D& u) const {
    return Vector2D(x - u.x, y - u.y);
}

Vector2D Vector2D::operator+(const Vector2D& u) const {
    return Vector2D(x + u.x, y + u.y);
}

Vector2D Vector2D::operator*(const float c) const {
    return Vector2D(c * x, c * y);
}

Vector2D Vector2D::operator/(const float c) const {
    return Vector2D(x / c, y / c);
}

Vector2D Vector2D::operator-=(const Vector2D& u) {
    x -= u.x;
    y -= u.y;
    return *this;
}

Vector2D Vector2D::operator+=(const Vector2D& u) {
    x += u.x;
    y += u.y;
    return *this;
}

Vector2D Vector2D::operator*=(float c) {
    x *= c;
    y *= c;
    return *this;
}

Vector2D Vector2D::operator/=(float c) {
    x /= c;
    y /= c;
    return *this;
}

float Vector2D::dot(const Vector2D& u) const {
    return x * u.x + y * u.y;
}

Vector2D Vector2D::cross(const Vector2D& u) const {
    return Vector2D(
        y * u.x - x * u.y,
        x * u.y - y * u.x
    );
}

float Vector2D::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalize() const {
    float m = std::sqrt(x * x + y * y);
    if (m != 0) {
        return Vector2D(x / m, y / m);
    }
    return Vector2D();
}

float Vector2D::distance(const Vector2D& u) const {
    Vector2D d = Vector2D(x - u.x, y - u.y);
    return std::sqrt(d.x * d.x + d.y * d.y);
}

std::string Vector2D::str() const {
    std::stringstream sstm;
    sstm << "(" << x << ", " << y << ")";
    return sstm.str();
}

inline Vector2D operator*(float c, Vector2D& u) {
    return u * c;
}

inline std::ostream& operator<<(std::ostream& Str, Vector2D& v) {
    std::cout << "printando";
    Str << "(" << v.x << ", " << v.y << ")";
}