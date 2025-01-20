#pragma once

#include "Object.h"
#include "Triangle.h"
#include <array>
#include "../acceleration/BVH.h"
class Mesh : public Object
{
private:
    


public:

    Object** faces;
    Material* material;
    ObjectType objectType;
    TransformationMatrix* tm;
    int num_faces;
    Object* last_intersected_obj;
    TextureMap* texture_maps;
    std::vector<std::pair<float, float> > uv_coords_mesh;
    int num_tex_maps;
    texture_flags tex_flags;
    Mesh(Material* material, ObjectType objectType,  Vec3* faces, int numfaces, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps, std::vector<std::pair<float, float> > uv_coords_mesh);
    ~Mesh();
    BVH* bvh_faces;
    float Intersects(Ray ray) override ;
    Vec3 getSurfaceNormal(Ray r) override;
    TransformationMatrix* gettm() override;
    Material* getMaterial() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
    Vec3 getMotionBlur() override;
    Vec3 center;

    int get_num_tex_maps() override;
    TextureMap* get_texture_maps() override;
    texture_flags get_texture_flags() override;

};
