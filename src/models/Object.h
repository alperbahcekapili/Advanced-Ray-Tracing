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
    TransformationMatrix* tm;
    
    ObjectType objectType;
    Vec3 min;
    Vec3 max;
    Vec3 motionBlur;
    int id; // Used as unqiue id 
    virtual float Intersects(Ray ray)  = 0;
    virtual Vec3 getSurfaceNormal(Ray r) = 0;
    virtual ~Object() = default ;
    virtual Material* getMaterial() = 0;
    virtual ObjectType getObject() = 0;
    virtual Vec3 getBoundingBox(bool isMax) = 0;
    virtual Vec3  getCenter() = 0;
    virtual Vec3 getMotionBlur() = 0;
    virtual TransformationMatrix* gettm() = 0;
    // virtual void transform(TransformationMatrix tm) = 0;
    
    
};