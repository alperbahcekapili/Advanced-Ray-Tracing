
#include "AreaLight.h"
#include "Light.h"
#include <vector>
#include "../util/util.h"
#include <math.h>


AreaLight::AreaLight(float width, Vec3 position, Vec3 normal, Vec3 radiance){
    this->width = width;
    this->location = position;
    this->normal = normal;
    this->radiance = radiance;
    this->area = width * width;
    this->ltype = LightType::AreaLightType;
}



Vec3 AreaLight::irradianceAt(Ray light_ray, Vec3  location){

   
    Vec3 sample_loc = light_ray.o;
    Vec3 light_direction = light_ray.d;

    float distance = (location - sample_loc).magnitude();
    float eq_top = this->normal.dot(light_direction);
    float dwi = this->area * eq_top / (distance*distance);

    return this->radiance * dwi;
}





Vec3 AreaLight::getPointOn(){
    // assumes area light is always parallel to axis

    float randompx = 0.5;//generate_random_01();
    float randompy = 0.5;//generate_random_01();
    
    // map range to [-size/2, size/2]

    Ray dummy =  Ray(Vec3(), this->normal);
    Vec3 ** tangents = dummy.getTangentVectors();
    Vec3 tangent1 = *tangents[0];
    Vec3 tangent2 = *tangents[1];
    // now create offset vector out of these tangent vectors
    Vec3 offset = tangent1 * randompx + tangent2 * randompy;
    return this->location + offset;
}
