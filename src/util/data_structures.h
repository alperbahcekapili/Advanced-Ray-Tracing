#pragma once
#include <cmath>
#include <array>
#include <algorithm>


struct Vec3 {
    float x, y, z;

    // Constructor
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}


    // Calculate the centroid
    static Vec3 centroid(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
        return Vec3{
            (v1.x + v2.x + v3.x) / 3,
            (v1.y + v2.y + v3.y) / 3,
            (v1.z + v2.z + v3.z) / 3
        };
    }

    // Calculate the signed angle from the centroid for sorting
    static float signedAngle(const Vec3& a, const Vec3& b, const Vec3& normal) {
        Vec3 ab = b - a;
        Vec3 refVec{1.0f, 0.0f, 0.0f};  // Reference vector in x direction
        float angle = std::atan2(ab.cross(refVec).z, ab.x);  // Get the angle in the xy plane

        // Adjust angle based on normal direction for clockwise ordering
        if (normal.z < 0) {
            angle = -angle;
        }

        return angle;
    }

    // Sort vertices in clockwise order
    static void sortVerticesClockwise(Vec3& v1, Vec3& v2, Vec3& v3) {
        Vec3 center = centroid(v1, v2, v3);
        Vec3 normal = (v2 - v1).cross(v3 - v1);  // Calculate the normal for orientation

        // Sort vertices based on signed angle with respect to the centroid and normal
        std::array<Vec3, 3> vertices = {v1, v2, v3};
        std::sort(vertices.begin(), vertices.end(), [&](const Vec3& a, const Vec3& b) {
            return signedAngle(center, a, normal) < signedAngle(center, b, normal);
        });

        // Update v1, v2, v3 with sorted vertices
        v1 = vertices[0];
        v2 = vertices[1];
        v3 = vertices[2];
    }

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