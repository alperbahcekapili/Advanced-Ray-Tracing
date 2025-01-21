#pragma once
#include "../util/data_structures.h"
#include "Object.h"

class ObjectInstance : public Object
{
private:
    /* data */
public:


    Object* parent;
    bool reset; // reset transform of parent object
    texture_flags tex_flags;
    TransformationMatrix* tm;
    float Intersects(Ray ray) override ;

    Vec3 getSurfaceNormal(Ray r) override;
    Material *getMaterial() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
    Vec3 getMotionBlur() override;
    TransformationMatrix* gettm() override;
    ObjectInstance(Object* parent, bool reset, TransformationMatrix* resulting_tm, Material* material);
    ~ObjectInstance();



    int get_num_tex_maps() override;
    TextureMap* get_texture_maps() override;
    texture_flags get_texture_flags() override;


};
