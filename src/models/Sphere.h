#include "Object.h"
#include <vector>

class Sphere : public Object
{
private:
    
public:
    Vec3 center;
    float R;
    Material* material;
    ObjectType objectType; 
    texture_flags tex_flags;
    int num_tex_maps;
    TextureMap* texture_maps;
    Sphere(Vec3 center, float R, Material* material, ObjectType objectType, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps);
    float Intersects(Ray ray) override ;
    Vec3 getSurfaceNormal(Ray r) override;
    Material* getMaterial() override;
    TransformationMatrix* gettm() override;
    ObjectType getObject() override;
    Vec3 getBoundingBox(bool isMax) override;
    Vec3  getCenter() override;
Vec3 getMotionBlur() override;
    
    
    int get_num_tex_maps() override;
    TextureMap* get_texture_maps() override;
    texture_flags get_texture_flags() override;
};

