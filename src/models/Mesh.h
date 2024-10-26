#pragma once

#include "Object.h"
#include "Triangle.h"
#include <array>
class Mesh : public Object
{
private:
    


public:

    Triangle* faces;
    Material* material;
    ObjectType objectType;
    int num_faces;
    Mesh(Material* material, ObjectType objectType,  Vec3* faces, int numfaces);
    ~Mesh();
    float Intersects(Ray ray) override ;
    Vec3 getSurfaceNormal(Vec3 location) override;
    Material* getMaterial() override;
    ObjectType getObject() override;

};
