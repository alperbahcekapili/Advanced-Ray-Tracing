#pragma once
#include "../util/data_structures.h"
#include "Object.h"
class Triangle : public Object
{
private:
    Material* material;
    ObjectType objectType;
    


public:

    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
    Vec3 normal;
    Triangle(Material* material, ObjectType objectType, Vec3 v1, Vec3 v2, Vec3 v3);
    float Intersects(Ray ray) override ;
    float getArea(void);
    Vec3 getSurfaceNormal(Vec3 location) override;
    Triangle();
    Material *getMaterial() override;
    ObjectType getObject() override;
};
