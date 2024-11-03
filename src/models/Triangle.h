#pragma once
#include "../util/data_structures.h"
#include "Object.h"
#include "Mesh.h"

class Mesh;

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
    Mesh* mesh;
    TransformationMatrix* tm;
    Triangle(Material* material, ObjectType objectType, Vec3 v1, Vec3 v2, Vec3 v3, TransformationMatrix* tm);
    float Intersects(Ray ray) override ;
    float getArea(void);
    Vec3 getSurfaceNormal(Ray r) override;
    Triangle();
    Material *getMaterial() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
};
