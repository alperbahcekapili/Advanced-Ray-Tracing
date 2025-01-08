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
    MeshType,
    MeshLightType,
    SphereLightType,
    TriangleLightType

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
    Vec3 radiance = Vec3(0,0,0); // TODO: remove this
    Vec3 min;
    Vec3 max;
    Vec3 motionBlur;
    texture_flags tex_flags;
    TextureMap* texture_maps;
    int id; // Used as unqiue id 
    virtual float Intersects(Ray ray)  {};
    virtual Vec3 getSurfaceNormal(Ray r) {};
    virtual ~Object() = default ;
    virtual Material* getMaterial() {};
    virtual ObjectType getObject() {};
    virtual Vec3 getBoundingBox(bool isMax) {};
    virtual Vec3  getCenter() {};
    virtual Vec3 getMotionBlur() {};
    virtual TransformationMatrix* gettm() {};
    virtual int get_num_tex_maps() =0;
    virtual TextureMap* get_texture_maps() {};
    virtual texture_flags get_texture_flags() {};
    
    // virtual void transform(TransformationMatrix tm) {};
    
    
};