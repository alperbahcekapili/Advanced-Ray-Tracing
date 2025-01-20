#include "SphereLight.h"
SphereLight::SphereLight(Vec3 center, float R, Material* material, ObjectType objectType, TransformationMatrix* tm, int num_tex_maps, TextureMap* texture_maps, Vec3 radiance)
:Sphere(center, R, material, objectType, tm, num_tex_maps, texture_maps)
{
    this->R = R;
    this->center = center;
    this->material = material;
    this->objectType = SphereLightType;
    this->tex_flags = texture_flags();
    this->texture_maps = texture_maps;
    this->num_tex_maps = num_tex_maps;
    


    
    this->tex_flags.any = false;

    
    this->tm = new TransformationMatrix();
    
    Vec3 point_on_sphere = Vec3(center.x + R, center.y, center.z);
    TransformationMatrix* to_center = new TransformationMatrix(-1*center, 't');
    TransformationMatrix* from_center = new TransformationMatrix(center, 't');

    *(this->tm) = (*from_center) * (*tm) * (*to_center);
    
    center = this->tm->transform(this->center);
    this->center = center;
    
    Vec3 new_pointon = this->tm->transform(point_on_sphere);
    R = (new_pointon-center).magnitude();
    this->R = R;

    this->min = Vec3(center.x-R, center.y-R, center.z-R);
    this->max = Vec3(center.x+R, center.y+R, center.z+R);

    this->radiance = radiance;
    
}






// TODO Update below functions
Vec3  SphereLight::irradianceAt(Ray light_ray, Vec3  location)  {
    float distance = (this->center - location).magnitude();
    return this->radiance;
}

Vec3 SphereLight::getPointOn(){
    float dir1 = generate_random_01()-0.5;
    float dir2 = generate_random_01()-0.5;
    float dir3 = generate_random_01()-0.5;
    Vec3 vec1 = Vec3(dir1, dir2, dir3).normalize();
    return this->center + vec1 * this->R;

}






SphereLight::~SphereLight(){

}