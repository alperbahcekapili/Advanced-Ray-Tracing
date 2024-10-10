
#include "Triangle.h"


// Function to calculate the magnitude of this->v1 vector
double Triangle::magnitude(const std::array<double, 3>& vec) {
    return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// Function to calculate the cross product of two vectors
std::array<double, 3>  Triangle::crossProduct(const std::array<double, 3>& vec1, const std::array<double, 3>& vec2) {
    return {
        vec1[1] * vec2[2] - vec1[2] * vec2[1], // x component
        vec1[2] * vec2[0] - vec1[0] * vec2[2], // y component
        vec1[0] * vec2[1] - vec1[1] * vec2[0]  // z component
    };
}



Triangle::Triangle(Material* material, ObjectType objectType, double v1[3], double v2[3], double v3[3]) : Object(material, objectType)
{
    this->material = material;
    this->objectType = objectType;
    this->v1[0] = v1[0];
    this->v1[1] = v1[1];
    this->v1[2] = v1[2];
    this->v2[0] = v2[0];
    this->v2[1] = v2[1];
    this->v2[2] = v2[2];
    this->v3[0] = v3[0];
    this->v3[1] = v3[1];
    this->v3[2] = v3[2];

    std::cout << "I initialize as triangle";
    std::cout << this->v1[0];
    std::cout << this->v1[1];
    std::cout << this->v1[2];
};

// Function to calculate the area of this->v1 triangle given its vertices
double Triangle::getArea(void) {
    // Vectors AB and AC
    
    std::array<double, 3> AB = { this->v2[0] - this->v1[0], this->v2[1] - this->v1[1], this->v2[2] - this->v1[2] };
    std::array<double, 3> AC = { this->v3[0] - this->v1[0], this->v3[1] - this->v1[1], this->v3[2] - this->v1[2] };
    
    // Cross product of AB and AC
    std::array<double, 3> cross = crossProduct(AB, AC);

    // Area is half the magnitude of the cross product
    return 0.5 * magnitude(cross);
}