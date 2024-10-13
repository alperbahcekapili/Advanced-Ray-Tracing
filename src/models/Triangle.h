#pragma once

#include "Object.h"
class Triangle 
{
private:
    float v1[3];
    float v2[3];
    float v3[3];
    Material* material;
    ObjectType objectType;

    static float magnitude(const std::vector<float> vec);


public:
    Triangle(Material* material, ObjectType objectType, float v1[3], float v2[3], float v3[3]);
    float getArea(void);
};
