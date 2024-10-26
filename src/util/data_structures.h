#pragma once
#include <cmath>

struct Vec3 {
    float x, y, z;

    // Constructor
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Addition operator
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator/(const Vec3& other) const {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }
    // Scalar multiplication (scaling)
    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    // Overload for scalar * Vec3 (enables float * Vec3 syntax)
    friend Vec3 operator/(float scalar, const Vec3& vec) {
        return Vec3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }


    // Subtraction operator
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    // Dot product
    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Magnitude
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalize
    Vec3 normalize() const {
        float mag = magnitude();
        return (mag != 0) ? Vec3(x / mag, y / mag, z / mag) : Vec3(0, 0, 0);
    }

    // Element-wise multiplication
    Vec3 operator*(const Vec3& other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    // Scalar multiplication (scaling)
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    // Overload for scalar * Vec3 (enables float * Vec3 syntax)
    friend Vec3 operator*(float scalar, const Vec3& vec) {
        return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

};



struct Vec4 {
    float x, y, z, h;
    Vec4(float x = 0, float y = 0, float z = 0, float h = 0) : x(x), y(y), z(z), h(h) {}
};