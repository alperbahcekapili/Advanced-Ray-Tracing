#pragma once

#include "Ray.h"
#include "Material.h"
#include "../shaders/TextureMap.h"

struct texture_flags{
    bool replace_kd=false;
    bool blend_kd=false;
    bool replace_ks=false;
    bool bump_normal=false;
    bool replace_all=false;
    bool replace_normal=false;
    bool any=false;
    TextureMap* replace_kd_texture;
    TextureMap* blend_kd_texture;
    TextureMap* replace_ks_texture;
    TextureMap* bump_normal_texture;
    TextureMap* replace_all_texture;
    TextureMap* replace_normal_texture;

};

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
    int num_tex_maps;
    ObjectType objectType;
    Vec3 min;
    Vec3 max;
    Vec3 motionBlur;
    texture_flags tex_flags;
    TextureMap* texture_maps;
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
    virtual int get_num_tex_maps() =0;
    virtual TextureMap* get_texture_maps() = 0;
    virtual texture_flags get_texture_flags() = 0;
    
    // virtual void transform(TransformationMatrix tm) = 0;
    
    
};