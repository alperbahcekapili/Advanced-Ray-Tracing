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
    Vec3 T;
    Vec3 B;
    bool tbset=false;
    Vec3 normal;
    Mesh* mesh;
    TransformationMatrix* tm;
    texture_flags tex_flags;
    TextureMap* texture_maps;
    std::vector<std::pair<float, float> > uv_coords_triangle;
    int num_tex_maps;
    Triangle(Material* material, ObjectType objectType, Vec3 v1, Vec3 v2, Vec3 v3, TransformationMatrix* tm, Mesh* mesh, int num_tex_maps, TextureMap* texture_maps, std::vector<std::pair<float, float> > uv_coords_triangle);
    float Intersects(Ray ray) override ;
    TransformationMatrix* gettm() override;

    Vec3 getSurfaceNormal(Ray r) override;
    Triangle();
    Material *getMaterial() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
    Vec3 getMotionBlur() override;

    int get_num_tex_maps() override;
    TextureMap* get_texture_maps() override;
    texture_flags get_texture_flags() override;
};
