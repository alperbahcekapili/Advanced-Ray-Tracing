#pragma once

#include "Object.h"
#include "Triangle.h"
#include <array>
#include "../acceleration/BVH.h"
class Mesh : public Object
{
private:
    


public:

    Object* faces;
    Material* material;
    ObjectType objectType;
    int num_faces;
    Object* last_intersected_obj;
    Mesh(Material* material, ObjectType objectType,  Vec3* faces, int numfaces, TransformationMatrix* tm);
    ~Mesh();
    BVH* bvh_faces;
    float Intersects(Ray ray) override ;
    Vec3 getSurfaceNormal(Ray r) override;
    TransformationMatrix* gettm() override;
    Material* getMaterial() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
    Vec3 center;

};
