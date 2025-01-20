#include "Sphere.h"
#include "../lights/Light.h"

class SphereLight: public Sphere, public Light
{
private:
    /* data */
public:
    SphereLight(Vec3 center, float R, Material* material, ObjectType objectType, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps, Vec3 radiance);
    ~SphereLight();

    Vec3 irradianceAt(Ray light_ray, Vec3  location) ;
    Vec3 getPointOn();
};



