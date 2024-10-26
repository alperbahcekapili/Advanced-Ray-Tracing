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
    virtual float Intersects(Ray ray)  = 0;
    virtual Vec3 getSurfaceNormal(Vec3 location) = 0;
    virtual ~Object() = default ;
    virtual Material* getMaterial() = 0;
    virtual ObjectType getObject() = 0;
};