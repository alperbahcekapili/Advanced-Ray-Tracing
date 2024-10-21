#pragma once

#include "Object.h"
#include "Triangle.h"

class Mesh : public Object
{
private:
    


public:

    std::vector<Triangle> faces;
    Material* material;
    ObjectType objectType;
    int num_faces;
    Mesh(Material* material, ObjectType objectType, std::vector< std::array<std::array<float, 3>, 3>>  faces, int numfaces);
    float Intersects(Ray ray) override ;
    vector<float> getSurfaceNormal(vector<float> location) override;
    Material* getMaterial() override;
    ObjectType getObject() override;

};
