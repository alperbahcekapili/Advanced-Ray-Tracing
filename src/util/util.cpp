

#include <vector>
#include <cassert>
#include "../models/Ray.h"
#include "util.h"

using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))



float det3x3(float m[3][3]){
    // satir, sutun
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[1][0] * (m[0][2] * m[2][1] - m[0][1] * m[2][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

std::vector<float> clipValues(std::vector<float> v, float clip_val){
    for (size_t i = 0; i < v.size(); i++)
    {
        if(v.at(0) > clip_val)
            v.at(0) = clip_val;
        if(v.at(1) > clip_val)
            v.at(1) = clip_val;
        if(v.at(2) > clip_val)
            v.at(2) = clip_val;
    }
    return v;
    
}

std::vector<float> vectorAdd(std::vector<float> v1, std::vector<float> v2){
    assertm(v1.size() == v2.size(), "Dimensions do not match in std::vector add") ;
    std::vector<float> resultingVector(v1);
    for (size_t i = 0; i < v1.size(); i++)
    {
        resultingVector.at(i) += v2.at(i);
    }
    return resultingVector;
}

std::vector<float> vectorScale(std::vector<float> v1, float magnitude){
    std::vector<float> resultingVector(v1);
    for (size_t i = 0; i < v1.size(); i++)
    {
        resultingVector.at(i) *= magnitude;
    }
    return resultingVector;
}

std::vector<float> vectorMultiplyElementwise(std::vector<float> v1, std::vector<float> v2){
    assertm(v1.size() == v2.size(), "Dimensions do not match in std::vector multiply elementwise") ;
    std::vector<float> resultingVector(v1);
    for (size_t i = 0; i < v1.size(); i++)
    {
        resultingVector.at(i) *= v2.at(i);
    }
    return resultingVector;
}


std::vector<float> vectorCrossProduct3D(std::vector<float> vec1, std::vector<float> vec2) {
    assertm(vec1.size() == vec2.size(), "Dimensions do not match in cross product 3d") ;
    std::vector<float> resultingVectoor;
    resultingVectoor.push_back(vec1.at(1) * vec2.at(2) - vec1.at(2) * vec2.at(1));
    resultingVectoor.push_back(vec1.at(2) * vec2.at(0) - vec1.at(0) * vec2.at(2));
    resultingVectoor.push_back(vec1.at(0) * vec2.at(1) - vec1.at(1) * vec2.at(0)); 
    return resultingVectoor;
}

float dotProduct(std::vector<float> vec1, std::vector<float> vec2){
    assertm(vec1.size() == vec2.size(), "Dimensions do not match in std::vector dot Product") ;
    float cum = 0;
    for (size_t i = 0; i < vec1.size(); i++)
    {
        cum += vec1.at(i) * vec2.at(i);
    }
    return cum;
    
}


Ray createRayFrom(vector<float> start, vector<float> destination){
    // In order to create ray from start and destination we need to substract start from destination
    vector<float> scaledRay = vectorAdd(destination, vectorScale(start, -1));
    return Ray(start, scaledRay);
}

vector<float> normalize(vector<float> v){
    // returns unit vector with the same direction
    vector<float> unitVector = vectorScale(v, 1/sqrt(dotProduct(v, v)));
    return unitVector;
}

float getMagnitude(std::vector<float> v){
    return dotProduct(v,v);
}