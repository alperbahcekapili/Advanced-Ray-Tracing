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
    Vec3 min;
    Vec3 max;
    virtual float Intersects(Ray ray)  = 0;
    virtual Vec3 getSurfaceNormal(Vec3 location) = 0;
    virtual ~Object() = default ;
    virtual Material* getMaterial() = 0;
    virtual ObjectType getObject() = 0;
    virtual Vec3 getBoundingBox(bool isMax) = 0;
    virtual Vec3  getCenter() = 0;
    
};