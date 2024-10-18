#pragma once

#include "Object.h"
class Triangle : public Object
{
private:
    float v1[3];
    float v2[3];
    float v3[3];
    Material* material;
    ObjectType objectType;
    


public:
    Triangle(Material* material, ObjectType objectType, float v1[3], float v2[3], float v3[3]);
    float Intersects(Ray ray) override ;
    float getArea(void);
    vector<float> getSurfaceNormal(vector<float> location) override;
    Material* getMaterial() override;
    ObjectType getObject() override;
};
