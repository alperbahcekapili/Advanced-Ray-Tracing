

#include <vector>

#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))




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