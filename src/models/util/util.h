#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <vector>
#include <string>
#include <cassert>
#include "../Ray.h"

// Macro for assertions with messages
#define assertm(exp, msg) assert(((void)msg, exp))

// Function declarations


// Function to add two vectors element-wise
std::vector<float> vectorAdd(std::vector<float> v1, std::vector<float> v2);

// Function to scale a vector by a given magnitude
std::vector<float> vectorScale(std::vector<float> v1, float magnitude);

// Function to multiply two vectors element-wise
std::vector<float> vectorMultiplyElementwise(std::vector<float> v1, std::vector<float> v2);

// Function to compute the cross product of two 3D vectors
std::vector<float> vectorCrossProduct3D(std::vector<float> vec1, std::vector<float> vec2);

float dotProduct(std::vector<float> vec1, std::vector<float> vec2);

Ray createRayFrom(vector<float> start, vector<float> destination);

float getMagnitude(std::vector<float> v);

#endif // VECTOR_UTILS_H
