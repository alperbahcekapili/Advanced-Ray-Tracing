#pragma once

#include "Ray.h"
#include "Material.h"

enum ObjectType{
    TriangleType,
    SphereType,
    MeshType
};

class Object
{
private:
    /* data */
public:

    Material* material;
    ObjectType objectType;
    virtual float Intersects(Ray ray) ;
    virtual vector<float> getSurfaceNormal(vector<float> location);
    virtual ~Object() = default;
};