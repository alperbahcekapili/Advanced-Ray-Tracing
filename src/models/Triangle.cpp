
#include "Triangle.h"
#include "util/util.h"
#include <math.h>

// Function to calculate the magnitude of this->v1 vector
float Triangle::magnitude(const std::vector<float> vec) {
    return std::sqrt(vec.at(0) * vec.at(0) + vec.at(1) * vec.at(1) + vec.at(2) * vec.at(2));
}


Triangle::Triangle(Material* material, ObjectType objectType, float v1[3], float v2[3], float v3[3])
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
    std::cout << this->v1[0];
    std::cout << this->v1[1];
    std::cout << this->v1[2];
};

// Function to calculate the area of this->v1 triangle given its vertices
float Triangle::getArea(void) {
    // Vectors AB and AC
    
    std::vector<float> AB = { this->v2[0] - this->v1[0], this->v2[1] - this->v1[1], this->v2[2] - this->v1[2] };
    std::vector<float> AC = { this->v3[0] - this->v1[0], this->v3[1] - this->v1[1], this->v3[2] - this->v1[2] };

    // Cross product of AB and AC
    std::vector<float> cross = vectorCrossProduct3D(AB, AC);

    // Area is half the magnitude of the cross product
    return 0.5 * magnitude(cross);
}